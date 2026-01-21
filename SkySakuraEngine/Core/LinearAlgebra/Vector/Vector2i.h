#pragma once
#include <stdexcept>

#include "Vector3i.h"

class Vector2i
{
public:
    Vector2i()
    {
        x_=0;
        y_=0;
    }
    Vector2i(int x, int y)
    {
        x_=x;
        y_=y;
    }
    
    int& x()
    {
        return x_;
    }
    
    int& y()
    {
        return y_;
    }
    
    int operator[](int index)=delete;
    
    Vector2i operator+(const Vector2i& v) const
    {
        return {x_+v.x_,y_+v.y_};
    }
    
    Vector2i operator-(const Vector2i& v) const
    {
        return {x_-v.x_,y_-v.y_};
    }
    
    bool operator==(const Vector2i& v) const
    {
        return x_==v.x_ && y_==v.y_;
    }
    
    bool operator!=(const Vector2i& v) const
    {
        return x_!=v.x_ || y_!=v.y_;
    }
    
    void operator+=(const Vector2i& v)
    {
        x_+=v.x_;
        y_+=v.y_;
    }
    
    void operator-=(const Vector2i& v)
    {
        x_-=v.x_;
        y_-=v.y_;
    }
    
    Vector2i operator*(const int mult) const
    {
        return {x_*mult,y_*mult};
    }
    
    friend Vector2i operator*(const int mult, const Vector2i& v)
    {
        return {mult*v.x_,mult*v.y_};
    }
    
    void operator*=(const int mult)
    {
        x_*=mult;
        y_*=mult;
    }
    
    Vector2i operator/(const int divide) const
    {
        return {x_/divide,y_/divide};
    }
    
    friend Vector2i operator/(const int divide, const Vector2i& v)
    {
        return {v.x_/divide,v.y_/divide};
    }
    
    void operator/=(const int divide)
    {
        x_/=divide;
        y_/=divide;
    }

    [[nodiscard]] int squaredMagnitude() const
    {
        return x_*x_+y_*y_;
    }

    [[nodiscard]] int dot(const Vector2i& v) const
    {
        return x_*v.x_+y_*v.y_;
    }

    [[nodiscard]] Vector3i cross(const Vector2i& v) const
    {
        return {0,0,x_*v.y_-y_*v.x_};
    }

    [[nodiscard]] int crossMagnitude(const Vector2i& v) const
    {
        return x_*v.y_-y_*v.x_;
    }
    
private:
    int x_, y_;
};
