#pragma once
#include <cmath>
#include <initializer_list>
#include <stdexcept>

#include "Matrix4i.h"
#include "../Vector/Vector3f.h"
#include "../Vector/Vector4f.h"
#include "../../MathFunc/Mathf.h"

class Matrix4f
{
public:
    Matrix4f()
    {
        for (int i=0;i<16;i++)
        {
            matrix_value_[i]=0;
        }
    }
    
    Matrix4f(const Matrix4f& m)
    {
        for (int i=0;i<16;i++)
        {
            matrix_value_[i]=m.matrix_value_[i];
        }
    }
    
    Matrix4f(const std::initializer_list<float>& list)
    {
        if (list.size()!=16)
        {
            throw std::out_of_range("Argument list must be of size 16");
        }
        const float* ptr=list.begin();
        for (int i=0;i<16;i++)
        {
            matrix_value_[i]=*(ptr+i);
        }
    }
    
    Matrix4f(const std::initializer_list<std::initializer_list<float>>& list)
    {
        if (list.size()!=4)
        {
            throw std::out_of_range("Argument list must be of size 4");
        }
        const std::initializer_list<float>* ptr=list.begin();
        for (int i=0;i<4;i++)
        {
            if ((ptr+i)->size()!=4)
            {
                throw std::out_of_range("Argument list must be of size 4");
            }
            const float* ptr2=(ptr+i)->begin();
            for (int j=0;j<4;j++)
            {
                matrix_value_[i*4+j]=*(ptr2+j);
            }
        }
    }
    
    // 从整型矩阵构造
    Matrix4f(Matrix4i& mi)
    {
        for (int i=0;i<4;i++)
        {
            for (int j=0;j<4;j++)
            {
                matrix_value_[i*4+j]=static_cast<float>(mi.at(i,j));
            }
        }
    }
    
    Matrix4f& operator=(const Matrix4f& m)
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
    
    // 从整型矩阵赋值
    Matrix4f& operator=(Matrix4i& mi)
    {
        for (int i=0;i<4;i++)
        {
            for (int j=0;j<4;j++)
            {
                matrix_value_[i*4+j]=static_cast<float>(mi.at(i,j));
            }
        }
        return *this;
    }
    
    float& at(const int row,const int col)
    {
        if (row<0||row>=4||col<0||col>=4)
        {
            throw std::out_of_range("Index out of range");
        }
        return matrix_value_[row*4+col];
    }
    
    Matrix4f operator*(const Matrix4f& m) const
    {
        Matrix4f result;
        for (int i=0;i<4;i++)
        {
            for (int j=0;j<4;j++)
            {
                float sum=0;
                for (int k=0;k<4;k++)
                {
                    sum+=matrix_value_[i*4+k]*m.matrix_value_[k*4+j];
                }
                result.matrix_value_[i*4+j]=sum;
            }
        }
        return result;
    }
    
    Vector4f operator*(const Vector4f& v) const
    {
        Vector4f temp=v;
        Vector4f result;
        result.x()=matrix_value_[0]*temp.x()+matrix_value_[1]*temp.y()+matrix_value_[2]*temp.z()+matrix_value_[3]*temp.w();
        result.y()=matrix_value_[4]*temp.x()+matrix_value_[5]*temp.y()+matrix_value_[6]*temp.z()+matrix_value_[7]*temp.w();
        result.z()=matrix_value_[8]*temp.x()+matrix_value_[9]*temp.y()+matrix_value_[10]*temp.z()+matrix_value_[11]*temp.w();
        result.w()=matrix_value_[12]*temp.x()+matrix_value_[13]*temp.y()+matrix_value_[14]*temp.z()+matrix_value_[15]*temp.w();
        return result;
    }
    
    Matrix4f multiply(const Matrix4f& m) const
    {
        return (*this)*m;
    }
    
    Vector4f multiply(const Vector4f& v) const
    {
        return (*this)*v;
    }
    
    Matrix4f operator*(const float& num) const
    {
        Matrix4f result;
        for (int i=0;i<16;i++)
        {
            result.matrix_value_[i]=matrix_value_[i]*num;
        }
        return result;
    }
    
