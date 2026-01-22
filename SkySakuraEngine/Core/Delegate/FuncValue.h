#pragma once
#include "IFunc.h"

template <class Return, class... Args>
class FuncValue: IFunc<Return, Args...>
{
public:
    FuncValue(Return(*outer_func)(Args...))
    {
        func_=outer_func;
    }
    
    Return invoke(Args... args) override
    {
        return func_(args...);
    }
private:
    Return (*func_)(Args...);
};

template <typename Class,typename Return,typename... Args>
class FuncValue: IFunc<Return, Args...>
{
public:
    FuncValue(Class* outer_object,Return(Class::*outer_func)(Args...))
    {
        object_=outer_object;
        func_=outer_func;
    }
    
    
    Return invoke(Args... args) override
    {
        return (object_->*func_)(args...);
    }
private:
    Class* object_;
    Return (Class::*func_)(Args...);
};