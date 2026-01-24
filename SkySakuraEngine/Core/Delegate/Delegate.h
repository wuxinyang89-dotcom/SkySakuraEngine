#pragma once
#include <stdexcept>

#include "FuncValue.h"
#include "FuncValueWithClass.h"
#include "FuncValueWithClassConst.h"


template<typename Return,typename... Args>
class Delegate
{
public:
    
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
    
    template<typename Class>
    void bind(Class* class_ptr,Return(Class::*func)(Args...))
    {
        if (size_ == total_size_)
        {
            expand_size_when_add();
        }
        delegate_list_[size_]=new FuncValueWithClass<Class,Return, Args...>(class_ptr,func);
        size_++;
    }
    
    template<typename Class>
    void bind(Class* class_ptr,Return(Class::*func)(Args...) const)
    {
        if (size_ == total_size_)
        {
            expand_size_when_add();
        }
        delegate_list_[size_]=new FuncValueWithClassConst<Class,Return, Args...>(class_ptr,func);
        size_++;
    }
    
    void bind(Delegate& delegate)
    {
        if (static_cast<unsigned int>(size_+delegate.count())>=1<<31)
        {
            throw std::out_of_range("List overflow");
        }
        if (static_cast<unsigned int>(size_+delegate.count())>total_size_)
        {
            expand_size_when_add_several(size_+delegate.count());
        }
        for (int i=0;i<delegate.count();i++)
        {
            delegate_list_[size_+i]=delegate.delegate_list_[i];
        }
        size_+=delegate.count();
    }
    
    void unbind(Return(*func)(Args...))
    {
        int index=-1;
        for (int i=0;i<size_;i++)
        {
            if (!delegate_list_[i]->has_class()&&!delegate_list_[i]->is_const()
                &&static_cast<FuncValue<Return, Args...> *>(delegate_list_[i])->get_func()==func)
            {
                index=i;
                break;
            }
        }
        if (index==-1)
        {
            throw std::out_of_range("Invalid function index");
        }
        for (int i=index;i<size_-1;i++)
        {
            delegate_list_[i]=delegate_list_[i+1];
        }
        size_--;
    }
    
    template<typename Class>
    void unbind(Class* class_ptr,Return(Class::*func)(Args...))
    {
        int index=-1;
        for (int i=0;i<size_;i++)
        {
            if (delegate_list_[i]->has_class()&&!delegate_list_[i]->is_const()
                &&static_cast<FuncValueWithClass<Class,Return, Args...> *>(delegate_list_[i])->get_class()==class_ptr
                &&static_cast<FuncValueWithClass<Class,Return, Args...> *>(delegate_list_[i])->get_func()==func)
            {
                index=i;
                break;
            }
        }
        if (index==-1)
        {
            throw std::out_of_range("Invalid function index");
        }
        for (int i=index;i<size_-1;i++)
        {
            delegate_list_[i]=delegate_list_[i+1];
        }
        size_--;
    }
    
    template<typename Class>
    void unbind(Class* class_ptr,Return(Class::*func)(Args...) const)
    {
        int index=-1;
        for (int i=0;i<size_;i++)
        {
            if (delegate_list_[i]->has_class()&&delegate_list_[i]->is_const()
                &&static_cast<FuncValueWithClassConst<Class,Return, Args...> *>(delegate_list_[i])->get_class()==class_ptr
                &&static_cast<FuncValueWithClassConst<Class,Return, Args...> *>(delegate_list_[i])->get_func()==func)
            {
                index=i;
                break;
            }
        }
        if (index==-1)
        {
            throw std::out_of_range("Invalid function index");
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
    
    Return invoke(Args... arg)
    {
        Return result=NULL;
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
    
    void expand_size_when_add_several(unsigned int target)
    {
        while (target>total_size_)
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
