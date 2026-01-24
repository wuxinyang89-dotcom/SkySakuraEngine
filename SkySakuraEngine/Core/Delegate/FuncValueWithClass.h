#pragma once
#include "IFunc.h"

template <typename Class,typename Return,typename... Args>
class FuncValueWithClass:public IFunc<Return,Args...>
{
public:
    using func_type = Return (Class::*)(Args... args);
    
    FuncValueWithClass(Class* outer_object,Return(Class::*outer_func)(Args...)) : IFunc<Return, Args...>(true,false)
    {
        object_ = outer_object;
        func_ = outer_func;
    }

    Return invoke(Args... args) override
    {
        return (object_->*func_)(args...);
    }
    
    Class* get_class()
    {
        return object_;
    }
    
    func_type get_func()
    {
        return func_;
    }
private:
    Class* object_;
    Return (Class::*func_)(Args...);
};
