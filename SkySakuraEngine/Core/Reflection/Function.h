#pragma once

template<typename T>
class Function;

template<typename Return,typename... Args>
class Function<Return(*)(Args...)>
{
public:
    Function(Return (*func)(Args...))
    {
        func_ = func;
    }
    
    using return_type = Return;
    
    bool constexpr is_const=false;
    bool constexpr is_member=false;
    
    Return operator()(Args... args)
    {
        return func_(args...);
    }
private:
    Return (*func_)(Args...);
};

template<typename Return,typename... Args>
class Function<const Return(*)(Args...)>
{
public:
    using return_type = Return;
};

template<typename Return,typename Class,typename... Args>
class Function<Return(Class::*)(Args...)>
{
public:
    using return_type = Return;
    using class_type = Class;
    
    explicit Function(Class class_,Return(Class::*func)(Args...))
    {
        curr_class_ = class_;
        func_ = func;
    }
    
    bool constexpr is_const=false;
    bool constexpr is_member=true;
    
    Return operator()(Class classname,Args... args)
    {
        return (classname.*func_)(args...);
    }
private:
    Class* curr_class_;
    Return (Class::*func_)(Args...);
};

template<typename Return,typename Class,typename... Args>
class Function<Return(Class::*)(Args...) const>
{
public:
    explicit Function(Class* class_,Return(Class::*func)(Args...) const)
    {
        curr_class_ = class_;
        func_ = func;
    }
    
    using return_type = Return;
    using class_type = Class;
    //using arg_list = List<Args...>;
    using arg_list = std::tuple<Args...>;
    
    bool constexpr is_const=true;
    bool constexpr is_member=true;
    
    Return operator()(Args... args)
    {
        return (curr_class_->*func_)(args...);
    }
    
private:
    Class* curr_class_;
    Return (Class::*func_)(Args...) const;
};
