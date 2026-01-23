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
