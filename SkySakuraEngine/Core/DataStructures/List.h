#pragma once
#include <stdexcept>
#include "Array.h"

template <typename T>
class List
{
public:
    List()
    {
        size_=0;
        total_size_=4;
        array_=new T[total_size_];
    }
    
    List<T>(const int size)
    {
        if (size < 0)
        {
            throw std::out_of_range("List size must be greater than 0");
        }
        size_=size;
        total_size_=calc_total_size(size);
        array_=new T[total_size_];
    }
    
    List<T>(const int size,const T value)
    {
        if (size < 0)
        {
            throw std::out_of_range("List size must be greater than 0");
        }
        size_=size;
        total_size_=calc_total_size(size);
        array_=new T[total_size_];
        for (int i=0;i<size_;i++)
        {
            array_[i]=value;
        }
    }
    
    List(std::initializer_list<T> list)
    {
        if (list.size() >= static_cast<size_t>(1 << 31))
        {
            throw std::out_of_range("Array size must be less than 0x80000000");
        }
        size_=list.size();
        total_size_=calc_total_size(size_);
        array_=new T[total_size_];
        const T* ptr=list.begin();
        for(int i=0;i<size_;i++)
        {
            array_[i]=*ptr;
            ++ptr;
        }
    }
    
    List<T>(const Array<T>& array)
    {
        size_=array.size();
        total_size_=calc_total_size(size_);
        array_=new T[total_size_];
        for (int i=0;i<size_;i++)
        {
            array_[i]=array[i];
        }
    }
    
    List<T>(const List<T>& list)
    {
        size_=list.size_;
        total_size_=list.total_size_;
        array_=new T[total_size_];
        for (int i=0;i<size_;i++)
        {
            array_[i]=list.array_[i];
        }
    }
    
    List& operator=(const List& list)
    {
        if (this != &list)
        {
            delete [] array_;
            size_=list.size_;
            total_size_=list.total_size_;
            array_=new T[total_size_];
            for (int i=0;i<size_;i++)
            {
                array_[i]=list.array_[i];
            }
        }
        return *this;
    }
    
    List& operator=(const std::initializer_list<T>& list)
    {
        if (list.size() >= static_cast<size_t>(1 << 31))
        {
            throw std::out_of_range("Array size must be less than 0x80000000");
        }
        delete [] array_;
        size_=list.size();
        total_size_=calc_total_size(size_);
        array_=new T[total_size_];
        const T* ptr=list.begin();
        for(int i=0;i<size_;i++)
        {
            array_[i]=*ptr;
            ++ptr;
        }
        return *this;
    }
    
    T& operator[](int index)
    {
        if (index >= size_||index < 0)
        {
            throw std::out_of_range("Index out of range");
        }
        return array_[index];
    }
    
    T& operator[](int index) const
    {
        if (index >= size_||index < 0)
        {
            throw std::out_of_range("Index out of range");
        }
        return array_[index];
    }
    
    void push_back(const T& value)
    {
        if (size_ == total_size_)
        {
            if (total_size_==~(1<<31))
            {
                throw std::out_of_range("List overflow");
            }
            if (total_size_==1<<30)
            {
                total_size_=~(1<<31);
            }
            else
            {
                total_size_<<=1;
            }
            T* temp=new T[total_size_];
            for (int i=0;i<size_;i++)
            {
                temp[i]=array_[i];
            }
            delete [] array_;
            array_=temp;
        }
        array_[size_]=value;
        size_++;
    }
    
    void pop_back()
    {
        if (size_ == 0)
        {
            throw std::out_of_range("List empty");
        }
        size_--;
    }
    
    void insert(int index, const T& value)
    {
        if (index >= size_||index < 0)
        {
            throw std::out_of_range("Index out of range");
        }
        if (size_ == total_size_)
        {
            if (total_size_==~(1<<31))
            {
                throw std::out_of_range("List overflow");
            }
            if (total_size_==1<<30)
            {
                total_size_=~(1<<31);
            }
            else
            {
                total_size_<<=1;
            }
            T* temp=new T[total_size_];
            for (int i=0;i<index;i++)
            {
                temp[i]=array_[i];
            }
            for (int i=size_-1;i>=index;i--)
            {
                temp[i+1]=array_[i];
            }
            delete [] array_;
            array_=temp;
            array_[index]=value;
            size_++;
            return;
        }
        for (int i=size_-1;i>=index;i--)
        {
            array_[i+1]=array_[i];
        }
        array_[index]=value;
        size_++;
    }
    
    void erase(int index)
    {
        if (index >= size_||index < 0)
        {
            throw std::out_of_range("Index out of range");
        }
        for (int i=index+1;i<size_;i++)
        {
            array_[i-1]=array_[i];
        }
        size_--;
    }
    
    void clear()
    {
        size_=0;
    }
    
    bool empty() const
    {
        return size_==0;
    }
    
    T* begin()
    {
        return array_;
    }
    
    T* end()
    {
        return array_+size_;
    }
    
    ~List<T>()
    {
        delete [] array_;
    }
    
private:
    int total_size_;
    int size_;
    T* array_;
    
    static int calc_total_size(const int size)
    {
        /*if (size == 0)
        {
            return 1;
        }
        int cmp=1<<31;
        int ans=32;
        while ((size&cmp)==0)
        {
            size<<=1;
            ans--;
        }
        if (ans==32)
        {
            throw std::out_of_range("List overflow");
        }
        if (ans==31)
        {
            return ~(1<<31);
        }
        return 1<<ans;*/
        if (size >= (1<<31))
        {
            throw std::out_of_range("List overflow");
        }
        unsigned int ans=1;
        while (ans<size)
        {
            ans<<=1;
        }
        if (ans==1<<31)
        {
            ans=~(1<<31);
        }
        return ans;
    }
};
