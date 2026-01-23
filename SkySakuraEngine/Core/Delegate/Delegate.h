#pragma once
#include <stdexcept>

#include "FuncValue.h"
#include "FuncValueWithClass.h"


template<typename Return,typename... Args>
class Delegate
{
public:
    //using delegateFunc=Return(Class::*)(Args...);
    
    Delegate()
    {
        size_=0;
        total_size_=4;
        delegate_list_=new IFunc<Return, Args...>*[total_size_];
    }
    
    Delegate(Delegate& delegate)=delete;
    
    Delegate& operator=(Delegate& delegate)=delete;
    
    Delegate(Delegate&& delegate)=delete;
    
    Delegate& operator=(Delegate&& delegate)=delete;
    
    void bind(Return(*func)(Args...))
    {
        if (size_ == total_size_)
        {
            expand_size_when_add();
        }
        delegate_list_[size_]=new FuncValue<Return, Args...>(func);
        size_++;
    }
    
    void bind(const Return(*func)(Args...))
    {
        if (size_ == total_size_)
        {
            expand_size_when_add();
        }
        delegate_list_[size_]=new FuncValue<Return, Args...>(func);
        size_++;
    }
    
    template<typename Class>
    void bind(Class classname,Return(Class::*func)(Args...))
    {
        if (size_ == total_size_)
        {
            expand_size_when_add();
        }
        delegate_list_[size_]=new FuncValueWithClass<Class,Return, Args...>(classname,func);
        size_++;
    }
    
    template<typename Class>
    void bind(Class classname,Return(Class::*func)(Args...) const)
    {
        if (size_ == total_size_)
        {
            expand_size_when_add();
        }
        delegate_list_[size_]=new FuncValueWithClass<Class,Return, Args...>(classname,func);
        size_++;
    }
    
    void bind(Delegate& delegate)
    {
        while (static_cast<unsigned int>(size_+delegate.count())>total_size_)
        {
            expand_size_when_add();
        }
        for (int i=0;i<delegate.count();i++)
        {
            delegate_list_[size_+i]=delegate.delegate_list_[i];
        }
        size_+=delegate.count();
    }
    
    /*void unbind(delegateFunc func)
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
    }*/
    
    int count() const
    {
        return size_;
    }
    
    Return invoke(Args... arg)
    {
        Return result;
        for (int i=0;i<size_;i++)
        {
            result=delegate_list_[i]->invoke(arg...);
        }
        return result;
    }
    
    ~Delegate()
    {
        delete [] delegate_list_;
    }
    
private:
    IFunc<Return,Args...>** delegate_list_;
    int size_;
    int total_size_;
    
    void expand_size_when_add()
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
        IFunc<Return,Args...>** temp=new IFunc<Return,Args...>*[total_size_];
        for (int i=0;i<size_;i++)
        {
            temp[i]=delegate_list_[i];
        }
        delete [] delegate_list_;
        delegate_list_=temp;
    }
};
