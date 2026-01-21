#pragma once

#include <cmath>

#include "Vector2i.h"
#include "Vector3f.h"

class Vector2f
{
public:
    Vector2f() 
    {
        x_=y_=0.0f;
    }
    
    Vector2f(const float x,const float y) 
    {
        x_=x;
        y_=y;
    }
    
    Vector2f(const Vector2f& v)
    {
        x_=v.x_;
        y_=v.y_;
    }
    
    Vector2f(Vector2i& v)
    {
        x_=static_cast<float>(v.x());
        y_=static_cast<float>(v.y());
    }
    
    Vector2f& operator=(const Vector2f& v)
    {
        if (this != &v)
        {
            x_=v.x_;
            y_=v.y_;
        }
        return *this;
    }
    
    Vector2f& operator=(Vector2i& v)
    {
        x_=static_cast<float>(v.x());
        y_=static_cast<float>(v.y());
        
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
    
    int operator[](int index)=delete;
    
    Vector2f operator+(const Vector2f& v) const
    {
        return {x_+v.x_,y_+v.y_};
    }
    
    void operator+=(const Vector2f& v)
    {
        x_+=v.x_;
        y_+=v.y_;
    }
    
    Vector2f operator-(const Vector2f& v) const
    {
        return {x_-v.x_,y_-v.y_};
    }
    
    void operator-()
    {
        x_=-x_;
        y_=-y_;
    }
    
    bool operator==(const Vector2f& v) const
    {
        return Mathf::FloatEqual(v.x_,x_) && Mathf::FloatEqual(v.y_,y_);
    }
    
    bool operator!=(const Vector2f& v) const
    {
        return !Mathf::FloatEqual(v.x_,x_) || !Mathf::FloatEqual(v.y_,y_);
    }
    
    void operator-=(const Vector2f& v)
    {
        x_-=v.x_;
        y_-=v.y_;
    }
    
    Vector2f operator*(const float multi) const
    {
        return {x_*multi,y_*multi};
    }
    
    Vector2f operator*(const int multi) const
    {
        float new_multi=static_cast<float>(multi);
        return {x_*new_multi,y_*new_multi};
    }
    
    friend Vector2f operator*(const float multi,const Vector2f& v)
    {
        return {v.x_*multi,v.y_*multi};
    }
    
    friend Vector2f operator*(const int multi,const Vector2f& v)
    {
        const float new_multi=static_cast<float>(multi);
        return {v.x_*new_multi,v.y_*new_multi};
    }
    
    Vector2f operator/(const float div) const
    {
        const float inv_div=1.0f/div;
        return {x_*inv_div,y_*inv_div};
    }
    
    Vector2f operator/(const int div) const
    {
        const float inv_div= static_cast<float>(1./div);
        return {x_*inv_div,y_*inv_div};
    }
    
    friend Vector2f operator/(const float div,const Vector2f& v)
    {
        const float inv_div=1.0f/div;
        return {v.x_*inv_div,v.y_*inv_div};
    }
    
    friend Vector2f operator/(const int div,const Vector2f& v)
    {
        const float inv_div= static_cast<float>(1./div);
        return {v.x_*inv_div,v.y_*inv_div};
    }
    
    void operator*=(const float multi)
    {
        x_*=multi;
        y_*=multi;
    }
    
    void operator*=(const int multi)
    {
        const float new_multi=static_cast<float>(multi);
        x_*=new_multi;
        y_*=new_multi;
    }
    
    void operator/=(const float div)
    {
        const float inv_div=1.0f/div;
        x_*=inv_div;
        y_*=inv_div;
    }
    
    void operator/=(const int div)
    {
        const float inv_div= static_cast<float>(1./div);
        x_*=inv_div;
        y_*=inv_div;
    }

    [[nodiscard]] float magnitude() const
    {
        return std::sqrt(x_*x_+y_*y_);
    }

    [[nodiscard]] float squaredMagnitude() const
    {
        return x_*x_+y_*y_;
    }
    
    [[nodiscard]] Vector2f normalized() const
    {
        const float inv_magnitude=1.0f/magnitude();
        return {x_*inv_magnitude,y_*inv_magnitude};
    }
    
    [[nodiscard]] float dot(const Vector2f& v) const
    {
        return x_*v.x_+y_*v.y_;
    }

    [[nodiscard]] Vector3f cross(const Vector2f& v) const
    {
        return {0,0,x_*v.y_-y_*v.x_};
    }

    [[nodiscard]] float crossMagnitude(const Vector2f& v) const
    {
        return x_*v.x_-y_*v.y_;
    }
    
private:
    float x_,y_;
};
