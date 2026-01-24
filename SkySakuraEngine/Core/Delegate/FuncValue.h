#pragma once
#include "IFunc.h"

template <class Return, class... Args>
class FuncValue:public IFunc<Return, Args...>
{
public:
    using func_type=Return(*)(Args...);
    
    FuncValue(Return(*outer_func)(Args...)) : IFunc<Return, Args...>(false,false)
    {
        func_ = outer_func;
    }

    Return invoke(Args... args) override
    {
        return func_(args...);
    }
    
    func_type get_func()
    {
        return func_;
    }
private:
    Return (*func_)(Args...);
};
