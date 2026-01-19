#pragma once

template<class TValue>
class SetValue
{
public:
    TValue value;
    unsigned int hash;
    SetValue(TValue value,unsigned int hash):value(value),hash(hash){}
};