    Matrix4f operator/(const float& num) const
    {
        Matrix4f result;
        for (int i=0;i<16;i++)
        {
            result.matrix_value_[i]=matrix_value_[i]/num;
        }
        return result;
    }
    
    void operator*=(const float& num)
    {
        for (int i=0;i<16;i++)
        {
            matrix_value_[i]*=num;
        }
    }
    
    void operator/=(const float& num)
    {
        for (int i=0;i<16;i++)
        {
            matrix_value_[i]/=num;
        }
    }
    
    bool operator==(const Matrix4f& m) const
    {
        for (int i=0;i<16;i++)
        {
            if (!Mathf::FloatEqual(matrix_value_[i],m.matrix_value_[i]))
            {
                return false;
            }
        }
        return true;
    }
    
    bool operator!=(const Matrix4f& m) const
    {
        for (int i=0;i<16;i++)
        {
            if (!Mathf::FloatEqual(matrix_value_[i],m.matrix_value_[i]))
            {
                return true;
            }
        }
        return false;
    }
    
    Matrix4f transpose() const
    {
        Matrix4f result;
        for (int i=0;i<4;i++)
        {
            for (int j=0;j<4;j++)
            {
                result.matrix_value_[j*4+i]=matrix_value_[i*4+j];
            }
        }
        return result;
    }
    
    float determinant() const
    {
        float det=0;
        for (int j=0;j<4;j++)
        {
            const float sign=(j%2==0)?1.0f:-1.0f;
            det+=sign*matrix_value_[j]*minor_det(0,j);
        }
        return det;
    }
    
    static Matrix4f identity()
    {
        Matrix4f result;
        result.matrix_value_[0]=1;
        result.matrix_value_[5]=1;
        result.matrix_value_[10]=1;
        result.matrix_value_[15]=1;
        return result;
    }
    
    Matrix4f inverse() const
    {
        const float det=determinant();
        if (Mathf::FloatEqual(det,0))
        {
            throw std::runtime_error("Matrix is not invertible");
        }
        const float inv_det=1.0f/det;
        Matrix4f result;
        for (int i=0;i<4;i++)
        {
            for (int j=0;j<4;j++)
            {
                const float sign=((i+j)%2==0)?1.0f:-1.0f;
                // 逆矩阵 = 伴随矩阵 / 行列式，伴随矩阵是余子式矩阵的转置，故此处行列互换
                result.matrix_value_[j*4+i]=sign*minor_det(i,j)*inv_det;
            }
        }
        return result;
    }
    
    // ---------------- 常用变换矩阵（均基于列向量右乘，即 M*v） ----------------
    
    // 平移矩阵
    static Matrix4f translate(const float tx,const float ty,const float tz)
    {
        Matrix4f result=identity();
        result.matrix_value_[3]=tx;
        result.matrix_value_[7]=ty;
        result.matrix_value_[11]=tz;
        return result;
    }
    
    static Matrix4f translate(Vector3f v)
    {
        return translate(v.x(),v.y(),v.z());
    }
    
    // 缩放矩阵
    static Matrix4f scale(const float sx,const float sy,const float sz)
    {
        Matrix4f result=identity();
        result.matrix_value_[0]=sx;
        result.matrix_value_[5]=sy;
        result.matrix_value_[10]=sz;
        return result;
    }
    
    static Matrix4f scale(Vector3f v)
    {
        return scale(v.x(),v.y(),v.z());
    }
    
    // 绕 X 轴旋转（弧度）
    static Matrix4f rotateX(const float radians)
    {
        Matrix4f result=identity();
        const float c=std::cos(radians);
        const float s=std::sin(radians);
        result.matrix_value_[5]=c;
        result.matrix_value_[6]=-s;
        result.matrix_value_[9]=s;
        result.matrix_value_[10]=c;
        return result;
    }
    
