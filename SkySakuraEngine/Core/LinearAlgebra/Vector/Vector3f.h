#pragma once
#include <cmath>

#include "Vector.h"
#include "Vector3i.h"
#include "../../MathFunc/Mathf.h"

class Vector3f
{
public:
    Vector3f()
    {
        x_=y_=z_=0.0f;
    }
    
    Vector3f(const float x, const float y, const float z=0.0f)
    {
        x_=x;
        y_=y;
        z_=z;
    }
    
    Vector3f(const Vector3f& v)
    {
        x_=v.x_;
        y_=v.y_;
        z_=v.z_;
    }
    
    Vector3f(Vector3i& v)
    {
        x_= static_cast<float>(v.x());
        y_= static_cast<float>(v.y());
        z_= static_cast<float>(v.z());
        
    }
    
    Vector3f& operator=(const Vector3f& v)
    {
        if (this != &v)
        {
            x_=v.x_;
            y_=v.y_;
            z_=v.z_;
        }
        return *this;
    }
    
    Vector3f& operator=(Vector3i& v)
    {
        x_= static_cast<float>(v.x());
        y_= static_cast<float>(v.y());
        z_= static_cast<float>(v.z());
        return *this;
    }
    
    float& x()
    {
        return x_;
    }
    
    float& y()
    {
        return y_;
    }
    
    float& z()
    {
        return z_;
    }
    
    Vector3f operator+(const Vector3f& v) const
    {
        return {x_+v.x_,y_+v.y_,z_+v.z_};
    }
    
    void operator+=(const Vector3f& v)
    {
        x_+=v.x_;
        y_+=v.y_;
        z_+=v.z_;
    }
    
    Vector3f operator-(const Vector3f& v) const
    {
        return {x_-v.x_,y_-v.y_,z_-v.z_};
    }
    
    void operator-()
    {
        x_=-x_;
        y_=-y_;
        z_=-z_;
    }
    
    void operator-=(const Vector3f& v)
    {
        x_-=v.x_;
        y_-=v.y_;
        z_-=v.z_;
    }
    
    bool operator==(const Vector3f& v) const
    {
        return Mathf::FloatEqual(v.x_,x_) && Mathf::FloatEqual(v.y_,y_)&&Mathf::FloatEqual(v.z_,z_);
    }
    
    bool operator!=(const Vector3f& v) const
    {
        return !Mathf::FloatEqual(v.x_,x_) || !Mathf::FloatEqual(v.y_,y_)|| !Mathf::FloatEqual(v.z_,z_);
    }
    
    Vector3f operator*(const int num) const
    {
        float new_num=static_cast<float>(num);
        return {x_*new_num,y_*new_num,z_*new_num};
    }
    
    Vector3f operator*(const float num) const
    {
        return {x_*num,y_*num,z_*num};
    }
    
    friend Vector3f operator*(const int num, const Vector3f& v)
    {
        float new_num=static_cast<float>(num);
        return {v.x_*new_num,v.y_*new_num,v.z_*new_num};
    }
    
    friend Vector3f operator*(const float num, const Vector3f& v)
    {
        return {v.x_*num,v.y_*num,v.z_*num};
    }
    
    void operator*=(const int num)
    {
        float new_num=static_cast<float>(num);
        x_*=new_num;
        y_*=new_num;
        z_*=new_num;
    }
    
    void operator*=(const float num)
    {
        x_*=num;
        y_*=num;
        z_*=num;
    }
    
    Vector3f operator/(const int num) const
    {
        float new_num=static_cast<float>(1./num);
        return {x_*new_num,y_*new_num,z_*new_num};
    }
    
    Vector3f operator/(const float num) const
    {
        float new_num=1/num;
        return {x_*new_num,y_*new_num,z_*new_num};
    }
    
    friend Vector3f operator/(const int num, const Vector3f& v)
    {
        float new_num=static_cast<float>(1./num);
        return {v.x_*new_num,v.y_*new_num,v.z_*new_num};
    }
    
    friend Vector3f operator/(const float num, const Vector3f& v)
    {
        float new_num=1/num;
        return {v.x_*new_num,v.y_*new_num,v.z_*new_num};
    }
    
    void operator/=(const int num)
    {
        const float new_num=static_cast<float>(1./num);
        x_*=new_num;
        y_*=new_num;
        z_*=new_num;
    }
    
    void operator/=(const float num)
    {
        const float new_num=1/num;
        x_*=new_num;
        y_*=new_num;
        z_*=new_num;
    }

    [[nodiscard]] float magnitude() const
    {
        return std::sqrt(x_*x_+y_*y_+z_*z_);
    }

    [[nodiscard]] float squaredMagnitude() const
    {
        return x_*x_+y_*y_+z_*z_;
    }

    [[nodiscard]] Vector3f normalized() const
    {
        float inv_magnitude=1.0f/magnitude();
        return {x_*inv_magnitude,y_*inv_magnitude,z_*inv_magnitude};
    }

    [[nodiscard]] float dot(const Vector3f& v) const
    {
        return x_*v.x_+y_*v.y_+z_*v.z_;
    }

    [[nodiscard]] Vector3f cross(const Vector3f& v) const
    {
        return {y_*v.z_-z_*v.y_,z_*v.x_-x_*v.z_,x_*v.y_-y_*v.x_};
    }

private:
    float x_,y_,z_;
};
