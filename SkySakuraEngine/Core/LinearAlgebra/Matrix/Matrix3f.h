#pragma once
#include <initializer_list>
#include <stdexcept>

#include "Matrix3i.h"
#include "../Vector/Vector3f.h"

class Matrix3f
{
public:
    Matrix3f()
    {
        matrix_value_={0};
    }
    
    Matrix3f(const Matrix3f& m)
    {
        for (int i=0;i<9;i++)
        {
            matrix_value_[i]=m.matrix_value_[i];
        }
    }
    
    Matrix3f(const std::initializer_list<float>& list)
    {
        if (list.size()!=9)
        {
            throw std::out_of_range("Argument list must be of size 4");
        }
        const float* ptr=list.begin();
        for (int i=0; i<9; i++)
        {
            matrix_value_[i]=*(ptr+i);
        }
    }
    
    Matrix3f(const std::initializer_list<std::initializer_list<float>>& list)
    {
        if (list.size()!=3||list.begin()->size()!=3||
            (list.begin()+1)->size()!=3||(list.begin()+2)->size()!=3)
        {
            throw std::out_of_range("Argument list must be of size 4");
        }
        const std::initializer_list<float>* ptr=list.begin();
        for (int i=0; i<3; i++)
        {
            const float* ptr2=(ptr+i)->begin();
            for (int j=0; j<2; j++)
            {
                matrix_value_[i*3+j]=*(ptr2+j);
            }
        }
    }
    
    Matrix3f(Matrix3i& mi)
    {
        for (int i=0; i<3; i++)
        {
            for (int j=0; j<3; j++)
            {
                matrix_value_[i*3+j]=static_cast<float>(mi.at(i,j));
            }
        }
    }
    
    Matrix3f& operator=(const Matrix3f& m)
    {
        if (this!=&m)
        {
            for (int i=0;i<9;i++)
            {
                matrix_value_[i]=m.matrix_value_[i];
            }
        }
        return *this;
    }
    
    Matrix3f& operator=(const std::initializer_list<float>& list)
    {
        if (list.size()!=9)
        {
            throw std::out_of_range("Argument list must be of size 4");
        }
        const float* ptr=list.begin();
        for (int i=0; i<9; i++)
        {
            matrix_value_[i]=*(ptr+i);
        }
        return *this;
    }
    
    Matrix3f& operator=(const std::initializer_list<std::initializer_list<float>>& list)
    {
        if (list.size()!=3||list.begin()->size()!=3||
            (list.begin()+1)->size()!=3||(list.begin()+2)->size()!=3)
        {
            throw std::out_of_range("Argument list must be of size 4");
        }
        const std::initializer_list<float>* ptr=list.begin();
        for (int i=0; i<3; i++)
        {
            const float* ptr2=(ptr+i)->begin();
            for (int j=0; j<2; j++)
            {
                matrix_value_[i*3+j]=*(ptr2+j);
            }
        }
        return *this;
    }
    
    Matrix3f& operator=(Matrix3i& mi)
    {
        for (int i=0; i<3; i++)
        {
            for (int j=0; j<3; j++)
            {
                matrix_value_[i*3+j]=static_cast<float>(mi.at(i,j));
            }
        }
        return *this;
    }
    
    float& at(int row,int col)
    {
        if (row>=3||col>=3||row<0||col<0)
        {
            throw std::runtime_error("You cannot get a element that index is not in the matrix");
        }
        return matrix_value_[row*3+col];
    }
    
    Vector3f operator*(Vector3f& v) const
    {
        Vector3f result;
        result.x()=matrix_value_[0]*v.x()+matrix_value_[1]*v.y()+matrix_value_[2]*v.z();
        result.y()=matrix_value_[3]*v.x()+matrix_value_[4]*v.y()+matrix_value_[5]*v.z();
        result.z()=matrix_value_[6]*v.x()+matrix_value_[7]*v.y()+matrix_value_[8]*v.z();
        return result;
    }
    
    Matrix3f operator*(const Matrix3f& m) const
    {
        Matrix3f result;
        for (int i=0;i<3;i++)
        {
            for (int j=0;j<3;j++)
            {
                for (int k=0;k<3;k++)
                {
                    result.at(i,j)+=matrix_value_[i*3+k]*m.matrix_value_[k*3+j];
                }
            }
        }
        return result;
    }
    
    Vector3f multiply(Vector3f& v) const
    {
        Vector3f result;
        result.x()=matrix_value_[0]*v.x()+matrix_value_[1]*v.y()+matrix_value_[2]*v.z();
        result.y()=matrix_value_[3]*v.x()+matrix_value_[4]*v.y()+matrix_value_[5]*v.z();
        result.z()=matrix_value_[6]*v.x()+matrix_value_[7]*v.y()+matrix_value_[8]*v.z();
        return result;
    }

    [[nodiscard]] Matrix3f multiply(const Matrix3f& m) const
    {
        Matrix3f result;
        for (int i=0;i<3;i++)
        {
            for (int j=0;j<3;j++)
            {
                for (int k=0;k<3;k++)
                {
                    result.at(i,j)+=matrix_value_[i*3+k]*m.matrix_value_[k*3+j];
                }
            }
        }
        return result;
    }
    
    Matrix3f operator*(const float& num)
    {
        Matrix3f result;
        for (int i=0;i<3;i++)
        {
            for (int j=0;j<3;j++)
            {
                result.at(i,j)=this->at(i,j)*num;
            }
        }
        return result;
    }
    
    Matrix3f operator/(const float& num)
    {
        Matrix3f result;
        float new_num=1/num;
        for (int i=0;i<3;i++)
        {
            for (int j=0;j<3;j++)
            {
                result.at(i,j)=this->at(i,j)*new_num;
            }
        }
        return result;
    }
    
