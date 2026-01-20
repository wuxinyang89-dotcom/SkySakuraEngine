#pragma once
#include "Matrix2i.h"
#include "../Vector/Vector2f.h"

class Matrix2f
{
public:
    Matrix2f()
    {
        matrix_value_={0};
    }
    
    Matrix2f(const Matrix2f& m)
    {
        for(int i=0;i<4;i++)
        {
            matrix_value_[i]=m.matrix_value_[i];
        }
    }
    
    Matrix2f(Matrix2i& mi)
    {
        for(int i=0;i<4;i++)
        {
            matrix_value_[i]=static_cast<float>(mi.at(i>>1,i&2));
        }
    }
    
    Matrix2f(const std::initializer_list<float>& list)
    {
        if(list.size()!=4)
        {
            throw std::out_of_range("Argument list must be of size 4");
        }
        const float* ptr=list.begin();
        for(int i=0;i<4;i++)
        {
            matrix_value_[i]=*(ptr+i);
        }
    }
    
    Matrix2f(const std::initializer_list<std::initializer_list<float>>& list)
    {
        if (list.size()!=2||list.begin()->size()!=2||(list.begin()+1)->size()!=2)
        {
            throw std::out_of_range("Argument list must be of size 4");
        }
        const std::initializer_list<float>* ptr=list.begin();
        for (int i=0; i<2; i++)
        {
            const float* ptr2=(ptr+i)->begin();
            for (int j=0; j<2; j++)
            {
                matrix_value_[i*2+j]=*(ptr2+j);
            }
        }
    }
    
    Matrix2f& operator=(const Matrix2f& m)
    {
        if (this!=&m)
        {
            for(int i=0;i<4;i++)
            {
                matrix_value_[i]=m.matrix_value_[i];
            }
        }
        return *this;
    }
    
    Matrix2f& operator=(const Matrix2i& mi)
    {
        for(int i=0;i<4;i++)
        {
            matrix_value_[i]=static_cast<float>(mi.at(i>>1,i&2));
        }
        return *this;
    }
    
    Matrix2f& operator=(const std::initializer_list<float>& list)
    {
        if(list.size()!=4)
        {
            throw std::out_of_range("Argument list must be of size 4");
        }
        const float* ptr=list.begin();
        for(int i=0;i<4;i++)
        {
            matrix_value_[i]=*(ptr+i);
        }
        return *this;
    }
    
    Matrix2f& operator=(const std::initializer_list<std::initializer_list<float>>& list)
    {
        if (list.size()!=2||list.begin()->size()!=2||(list.begin()+1)->size()!=2)
        {
            throw std::out_of_range("Argument list must be of size 4");
        }
        const std::initializer_list<float>* ptr=list.begin();
        for (int i=0; i<2; i++)
        {
            const float* ptr2=(ptr+i)->begin();
            for (int j=0; j<2; j++)
            {
                matrix_value_[i*2+j]=*(ptr2+j);
            }
        }
        return *this;
    }
    
    float& at(const int row,const int col)
    {
        return matrix_value_[row*2+col];
    }
    
    Vector2f operator*(Vector2f& v) const
    {
        return {
            matrix_value_[0]*v.x()+matrix_value_[1]*v.y(),
            matrix_value_[2]*v.x()+matrix_value_[3]*v.y()
        };
    }
    
    Matrix2f operator*(const Matrix2f& m) const
    {
        return {
            matrix_value_[0]*m.matrix_value_[0]+matrix_value_[1]*m.matrix_value_[2],
            matrix_value_[0]*m.matrix_value_[1]+matrix_value_[1]*m.matrix_value_[3],
            matrix_value_[2]*m.matrix_value_[0]+matrix_value_[3]*m.matrix_value_[2],
            matrix_value_[2]*m.matrix_value_[1]+matrix_value_[3]*m.matrix_value_[0],
        };
    }
    
    Vector2f multiply(Vector2f& v) const
    {
        return {
            matrix_value_[0]*v.x()+matrix_value_[1]*v.y(),
            matrix_value_[2]*v.x()+matrix_value_[3]*v.y()
        };
    }
    
    Matrix2f multiply(const Matrix2f& m) const
    {
        return {
            matrix_value_[0]*m.matrix_value_[0]+matrix_value_[1]*m.matrix_value_[2],
            matrix_value_[0]*m.matrix_value_[1]+matrix_value_[1]*m.matrix_value_[3],
            matrix_value_[2]*m.matrix_value_[0]+matrix_value_[3]*m.matrix_value_[2],
            matrix_value_[2]*m.matrix_value_[1]+matrix_value_[3]*m.matrix_value_[0],
        };
    }
    
    Matrix2f rotate() const
    {
        return {
            matrix_value_[0],matrix_value_[2],
            matrix_value_[1],matrix_value_[3]
        };
    }
    
    float determinant() const
    {
        return matrix_value_[0]*matrix_value_[3]-matrix_value_[1]*matrix_value_[2];
    }
    
    static Matrix2f identity()
    {
        return {
            1,0,0,1
        };
    }
    
    Matrix2f inverse() const
    {
        float det = determinant();
        if (det==0)
        {
            throw std::out_of_range("Inverse matrix requires its determinant is not zero");
        }
        return {
            matrix_value_[3]/det,-matrix_value_[2]/det,
            -matrix_value_[1]/det,matrix_value_[1]/det
        };
    }
    
private:
    float matrix_value_[4];
};
