#pragma once
#include "IFunc.h"

template <typename Class,typename Return,typename... Args>
class FuncValueWithClassConst:public IFunc<Return,Args...>
{
public:
    using func_type = Return (Class::*)(Args...) const;
    FuncValueWithClassConst(Class* outer_object,Return(Class::*outer_func)(Args...) const) : IFunc<Return, Args...>(true,true)
    {
        object_ = outer_object;
        func_ = outer_func;
    }

    Return invoke(Args... args) override
    {
        return (object_->*func_)(args...);
    }
    
    func_type get_func()
    {
        return func_;
    }
    
    Class* get_class()
    {
        return object_;
    }
private:
    Class* object_;
    Return (Class::*func_)(Args...) const;
};
