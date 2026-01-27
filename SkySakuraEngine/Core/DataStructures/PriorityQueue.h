#pragma once
#include <stdexcept>

#include "../MathFunc/Mathf.h"
#include "Compare/Greater.h"

template<typename T,typename Compare=Greater<T>>
class PriorityQueue
{
public:
    explicit PriorityQueue(const Compare& comp=Compare())
    {
        comp_=comp;
        size_=0;
        total_size_=4;
        data_=new T[total_size_];
    }
    
    PriorityQueue(const PriorityQueue& other)
    {
        comp_=other.comp_;
        size_=other.size_;
        total_size_=other.total_size_;
        data_=new T[size_];
        for (int i=0;i<size_;i++)
        {
            data_[i]=other.data_[i];
        }
    }
    
    PriorityQueue& operator=(const PriorityQueue& other)
    {
        if (this!=&other)
        {
            comp_=other.comp_;
            if (total_size_<other.size_)
            {
                delete [] data_;
                size_=other.size_;
                total_size_=other.total_size_;
                data_=new T[total_size_];
            }
            else
            {
                size_=other.size_;
            }
            for (int i=0;i<size_;i++)
            {
                data_[i]=other.data_[i];
            }
        }
        return *this;
    }
    
    void push(const T& item)
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
                temp[i]=data_[i];
            }
            delete [] data_;
            data_=temp;
        }
        data_[size_]=item;
        size_++;
        int index=size_-1;
        while (index>0&&comp_(data_[index],data_[(index+1)/2-1]))
        {
            Mathf::swap(data_[index],data_[(index+1)/2-1]);
            index=(index+1)/2-1;
        }
        while ((index+1)*2<=size_)
        {
            if (comp_(data_[(index+1)*2-1],data_[index])&&
                (index+1)*2+1<=size_?comp_(data_[(index+1)*2-1],data_[(index+1)*2]):true)
            {
                Mathf::swap(data_[(index+1)*2-1],data_[index]);
                index=(index+1)*2-1;
            }
            else if ((index+1)*2+1<=size_&&comp_(data_[(index+1)*2],data_[index]))
            {
                Mathf::swap(data_[(index+1)*2],data_[index]);
                index=(index+1)*2;
            }
            else
            {
                break;
            }
        }
        
    }
    
    T pop()
    {
        if (size_==0)
        {
            throw std::out_of_range("Stack is empty");
        }
        if (size_==1)
        {
            size_--;
            return data_[0];
        }
        
        Mathf::swap(data_[0],data_[size_-1]);
        size_--;
        int index=0;
        while ((index+1)*2<=size_)
        {
            if (comp_(data_[(index+1)*2-1],data_[index])&&
                (index+1)*2+1<=size_?comp_(data_[(index+1)*2-1],data_[(index+1)*2]):true)
            {
                Mathf::swap(data_[(index+1)*2-1],data_[index]);
                index=(index+1)*2-1;
            }
            else if ((index+1)*2+1<=size_&&comp_(data_[(index+1)*2],data_[index]))
            {
                Mathf::swap(data_[(index+1)*2],data_[index]);
                index=(index+1)*2;
            }
            else
            {
                break;
            }
        }
        return data_[size_];
    }
    
    T top()
    {
        if (size_==0)
        {
            throw std::out_of_range("Empty queue");
        }
        return data_[size_-1];
    }

    [[nodiscard]] bool empty() const
    {
        return size_==0;
    }
    
private:
    Compare comp_;
    T* data_;
    int total_size_;
    int size_;
};
