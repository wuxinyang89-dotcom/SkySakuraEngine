#pragma once

template<typename TKey,typename TValue>
struct DictionaryValue
{
    TKey key;
    TValue value;
    unsigned int hash;
    
    DictionaryValue(TKey key,TValue value,unsigned int hash):key(key),value(value),hash(hash){}
};
