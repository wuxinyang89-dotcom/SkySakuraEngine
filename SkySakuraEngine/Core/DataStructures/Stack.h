#pragma once
#include <stdexcept>

template <typename T>
class Stack
{
public:
    Stack()
    {
        size_=0;
        total_size_=4;
        array_=new T[total_size_];
    }
    
    Stack(const Stack& stack)
    {
        size_=stack.size_;
        total_size_=stack.size_;
        array_=new T[total_size_];
        for (int i=0;i<size_;i++)
        {
            array_[i]=stack.array_[i];
        }
    }
    
    Stack& operator=(const Stack& stack)
    {
        if (this!=&stack)
        {
            size_=stack.size_;
            total_size_=stack.size_;
            array_=new T[total_size_];
            for (int i=0;i<size_;i++)
            {
                array_[i]=stack.array_[i];
            }
        }
        return *this;
    }
    
    void push(const T item)
    {
        if (size_ == total_size_)
        {
            if (total_size_==~(1<<31))
            {
                throw std::out_of_range("Stack overflow");
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
        array_[size_]=item;
        size_++;
    }
    
    T pop()
    {
        if (size_==0)
        {
            throw std::out_of_range("Stack is empty");
        }
        size_--;
        return array_[size_];
    }
    
    T& top()
    {
        if (size_==0)
        {
            throw std::out_of_range("Stack is empty");
        }
        return array_[size_-1];
    }
    
    void clear()
    {
        size_=0;
    }
    
    int size() const
    {
        return size_;
    }
    
    bool empty() const
    {
        return size_==0;
    }
    
    
    ~Stack()
    {
        delete[] array_;
    }
private:
    int total_size_;
    int size_;
    T* array_;
};
