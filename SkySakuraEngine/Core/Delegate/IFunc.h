#pragma once

template <typename Return,typename... Args>
class IFunc
{
public:
    virtual Return invoke(Args... args)=0;
    
protected:
    virtual ~IFunc() = default;
};
