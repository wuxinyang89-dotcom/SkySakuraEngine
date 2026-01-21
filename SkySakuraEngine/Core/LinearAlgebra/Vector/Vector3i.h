#pragma once

class Vector3i
{
public:
    Vector3i()
    {
        x_=y_=z_=0;
    }
    
    Vector3i(const int x, const int y, const int z=0)
    {
        x_=x;
        y_=y;
        z_=z;
    }
    
    Vector3i(const Vector3i& v)
    {
        x_=v.x_;
        y_=v.y_;
        z_=v.z_;
    }
    
    Vector3i& operator=(const Vector3i& v)
    {
        if (this==&v) return *this;
        x_=v.x_;
        y_=v.y_;
        z_=v.z_;
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
    
    int operator[](int index)=delete;
    
    Vector3i operator+(const Vector3i& v) const
    {
        return {x_+v.x_,y_+v.y_,z_+v.z_};
    }
    
    void operator+=(const Vector3i& v)
    {
        x_+=v.x_;
        y_+=v.y_;
        z_+=v.z_;
    }
    
    Vector3i operator-(const Vector3i& v) const
    {
        return {x_-v.x_,y_-v.y_,z_-v.z_};
    }
    
    void operator-()
    {
        x_=-x_;
        y_=-y_;
        z_=-z_;
    }
    
    void operator-=(const Vector3i& v)
    {
        x_-=v.x_;
        y_-=v.y_;
        z_-=v.z_;
    }
    
    bool operator==(const Vector3i& v) const
    {
        return v.x_==x_ && v.y_==y_&&v.z_==z_;
    }
    
    bool operator!=(const Vector3i& v) const
    {
        return v.x_!=x_ || v.y_!=y_|| v.z_!=z_;
    }
    
    Vector3i operator*(const int num) const
    {
        return {x_*num,y_*num,z_*num};
    }
    
    friend Vector3i operator*(const int num, const Vector3i& v)
    {
        return {v.x_*num,v.y_*num,v.z_*num};
    }
    
    void operator*=(const int num)
    {
        x_*=num;
        y_*=num;
        z_*=num;
    }
    
    Vector3i operator/(const int num) const
    {
        return {x_/num,y_/num,z_/num};
    }
    
    friend Vector3i operator/(const int num, const Vector3i& v)
    {
        return {v.x_/num,v.y_/num,v.z_/num};
    }
    
    void operator/=(const int num)
    {
        const int new_num=static_cast<int>(1./num);
        x_*=new_num;
        y_*=new_num;
        z_*=new_num;
    }
    
    [[nodiscard]] int squaredMagnitude() const
    {
        return x_*x_+y_*y_+z_*z_;
    }

    [[nodiscard]] int dot(const Vector3i& v) const
    {
        return x_*v.x_+y_*v.y_+z_*v.z_;
    }

    [[nodiscard]] Vector3i cross(const Vector3i& v) const
    {
        return {y_*v.z_-z_*v.y_,z_*v.x_-x_*v.z_,x_*v.y_-y_*v.x_};
    }
    
private:
    int x_,y_,z_;
};