    // 绕 Y 轴旋转（弧度）
    static Matrix4f rotateY(const float radians)
    {
        Matrix4f result=identity();
        const float c=std::cos(radians);
        const float s=std::sin(radians);
        result.matrix_value_[0]=c;
        result.matrix_value_[2]=s;
        result.matrix_value_[8]=-s;
        result.matrix_value_[10]=c;
        return result;
    }
    
    // 绕 Z 轴旋转（弧度）
    static Matrix4f rotateZ(const float radians)
    {
        Matrix4f result=identity();
        const float c=std::cos(radians);
        const float s=std::sin(radians);
        result.matrix_value_[0]=c;
        result.matrix_value_[1]=-s;
        result.matrix_value_[4]=s;
        result.matrix_value_[5]=c;
        return result;
    }
    
    // 绕任意轴旋转（罗德里格斯公式，弧度）
    static Matrix4f rotate(Vector3f axis,const float radians)
    {
        Vector3f n=axis.normalized();
        const float x=n.x();
        const float y=n.y();
        const float z=n.z();
        const float c=std::cos(radians);
        const float s=std::sin(radians);
        const float t=1-c;
        Matrix4f result=identity();
        result.matrix_value_[0]=t*x*x+c;
        result.matrix_value_[1]=t*x*y-s*z;
        result.matrix_value_[2]=t*x*z+s*y;
        result.matrix_value_[4]=t*x*y+s*z;
        result.matrix_value_[5]=t*y*y+c;
        result.matrix_value_[6]=t*y*z-s*x;
        result.matrix_value_[8]=t*x*z-s*y;
        result.matrix_value_[9]=t*y*z+s*x;
        result.matrix_value_[10]=t*z*z+c;
        return result;
    }
    
    // 透视投影矩阵（fov 为垂直视场角，单位弧度；aspect 为宽高比）
    static Matrix4f perspective(const float fov,const float aspect,const float z_near,const float z_far)
    {
        const float f=1.0f/std::tan(fov/2);
        Matrix4f result;
        result.matrix_value_[0]=f/aspect;
        result.matrix_value_[5]=f;
        result.matrix_value_[10]=(z_far+z_near)/(z_near-z_far);
        result.matrix_value_[11]=(2*z_far*z_near)/(z_near-z_far);
        result.matrix_value_[14]=-1;
        return result;
    }
    
    // 正交投影矩阵
    static Matrix4f ortho(const float left,const float right,const float bottom,const float top,const float z_near,const float z_far)
    {
        Matrix4f result=identity();
        result.matrix_value_[0]=2.0f/(right-left);
        result.matrix_value_[5]=2.0f/(top-bottom);
        result.matrix_value_[10]=-2.0f/(z_far-z_near);
        result.matrix_value_[3]=-(right+left)/(right-left);
        result.matrix_value_[7]=-(top+bottom)/(top-bottom);
        result.matrix_value_[11]=-(z_far+z_near)/(z_far-z_near);
        return result;
    }
    
    // 视图矩阵（右手坐标系，eye 为相机位置，center 为目标点，up 为上方向）
    static Matrix4f lookAt(Vector3f eye,Vector3f center,Vector3f up)
    {
        Vector3f f=(center-eye).normalized();
        Vector3f s=f.cross(up).normalized();
        Vector3f u=s.cross(f);
        Matrix4f result=identity();
        result.matrix_value_[0]=s.x();
        result.matrix_value_[1]=s.y();
        result.matrix_value_[2]=s.z();
        result.matrix_value_[3]=-s.dot(eye);
        result.matrix_value_[4]=u.x();
        result.matrix_value_[5]=u.y();
        result.matrix_value_[6]=u.z();
        result.matrix_value_[7]=-u.dot(eye);
        result.matrix_value_[8]=-f.x();
        result.matrix_value_[9]=-f.y();
        result.matrix_value_[10]=-f.z();
        result.matrix_value_[11]=f.dot(eye);
        return result;
    }
private:
    float matrix_value_[16];
    
    // 计算划去 row 行 col 列后剩余 3 阶子矩阵的行列式（余子式）
    float minor_det(const int row,const int col) const
    {
        float sub[9];
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
