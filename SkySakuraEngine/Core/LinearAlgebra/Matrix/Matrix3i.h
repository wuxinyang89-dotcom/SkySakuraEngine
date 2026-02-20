#pragma once
#include <initializer_list>
#include <stdexcept>

#include "../Vector/Vector3i.h"

class Matrix3i
{
public:
    Matrix3i()
    {
        matrix_value_={0};
    }
    
    Matrix3i(const Matrix3i& matrix)
    {
        for (int i=0;i<9;i++)
        {
            matrix_value_[i]=matrix.matrix_value_[i];
        }
    }
    
    Matrix3i(const std::initializer_list<int>& list)
    {
        if (list.size()!=9)
        {
            throw std::out_of_range("Argument list must be of size 4");
        }
        const int* ptr=list.begin();
        for (int i=0; i<9; i++)
        {
            matrix_value_[i]=*(ptr+i);
        }
    }
    
    Matrix3i(const std::initializer_list<std::initializer_list<int>>& list)
    {
        if (list.size()!=3||list.begin()->size()!=3||
            (list.begin()+1)->size()!=3||(list.begin()+2)->size()!=3)
        {
            throw std::out_of_range("Argument list must be of size 4");
        }
        const std::initializer_list<int>* ptr=list.begin();
        for (int i=0; i<3; i++)
        {
            const int* ptr2=(ptr+i)->begin();
            for (int j=0; j<2; j++)
            {
                matrix_value_[i*3+j]=*(ptr2+j);
            }
        }
    }
    
    int& at(const int row,const int col)
    {
        if (row>=3||col>=3||row<0||col<0)
        {
            throw std::runtime_error("You cannot get a element that index is not in the matrix");
        }
        return matrix_value_[row*3+col];
    }
    
    Vector3i operator*(Vector3i& vector) const
    {
        Vector3i result;
        result.x()=matrix_value_[0]*vector.x()+matrix_value_[1]*vector.y()+matrix_value_[2]*vector.z();
        result.y()=matrix_value_[3]*vector.x()+matrix_value_[4]*vector.y()+matrix_value_[5]*vector.z();
        result.z()=matrix_value_[6]*vector.x()+matrix_value_[7]*vector.y()+matrix_value_[8]*vector.z();
        return result;
    }
    
    Matrix3i operator*(const Matrix3i& matrix) const
    {
        Matrix3i result;
        for (int i=0;i<3;i++)
        {
            for (int j=0;j<3;j++)
            {
                for (int k=0;k<3;k++)
                {
                    result.at(i,j)+=matrix_value_[i*3+k]*matrix.matrix_value_[k*3+j];
                }
            }
        }
        return result;
    }
    
    Matrix3i operator*(const int num)
    {
        Matrix3i result;
        for (int i=0;i<3;i++)
        {
            for (int j=0;j<3;j++)
            {
                result.at(i,j)=this->at(i,j)*num;
            }
        }
        return result;
    }
    
    Matrix3i operator/(const int num)
    {
        Matrix3i result;
        for (int i=0;i<3;i++)
        {
            for (int j=0;j<3;j++)
            {
                result.at(i,j)=this->at(i,j)/num;
            }
        }
        return result;
    }
    
    friend Matrix3i operator*(const int num,Matrix3i matrix)
    {
        Matrix3i result;
        for (int i=0;i<3;i++)
        {
            for (int j=0;j<3;j++)
            {
                result.at(i,j)=matrix.at(i,j)*num;
            }
        }
        return result;
    }
    
    friend Matrix3i operator/(const int num,Matrix3i matrix)
    {
        Matrix3i result;
        for (int i=0;i<3;i++)
        {
            for (int j=0;j<3;j++)
            {
                result.at(i,j)=matrix.at(i,j)/num;
            }
        }
        return result;
    }
    
    void operator*=(const int num)
    {
        for (int i=0;i<3;i++)
        {
            for (int j=0;j<3;j++)
            {
                matrix_value_[i*3+j]*=num;
            }
        }
    }
    
    void operator/=(const int num)
    {
        for (int i=0;i<3;i++)
        {
            for (int j=0;j<3;j++)
            {
                matrix_value_[i*3+j]/=num;
            }
        }
    }
    
    Vector3i multiply(Vector3i& vector) const
    {
        Vector3i result;
        result.x()=matrix_value_[0]*vector.x()+matrix_value_[1]*vector.y()+matrix_value_[2]*vector.z();
        result.y()=matrix_value_[3]*vector.x()+matrix_value_[4]*vector.y()+matrix_value_[5]*vector.z();
        result.z()=matrix_value_[6]*vector.x()+matrix_value_[7]*vector.y()+matrix_value_[8]*vector.z();
        return result;
    }
    
    Matrix3i multiply(const Matrix3i& matrix) const
    {
        Matrix3i result;
        for (int i=0;i<3;i++)
        {
            for (int j=0;j<3;j++)
            {
                for (int k=0;k<3;k++)
                {
                    result.at(i,j)+=matrix_value_[i*3+k]*matrix.matrix_value_[k*3+j];
                }
            }
        }
        return result;
    }
    
    bool operator==(const Matrix3i& matrix) const
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
    
    bool operator!=(const Matrix3i& matrix) const
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

    [[nodiscard]] Matrix3i rotate() const
    {
        Matrix3i result;
        for (int i=0;i<3;i++)
        {
            for (int j=0;j<3;j++)
            {
                result.at(i,j)=matrix_value_[j*3+i];
            }
        }
        return result;
    }

    [[nodiscard]] int determinant() const
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

    [[nodiscard]] Matrix3i inverse() const
    {
        const int det=determinant();
        if (det==0)
        {
            throw std::out_of_range("Inverse matrix requires its determinant is not zero");
        }
        Matrix3i result={
            matrix_value_[4]*matrix_value_[8]-matrix_value_[5]*matrix_value_[7],matrix_value_[2]*matrix_value_[7]-matrix_value_[1]*matrix_value_[8],matrix_value_[1]*matrix_value_[5]-matrix_value_[2]*matrix_value_[4],
            matrix_value_[5]*matrix_value_[6]-matrix_value_[3]*matrix_value_[8],matrix_value_[0]*matrix_value_[8]-matrix_value_[2]*matrix_value_[6],matrix_value_[2]*matrix_value_[3]-matrix_value_[0]*matrix_value_[5],
            matrix_value_[3]*matrix_value_[7]-matrix_value_[4]*matrix_value_[6],matrix_value_[1]*matrix_value_[6]-matrix_value_[0]*matrix_value_[7],matrix_value_[0]*matrix_value_[4]-matrix_value_[1]*matrix_value_[3]
        };
        
        return result/det;
    }
    
private:
    int matrix_value_[9];
};
