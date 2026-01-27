#pragma once
#include <stdexcept>

template <typename T>
class Queue
{
public:
    Queue()
    {
        total_size_ = 4;
        head_=0;
        rear_=0;
        array_=new T[total_size_];
    }
    
    Queue(const Queue<T>& other)
    {
        total_size_=other.total_size_;
        head_=other.head_;
        rear_=other.rear_;
        array_=new T[total_size_];
        if (rear_<=head_)
        {
            for (int i=rear_;i<head_;i++)
            {
                array_[i]=other.array_[i];
            }
        }
        else
        {
            for (int i=0;i<head_;i++)
            {
                array_[i]=other.array_[i];
            }
            for (int i=rear_;i<total_size_;i++)
            {
                array_[i]=other.array_[i];
            }
        }
    }
    
    Queue<T>& operator=(const Queue<T>& other)
    {
        if (this!=&other)
        {
            delete[] array_;
            total_size_=other.total_size_;
            head_=other.head_;
            rear_=other.rear_;
            array_=new T[total_size_];
            if (rear_<=head_)
            {
                for (int i=rear_;i<head_;i++)
                {
                    array_[i]=other.array_[i];
                }
            }
            else
            {
                for (int i=0;i<head_;i++)
                {
                    array_[i]=other.array_[i];
                }
                for (int i=rear_;i<total_size_;i++)
                {
                    array_[i]=other.array_[i];
                }
            }
        }
        return *this;
    }
    
    void push(const T& value)
    {
        if ((head_+1)%total_size_==rear_)
        {
            int size=total_size_;
            if (total_size_==~(1<<31))
            {
                throw std::out_of_range("Queue overflow");
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
            
            for (int i=0;i<=head_;i++)
            {
                temp[i]=array_[i];
            }
            if (rear_>head_)
            {
                for (int i=0;i<=size-rear_;i++)
                {
                    temp[total_size_-1-i]=array_[size-1-i];
                }
            }
            delete[] array_;
        }
        array_[head_]=value;
        head_=(++head_)%total_size_;
    }
    
    T pop()
    {
        if (head_==rear_)
        {
            throw std::out_of_range("Queue underflow");
        }
        rear_=(++rear_)%total_size_;
        if (rear_==0)
        {
            return array_[total_size_-1];
        }
        return array_[rear_];
    }
    
    T& front()
    {
        if (head_==rear_)
        {
            throw std::out_of_range("Queue empty");
        }
        return array_[head_];
    }
    
    T& back()
    {
        if (head_==rear_)
        {
            throw std::out_of_range("Queue empty");
        }
        return array_[rear_];
    }
    
    void clear()
    {
        head_=0;
        rear_=0;
    }
    
    int size() const
    {
        if (rear_<=head_)
        {
            return head_-rear_;
        }
        return head_+total_size_-rear_;
    }
    
    bool empty() const
    {
        return head_==rear_;
    }
    
    ~Queue()
    {
        delete[] array_;
    }
    
private:
    T* array_;
    int total_size_;
    int head_;
    int rear_;
};
