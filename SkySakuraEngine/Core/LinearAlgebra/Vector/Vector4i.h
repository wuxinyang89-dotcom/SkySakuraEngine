#pragma once
#include <stdexcept>

#include "Vector3i.h"

class Vector4i
{
public:
    Vector4i()
    {
        x_=y_=z_=w_=0;
    }
    
    Vector4i(const Vector4i& v)
    {
        x_=v.x_;
        y_=v.y_;
        z_=v.z_;
        w_=v.w_;
    }
    
    Vector4i(const int x,const int y,const int z=0,const int w=0)
    {
        x_=x;
        y_=y;
        z_=z;
        w_=w;
    }
    
    Vector4i(Vector3i& v,const int w)
    {
        x_=v.x();
        y_=v.y();
        z_=v.z();
        w_=w;
    }
    
    Vector4i& operator=(const Vector4i& v)
    {
        if (this!=&v)
        {
            x_=v.x_;
            y_=v.y_;
            z_=v.z_;
            w_=v.w_;
        }
        return *this;
    }
    
    Vector4i& operator=(Vector3i& v,const int w)
    {
        x_=v.x();
        y_=v.y();
        z_=v.z();
        w_=w;
        return *this;
    }
    
    int& x()
    {
        return x_;
    }
    
    int& y()
    {
        return y_;
    }
    
    int& z()
    {
        return z_;
    }
    
    int& w()
    {
        return w_;
    }
    
    int operator[](int index)=delete;
    
    Vector4i operator+(const Vector4i& v) const
    {
        if (w_!=0&&v.w_!=0)
        {
            throw std::runtime_error("Make addition of two points is not supported!");
        }
        return {x_+v.x_,y_+v.y_,z_+v.z_,w_+v.w_};
    }
    
    Vector4i operator-(const Vector4i& v) const
    {
        if (w_==0&&v.w_!=0)
        {
            throw std::runtime_error("Make subtraction of a direction with a point is not supported!");
        }
        if (w_!=0&&v.w_!=0&&w_!=v.w_)
        {
            return {x_*v.w_-v.x_*w_,y_*v.w_-v.y_*w_,z_*v.w_-v.z_*w_,0};
        }
        return {x_-v.x_,y_-v.y_,z_-v.z_,w_-v.w_};
    }
    
    void operator+=(const Vector4i& v)
    {
        if (w_!=0&&v.w_!=0)
        {
            throw std::runtime_error("Make addition of two points is not supported!");
        }
        x_+=v.x_;
        y_+=v.y_;
        z_+=v.z_;
        w_+=v.w_;
    }
    
    void operator-=(const Vector4i& v)
    {
        if (w_==0&&v.w_!=0)
        {
            throw std::runtime_error("Make subtraction of a direction with a point is not supported!");
        }
        if (w_!=0&&v.w_!=0&&w_!=v.w_)
        {
            x_=x_*v.w_-v.x_*w_;
            y_=y_*v.w_-v.y_*w_;
            z_=z_*v.w_-v.z_*w_;
            w_=0;
        }
        x_-=v.x_;
        y_-=v.y_;
        z_-=v.z_;
        w_=0;
    }
    
    void operator-()
    {
        x_=-x_;
        y_=-y_;
        z_=-z_;
        w_=-w_;
    }
    
    bool operator==(const Vector4i& v) const
    {
        if (w_==v.w_)
        {
            return x_==v.x_&&y_==v.y_&&z_==v.z_;
        }
        if (w_==0||v.w_==0)
        {
            return false;
        }
        return x_*v.w_==v.x_*w_&&y_*v.w_==v.y_*w_&&z_*v.w_==v.z_*w_;
    }
    
    bool operator!=(const Vector4i& v) const
    {
        if (w_==v.w_)
        {
            return x_!=v.x_||y_!=v.y_||z_!=v.z_;
        }
        if (w_==0||v.w_==0)
        {
            return true;
        }
        return x_*v.w_!=v.x_*w_||y_*v.w_!=v.y_*w_||z_*v.w_!=v.z_*w_;
    }
    
    Vector4i operator*(const int num) const
    {
        return {x_*num,y_*num,z_*num,w_*num};
    }
    
    Vector4i operator/(const int num) const
    {
        return {x_/num,y_/num,z_/num,w_/num};
    }
    
    friend Vector4i operator*(const int num,const Vector4i& v)
    {
        return {v.x_*num,v.y_*num,v.z_*num,v.w_*num};
    }
    
    friend Vector4i operator/(const int num,const Vector4i& v)
    {
        return {v.x_/num,v.y_/num,v.z_/num,v.w_/num};
    }
    
    void operator*=(const int num)
    {
        x_*=num;
        y_*=num;
        z_*=num;
        w_*=num;
    }
    
    void operator/=(const int num)
    {
        x_/=num;
        y_/=num;
        z_/=num;
        w_/=num;
    }

    [[nodiscard]] int squaredMagnitude() const
    {
        if (w_!=0)
        {
            throw std::runtime_error("Calculate magnitude for points is not supported!");
        }
        return x_*x_+y_*y_+z_*z_;
    }

    [[nodiscard]] int dot(const Vector4i& v) const
    {
        if (w_!=0||v.w_!=0)
        {
            throw std::runtime_error("Make dot product for points is not supported!");
        }
        return x_*v.x_+y_*v.y_+z_*v.z_;
    }

    [[nodiscard]] Vector4i cross(const Vector4i& v) const
    {
        if (w_!=0||v.w_!=0)
        {
            throw std::runtime_error("Make cross product for points is not supported!");
        }
        return {y_*v.z_-z_*v.y_,x_*v.z_-z_*v.x_,0};
    }

    [[nodiscard]] bool isPoint() const
    {
        return w_!=0;
    }

    [[nodiscard]] bool isDirection() const
    {
        return w_==0;
    }
    
private:
    int x_, y_, z_, w_;
};
