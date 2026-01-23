#pragma once
#include "IFunc.h"

template <typename Class,typename Return,typename... Args>
class FuncValueWithClass:IFunc<Return,Args...>
{
public:
    FuncValueWithClass(Class* outer_object,Return(Class::*outer_func)(Args...))
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
