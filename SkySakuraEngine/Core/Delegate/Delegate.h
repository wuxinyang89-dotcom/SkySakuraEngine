#pragma once
#include <stdexcept>

#include "FuncValue.h"

template<typename Reutrn,typename... Args>
class Delegate;

template<typename Class,typename Return,typename... Args>
class Delegate<Return,Args...>
{
public:
    using delegateFunc=Return(Class::*)(Args...);
    
    Delegate()
    {
        size_=0;
        total_size_=4;
        delegate_list_=new FuncValue<Return, Args...>[total_size_];
    }
    
    Delegate(Delegate& delegate)=delete;
    
    Delegate& operator=(Delegate& delegate)=delete;
    
    Delegate(Delegate&& delegate)=delete;
    
    Delegate& operator=(Delegate&& delegate)=delete;
    
    void bind(Class classname,delegateFunc func)
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
            delegateFunc* temp=new FuncValue<Return, Args...>[total_size_];
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
    
    void bind(Delegate& delegate)
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
    
    void unbind(delegateFunc func)
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
    
    Return invoke(Args arg...)
    {
        Return result;
        for (int i=0;i<size_;i++)
        {
            result=(*delegate_list_[i])(arg...);
        }
        return result;
    }
    
    ~Delegate()
    {
        delete [] delegate_list_;
    }
    
private:
    FuncValue<Return,Args...>* delegate_list_;
    int size_;
    int total_size_;
};
