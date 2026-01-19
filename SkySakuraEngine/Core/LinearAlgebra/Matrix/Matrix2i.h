#pragma once
#include <initializer_list>
#include <stdexcept>

#include "../Vector/Vector2i.h"

class Matrix2i
{
public:
    Matrix2i()
    {
        matrix_value_={0};
    }
    
    Matrix2i(const Matrix2i& matrix)
    {
        for (int i=0; i<4; i++)
        {
            matrix_value_[i]=matrix.matrix_value_[i];
        }
    }
    
    Matrix2i(const std::initializer_list<int>& list)
    {
        if (list.size()!=4)
        {
            throw std::out_of_range("Argument list must be of size 4");
        }
        const int* ptr=list.begin();
        for (int i=0; i<4; i++)
        {
            matrix_value_[i]=*(ptr+i);
        }
    }
    
    Matrix2i(const std::initializer_list<std::initializer_list<int>>& list)
    {
        if (list.size()!=2||list.begin()->size()!=2||(list.begin()+1)->size()!=2)
        {
            throw std::out_of_range("Argument list must be of size 4");
        }
        const std::initializer_list<int>* ptr=list.begin();
        for (int i=0; i<2; i++)
        {
            const int* ptr2=(ptr+i)->begin();
            for (int j=0; j<2; j++)
            {
                matrix_value_[i*2+j]=*(ptr2+j);
            }
        }
    }
    
    Matrix2i& operator=(const Matrix2i& matrix)
    {
        if (this!=&matrix)
        {
            for (int i=0; i<4; i++)
            {
                matrix_value_[i]=matrix.matrix_value_[i];
            }
        }
        return *this;
    }
    
    Matrix2i& operator=(const std::initializer_list<int>& list)
    {
        if (list.size()!=4)
        {
            throw std::out_of_range("Argument list must be of size 4");
        }
        const int* ptr=list.begin();
        for (int i=0; i<4; i++)
        {
            matrix_value_[i]=*(ptr+i);
        }
        return *this;
    }
    
    Matrix2i& operator=(const std::initializer_list<std::initializer_list<int>>& list)
    {
        if (list.size()!=2||list.begin()->size()!=2||(list.begin()+1)->size()!=2)
        {
            throw std::out_of_range("Argument list must be of size 4");
        }
        const std::initializer_list<int>* ptr=list.begin();
        for (int i=0; i<2; i++)
        {
            const int* ptr2=(ptr+i)->begin();
            for (int j=0; j<2; j++)
            {
                matrix_value_[i*2+j]=*(ptr2+j);
            }
        }
        return *this;
    }
    
    int& at(const int row,const int col)
    {
        if (row<0||row>2||col<0||col>2)
        {
            throw std::out_of_range("Argument matrix must be of size 2");
        }
        return matrix_value_[row*2+col];
    }

    [[nodiscard]] Matrix2i rotate() const
    {
        return Matrix2i({
            matrix_value_[0],matrix_value_[2],
            matrix_value_[1],matrix_value_[3]
        });
    }
    
    Vector2i operator*(const Vector2i& v) const
    {
        return {
            matrix_value_[0]*v.x()+matrix_value_[1]*v.y(),
            matrix_value_[2]*v.x()+matrix_value_[3]*v.y()
        };
    }
    
    Matrix2i operator*(const Matrix2i& matrix) const
    {
        return Matrix2i({
            matrix_value_[0]*matrix.matrix_value_[0]+matrix_value_[1]*matrix.matrix_value_[2],
            matrix_value_[0]*matrix.matrix_value_[1]+matrix_value_[1]*matrix.matrix_value_[3],
            matrix_value_[2]*matrix.matrix_value_[0]+matrix_value_[3]*matrix.matrix_value_[2],
            matrix_value_[2]*matrix.matrix_value_[1]+matrix_value_[3]*matrix.matrix_value_[0],
        });
    }

    [[nodiscard]] int det() const
    {
        return matrix_value_[0]*matrix_value_[3]-matrix_value_[1]*matrix_value_[2];
    }
private:
    int matrix_value_[4];
};
