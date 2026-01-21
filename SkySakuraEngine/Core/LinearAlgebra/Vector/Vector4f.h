#pragma once
#include "Vector3f.h"
#include "Vector4i.h"

class Vector4f
{
public:
    Vector4f()
    {
        x_=y_=z_=w_=0;
    }
    
    Vector4f(const Vector4f& v)
    {
        x_=v.x_;
        y_=v.y_;
        z_=v.z_;
        w_=v.w_;
    }
    
    Vector4f(const float x,const float y,const float z=0,const float w=0)
    {
        x_=x;
        y_=y;
        z_=z;
        w_=w;
    }
    
    Vector4f(Vector4i& vi)
    {
        x_=static_cast<float>(vi.x());
        y_=static_cast<float>(vi.y());
        z_=static_cast<float>(vi.z());
        w_=static_cast<float>(vi.w());
    }
    
    Vector4f(Vector3f& v3,const int w)
    {
        x_=v3.x();
        y_=v3.y();
        z_=v3.z();
        w_= static_cast<float>(w);
    }
    
    Vector4f(Vector3i& v3_i,const int w)
    {
        x_=static_cast<float>(v3_i.x());
        y_=static_cast<float>(v3_i.y());
        z_=static_cast<float>(v3_i.z());
        w_= static_cast<float>(w);
    }
    
    Vector4f& operator=(const Vector4f& v)
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
    
    Vector4f& operator=(Vector4i& vi)
    {
        x_=static_cast<float>(vi.x());
        y_=static_cast<float>(vi.y());
        z_=static_cast<float>(vi.z());
        w_=static_cast<float>(vi.w());
        return *this;
    }
    
    Vector4f& operator=(Vector3f& v3,const int w)
    {
        x_=v3.x();
        y_=v3.y();
        z_=v3.z();
        w_= static_cast<float>(w);
        return *this;
    }
    
    Vector4f& operator=(Vector3i& v3_i,const int w)
    {
        x_=static_cast<float>(v3_i.x());
        y_=static_cast<float>(v3_i.y());
        z_=static_cast<float>(v3_i.z());
        w_= static_cast<float>(w);
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
    
    float& w()
    {
        return w_;
    }
    
    Vector4f operator+(const Vector4f& v) const
    {
        if (w_!=0&&v.w_!=0)
        {
            throw std::runtime_error("Make addition of two points is not supported!");
        }
        return {x_+v.x_,y_+v.y_,z_+v.z_,w_+v.w_};
    }
    
    Vector4f operator-(const Vector4f& v) const
    {
        if (w_==0&&v.w_!=0)
        {
            throw std::runtime_error("Make subtraction of a direction with a point is not supported!");
        }
        if (w_!=0&&v.w_!=0&&!Mathf::FloatEqual(v.w_,w_))
        {
            return {x_*v.w_-v.x_*w_,y_*v.w_-v.y_*w_,z_*v.w_-v.z_*w_,0};
        }
        return {x_-v.x_,y_-v.y_,z_-v.z_,w_-v.w_};
    }
    
    void operator+=(const Vector4f& v)
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
    
    void operator-=(const Vector4f& v)
    {
        if (w_==0&&v.w_!=0)
        {
            throw std::runtime_error("Make subtraction of a direction with a point is not supported!");
        }
        if (w_!=0&&v.w_!=0&&!Mathf::FloatEqual(v.w_,w_))
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
    
    bool operator==(const Vector4f& v) const
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
    
    bool operator!=(const Vector4f& v) const
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
    
    Vector4f operator*(const float num) const
    {
        return {x_*num,y_*num,z_*num,w_*num};
    }
    
    Vector4f operator*(const int num) const
    {
        float new_num = static_cast<float>(num);
        return {x_*new_num,y_*new_num,z_*new_num,w_*new_num};
    }
    
    Vector4f operator/(const float num) const
    {
        float inv_num = 1/num;
        return {x_*inv_num,y_*inv_num,z_*inv_num,w_*inv_num};
    }
    
    Vector4f operator/(const int num) const
    {
        float inv_num = static_cast<float>(1./num);
        return {x_*inv_num,y_*inv_num,z_*inv_num,w_*inv_num};
    }
    
    friend Vector4f operator*(const float num, const Vector4f& v)
    {
        return {v.x_*num,v.y_*num,v.z_*num,v.w_*num};
    }
    
    friend Vector4f operator*(const int num, const Vector4f& v)
    {
        float new_num = static_cast<float>(num);
        return {v.x_*new_num,v.y_*new_num,v.z_*new_num,v.w_*new_num};
    }
    
    friend Vector4f operator/(const float num, const Vector4f& v)
    {
        float inv_num = 1/num;
        return {v.x_*inv_num,v.y_*inv_num,v.z_*inv_num,v.w_*inv_num};
    }
    
    friend Vector4f operator/(const int num, const Vector4f& v)
    {
        float inv_num = static_cast<float>(1./num);
        return {v.x_*inv_num,v.y_*inv_num,v.z_*inv_num,v.w_*inv_num};
    }

    [[nodiscard]] float magnitude() const
    {
        if (w_!=0)
        {
            throw std::runtime_error("Calculate magnitude for points is not supported!");
        }
        return sqrt(x_*x_+y_*y_*z_*z_);
    }

    [[nodiscard]] float squaredMagnitude() const
    {
        if (w_!=0)
        {
            throw std::runtime_error("Calculate magnitude for points is not supported!");
        }
        return x_*x_+y_*y_*z_*z_;
    }

    [[nodiscard]] float dot(const Vector4f& v) const
    {
        if (w_!=0||v.w_!=0)
        {
            throw std::runtime_error("Make dot product for points is not supported!");
        }
        return x_*v.x_+y_*v.y_+z_*v.z_;
    }

    [[nodiscard]] Vector4f cross(const Vector4f& v) const
    {
        if (w_!=0||v.w_!=0)
        {
            throw std::runtime_error("Make cross product for points is not supported!");
        }
        return {y_*v.z_-z_*v.y_,x_*v.z_-z_*v.x_,0};
    }

    [[nodiscard]] Vector4f standardize() const
    {
        if (w_==0)
        {
            throw std::runtime_error("Standardizing a direction is not supported!");
        }
        float inv_w = 1/w_;
        return {x_*inv_w,y_*inv_w,z_*inv_w,1};
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
    float x_, y_, z_, w_;
};
