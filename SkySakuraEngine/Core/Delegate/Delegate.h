#pragma once
#include <stdexcept>

template<typename Return,typename... Args>
class Delegate
{
public:
    typedef Return(*delegateFunc)(Args...);
    
    Delegate()
    {
        size_=0;
        total_size_=4;
        delegate_list_=new delegateFunc[total_size_];
    }
    
    void operator+=(delegateFunc func)
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
            delegateFunc* temp=new delegateFunc[total_size_];
            for (int i=0;i<size_;i++)
            {
                temp[i]=delegate_list_[i];
            }
            delete [] delegate_list_;
            delegate_list_=temp;
        }
        delegate_list_[size_]=func;
        size_++;
    }
    
    void operator+=(Delegate& delegate)
    {
        while (static_cast<unsigned int>(size_+delegate.count())>total_size_)
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
            delegateFunc* temp=new delegateFunc[total_size_];
            for (int i=0;i<size_;i++)
            {
                temp[i]=delegate_list_[i];
            }
            delete [] delegate_list_;
            delegate_list_=temp;
        }
        for (int i=0;i<delegate.count();i++)
        {
            delegate_list_[size_+i]=delegate.delegate_list_[i];
        }
        size_+=delegate.count();
    }
    
    void operator-=(delegateFunc func)
    {
        int index=0;
        for (int i=0;i<size_;i++)
        {
            if (delegate_list_[i]==func)
            {
                index=i;
                break;
            }
        }
        for (int i=index;i<size_-1;i++)
        {
            delegate_list_[i]=delegate_list_[i+1];
        }
        size_--;
    }
    
    int count() const
    {
        return size_;
    }
    
    ~Delegate()
    {
        delete [] delegate_list_;
    }
    
private:
    delegateFunc* delegate_list_;
    int size_;
    int total_size_;
};
