#pragma once

template <typename Return,typename... Args>
class IFunc
{
public:
    virtual Return invoke(Args... args)=0;
    
    virtual ~IFunc() = default;
};