    Matrix3f operator*(const int& num)
    {
        Matrix3f result;
        float new_num= static_cast<float>(num);
        for (int i=0;i<3;i++)
        {
            for (int j=0;j<3;j++)
            {
                result.at(i,j)=this->at(i,j)*new_num;
            }
        }
        return result;
    }
    
    Matrix3f operator/(const int& num)
    {
        Matrix3f result;
        float new_num= static_cast<float>(1./num);
        for (int i=0;i<3;i++)
        {
            for (int j=0;j<3;j++)
            {
                result.at(i,j)=this->at(i,j)*new_num;
            }
        }
        return result;
    }
    
    friend Matrix3f operator*(const float& num, Matrix3f& m)
    {
        Matrix3f result;
        for (int i=0;i<3;i++)
        {
            for (int j=0;j<3;j++)
            {
                result.at(i,j)=m.at(i,j)*num;
            }
        }
        return result;
    }
    
    friend Matrix3f operator/(const float& num, Matrix3f& m)
    {
        Matrix3f result;
        float new_num=1/num;
        for (int i=0;i<3;i++)
        {
            for (int j=0;j<3;j++)
            {
                result.at(i,j)=m.at(i,j)*new_num;
            }
        }
        return result;
    }
    
    friend Matrix3f operator*(const int& num,Matrix3f& m)
    {
        Matrix3f result;
        float new_num= static_cast<float>(num);
        for (int i=0;i<3;i++)
        {
            for (int j=0;j<3;j++)
            {
                result.at(i,j)=m.at(i,j)*new_num;
            }
        }
        return result;
    }
    
    friend Matrix3f operator/(const int& num, Matrix3f& m)
    {
        Matrix3f result;
        float new_num= static_cast<float>(1./num);
        for (int i=0;i<3;i++)
        {
            for (int j=0;j<3;j++)
            {
                result.at(i,j)=m.at(i,j)*new_num;
            }
        }
        return result;
    }
    
    void operator*=(const float& num)
    {
        for (int i=0;i<3;i++)
        {
            for (int j=0;j<3;j++)
            {
                this->at(i,j)*=num;
            }
        }
    }
    
    void operator/=(const float& num)
    {
        float inv_num=1./num;
        for (int i=0;i<3;i++)
        {
            for (int j=0;j<3;j++)
            {
                this->at(i,j)*=inv_num;
            }
        }
    }
    
    void operator*=(const int& num)
    {
        float new_num= static_cast<float>(num);
        for (int i=0;i<3;i++)
        {
            for (int j=0;j<3;j++)
            {
                this->at(i,j)*=new_num;
            }
        }
    }
    
    void operator/=(const int& num)
    {
        float new_num= static_cast<float>(1./num);
        for (int i=0;i<3;i++)
        {
            for (int j=0;j<3;j++)
            {
                this->at(i,j)*=new_num;
            }
        }
    }
    
    bool operator==(const Matrix3f& matrix) const
    {
        for (int i=0;i<3;i++)
        {
            for (int j=0;j<3;j++)
            {
                if (matrix_value_[i*3+j]!=matrix.matrix_value_[i*3+j])
                {
                    return false;
                }
            }
        }
        return true;
    }
    
    bool operator!=(const Matrix3f& matrix) const
    {
        for (int i=0;i<3;i++)
        {
            for (int j=0;j<3;j++)
            {
                if (matrix_value_[i*3+j]!=matrix.matrix_value_[i*3+j])
                {
                    return true;
                }
            }
        }
        return false;
    }
    
    [[nodiscard]] Matrix3f rotate() const
    {
        Matrix3f result;
        for (int i=0;i<3;i++)
        {
            for (int j=0;j<3;j++)
            {
                result.at(i,j)=matrix_value_[j*3+i];
            }
        }
        return result;
    }

    [[nodiscard]] float determinant() const
    {
        return matrix_value_[0]*matrix_value_[4]*matrix_value_[8]+matrix_value_[1]*matrix_value_[5]*matrix_value_[6]+matrix_value_[2]*matrix_value_[3]*matrix_value_[7]-
            matrix_value_[2]*matrix_value_[4]*matrix_value_[6]-matrix_value_[1]*matrix_value_[3]*matrix_value_[8]-matrix_value_[0]*matrix_value_[5]*matrix_value_[7];
    }
    
    static Matrix3i identity()
    {
        return {1,0,0,
                0,1,0,
                0,0,1};
    }

    [[nodiscard]] Matrix3f inverse() const
    {
        const float det=determinant();
        if (det==0)
        {
            throw std::out_of_range("Inverse matrix requires its determinant is not zero");
        }
        Matrix3f result={
            matrix_value_[4]*matrix_value_[8]-matrix_value_[5]*matrix_value_[7],matrix_value_[2]*matrix_value_[7]-matrix_value_[1]*matrix_value_[8],matrix_value_[1]*matrix_value_[5]-matrix_value_[2]*matrix_value_[4],
            matrix_value_[5]*matrix_value_[6]-matrix_value_[3]*matrix_value_[8],matrix_value_[0]*matrix_value_[8]-matrix_value_[2]*matrix_value_[6],matrix_value_[2]*matrix_value_[3]-matrix_value_[0]*matrix_value_[5],
            matrix_value_[3]*matrix_value_[7]-matrix_value_[4]*matrix_value_[6],matrix_value_[1]*matrix_value_[6]-matrix_value_[0]*matrix_value_[7],matrix_value_[0]*matrix_value_[4]-matrix_value_[1]*matrix_value_[3]
        };
        
        return result/det;
    }
    
private:
    float matrix_value_[9];
};
