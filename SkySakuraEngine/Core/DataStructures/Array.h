#pragma once
#include <stdexcept>

template<class T>
class Array
{

public:
    Array()
    {
        size_=0;
        data_=nullptr;
    }
    Array(const int array_size)
    {
        if (array_size <= 0)
        {
            throw std::out_of_range("Array size must be greater than 0");
        }
        size_=array_size;
        data_=new T[size_];
    }
    
    Array(const std::initializer_list<T>& list)
    {
        if (list.size() <= 0)
        {
            throw std::out_of_range("Array size must be greater than 0");
        }
        if (list.size() >= static_cast<size_t>(1 << 31))
        {
            throw std::out_of_range("Array size must be less than 0x80000000");
        }
        size_=static_cast<int>(list.size());
        data_=new T[size_];
        const T* ptr=list.begin();
        for(int i=0;i<size_;i++)
        {
            data_[i]=*ptr;
            ++ptr;
        }
    }

    Array(const Array& array)
    {
        size_=array.size();
        data_=new T[size_];
        for(int i=0;i<size_;i++)
        {
            data_[i]=array.data_[i];
        }
    }
    
    
    Array& operator=(const Array& array)
    {
        if (this != &array)
        {
            delete[] data_;
            size_=array.size();
            data_=new T[size_];
            for(int i=0;i<size_;i++)
            {
                data_[i]=array.data_[i];
            }
        }
        return *this;
    }
    
    Array& operator=(const std::initializer_list<T>& list)
    {
        if (list.size() >= static_cast<size_t>(1 << 31))
        {
            throw std::out_of_range("Array size must be less than 0x80000000");
        }
        delete[] data_;
        size_=static_cast<int>(list.size());
        data_=new T[size_];
        const T* ptr=list.begin();
        for(int i=0;i<size_;i++)
        {
            data_[i]=*ptr;
            ++ptr;
        }
        return *this;
    }
    
    T& operator[](const int index)
    {
        if (index >= size_||index < 0)
        {
            throw std::out_of_range("Index out of range");
        }
        return data_[index];
    }
    
    T& operator[](const int index) const
    {
        if (index >= size_||index < 0)
        {
            throw std::out_of_range("Index out of range");
        }
        return data_[index];
    }
    
    int size() const
    {
        return size_;
    }
    
    T* begin()
    {
        return data_;
    }
    
    T* end()
    {
        return data_+size_;
    }
    
    ~Array()
    {
        delete[] data_;
    }
protected:
    T* data_;
    int size_;
};
