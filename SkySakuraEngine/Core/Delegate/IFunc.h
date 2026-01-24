#pragma once

template <typename Return,typename... Args>
class IFunc
{
public:
    IFunc(bool hasclass,bool isconst)
    {
        has_class_ = hasclass;
        is_const_ = isconst;
    }
    
    virtual Return invoke(Args... args)=0;
    
    virtual ~IFunc() = default;
    
    bool has_class() const
    {
        return has_class_;
    }
    
    bool is_const() const
    {
        return is_const_;
    }
    
private:
    bool has_class_;
    bool is_const_;
};
