#pragma once
#include <initializer_list>
#include <stdexcept>

#include "../Vector/Vector4i.h"

class Matrix4i
{
public:
    Matrix4i()
    {
        for (int i=0;i<16;i++)
        {
            matrix_value_[i]=0;
        }
    }
    
    Matrix4i(const Matrix4i& m)
    {
        for (int i=0;i<16;i++)
        {
            matrix_value_[i]=m.matrix_value_[i];
        }
    }
    
    Matrix4i(const std::initializer_list<int>& list)
    {
        if (list.size()!=16)
        {
            throw std::out_of_range("Argument list must be of size 16");
        }
        const int* ptr=list.begin();
        for (int i=0;i<16;i++)
        {
            matrix_value_[i]=*(ptr+i);
        }
    }
    
    Matrix4i(const std::initializer_list<std::initializer_list<int>>& list)
    {
        if (list.size()!=4)
        {
            throw std::out_of_range("Argument list must be of size 4");
        }
        const std::initializer_list<int>* ptr=list.begin();
        for (int i=0;i<4;i++)
        {
            if ((ptr+i)->size()!=4)
            {
                throw std::out_of_range("Argument list must be of size 4");
            }
            const int* ptr2=(ptr+i)->begin();
            for (int j=0;j<4;j++)
            {
                matrix_value_[i*4+j]=*(ptr2+j);
            }
        }
    }
    
    Matrix4i& operator=(const Matrix4i& m)
    {
        if (this!=&m)
        {
            for (int i=0;i<16;i++)
            {
                matrix_value_[i]=m.matrix_value_[i];
            }
        }
        return *this;
    }
    
    int& at(const int row,const int col)
    {
        if (row<0||row>=4||col<0||col>=4)
        {
            throw std::out_of_range("Index out of range");
        }
        return matrix_value_[row*4+col];
    }
    
    Matrix4i operator*(const Matrix4i& m) const
    {
        Matrix4i result;
        for (int i=0;i<4;i++)
        {
            for (int j=0;j<4;j++)
            {
                int sum=0;
                for (int k=0;k<4;k++)
                {
                    sum+=matrix_value_[i*4+k]*m.matrix_value_[k*4+j];
                }
                result.matrix_value_[i*4+j]=sum;
            }
        }
        return result;
    }
    
    Vector4i operator*(const Vector4i& v) const
    {
        Vector4i temp=v;
        Vector4i result;
        result.x()=matrix_value_[0]*temp.x()+matrix_value_[1]*temp.y()+matrix_value_[2]*temp.z()+matrix_value_[3]*temp.w();
        result.y()=matrix_value_[4]*temp.x()+matrix_value_[5]*temp.y()+matrix_value_[6]*temp.z()+matrix_value_[7]*temp.w();
        result.z()=matrix_value_[8]*temp.x()+matrix_value_[9]*temp.y()+matrix_value_[10]*temp.z()+matrix_value_[11]*temp.w();
        result.w()=matrix_value_[12]*temp.x()+matrix_value_[13]*temp.y()+matrix_value_[14]*temp.z()+matrix_value_[15]*temp.w();
        return result;
    }
    
    Matrix4i multiply(const Matrix4i& m) const
    {
        return (*this)*m;
    }
    
    Vector4i multiply(const Vector4i& v) const
    {
        return (*this)*v;
    }
    
    Matrix4i operator*(const int& num) const
    {
        Matrix4i result;
        for (int i=0;i<16;i++)
        {
            result.matrix_value_[i]=matrix_value_[i]*num;
        }
        return result;
    }
    
    Matrix4i operator/(const int& num) const
    {
        Matrix4i result;
        for (int i=0;i<16;i++)
        {
            result.matrix_value_[i]=matrix_value_[i]/num;
        }
        return result;
    }
    
    void operator*=(const int& num)
    {
        for (int i=0;i<16;i++)
        {
            matrix_value_[i]*=num;
        }
    }
    
    void operator/=(const int& num)
    {
        for (int i=0;i<16;i++)
        {
            matrix_value_[i]/=num;
        }
    }
    
    bool operator==(const Matrix4i& m) const
    {
        for (int i=0;i<16;i++)
        {
            if (matrix_value_[i]!=m.matrix_value_[i])
            {
                return false;
            }
        }
        return true;
    }
    
    bool operator!=(const Matrix4i& m) const
    {
        for (int i=0;i<16;i++)
        {
            if (matrix_value_[i]!=m.matrix_value_[i])
            {
                return true;
            }
        }
        return false;
    }
    
    Matrix4i transpose() const
    {
        Matrix4i result;
        for (int i=0;i<4;i++)
        {
            for (int j=0;j<4;j++)
            {
                result.matrix_value_[j*4+i]=matrix_value_[i*4+j];
            }
        }
        return result;
    }
    
    int determinant() const
    {
        int det=0;
        for (int j=0;j<4;j++)
        {
            const int sign=(j%2==0)?1:-1;
            det+=sign*matrix_value_[j]*minor_det(0,j);
        }
        return det;
    }
    
    static Matrix4i identity()
    {
        Matrix4i result;
        result.matrix_value_[0]=1;
        result.matrix_value_[5]=1;
        result.matrix_value_[10]=1;
        result.matrix_value_[15]=1;
        return result;
    }
private:
    int matrix_value_[16];
    
    // 计算划去 row 行 col 列后剩余 3 阶子矩阵的行列式（余子式）
    int minor_det(const int row,const int col) const
    {
        int sub[9];
        int idx=0;
        for (int i=0;i<4;i++)
        {
            if (i==row)
            {
                continue;
            }
            for (int j=0;j<4;j++)
            {
                if (j==col)
                {
                    continue;
                }
                sub[idx++]=matrix_value_[i*4+j];
            }
        }
        return sub[0]*(sub[4]*sub[8]-sub[5]*sub[7])
              -sub[1]*(sub[3]*sub[8]-sub[5]*sub[6])
              +sub[2]*(sub[3]*sub[7]-sub[4]*sub[6]);
    }
};
