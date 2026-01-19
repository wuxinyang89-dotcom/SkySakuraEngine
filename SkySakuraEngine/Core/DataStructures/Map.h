#pragma once
#include <stdexcept>
#include <type_traits>

#include "Dictionary/DictionaryValue.h"

template<typename TKey,typename TValue>
class Map
{
public:
    Map()
    {
        count_=0;
        total_size_=4;
        hash_list_=new DictionaryValue<TKey,TValue>[total_size_];
    }
    Map(const Map& other)
    {
        count_=other.count_;
        total_size_=other.total_size_;
        hash_list_=new DictionaryValue<TKey,TValue>[total_size_];
        for (int i=0;i<total_size_;i++)
        {
            hash_list_[i]=other.hash_list_[i];
        }
    }
    Map& operator=(const Map& other)
    {
        if (this!=&other)
        {
            count_=other.count_;
            total_size_=other.total_size_;
            hash_list_=new DictionaryValue<TKey,TValue>[total_size_];
            for (int i=0;i<total_size_;i++)
            {
                hash_list_[i]=other.hash_list_[i];
            }
        }
        return *this;
    }
    
    TValue& operator[](const TKey& key)
    {
        unsigned int hash=calculate_hash(key);
        int index=hash&calculate_hash_mod(total_size_);
        int offset=0;
        while (hash_list_[index]!=nullptr&&hash_list_[index].key!=key)
        {
            offset=~offset+1;
            offset++;
            index+=offset;
            if (index>=total_size_)
            {
                index-=total_size_;
            }
            else if (index<0)
            {
                index+=total_size_;
            }
        }
        if (hash_list_[index]==nullptr)
        {
            if (++count_*4>total_size_*3)
            {
                hash_list_[index]=new DictionaryValue<TKey,TValue>(key,nullptr,hash);
                DictionaryValue<TKey,TValue>& ans=hash_list_[index];
                expand_size();
                return ans;
            }
            hash_list_[index]=new DictionaryValue<TKey,TValue>(key,nullptr,hash);
            return hash_list_[index].value;
        }
        return hash_list_[index].value;
        
    }
    
    bool empty() const
    {
        return count_==0;
    }
    
    unsigned int size() const
    {
        return count_;
    }
    
    bool exists(const TKey& key) const
    {
        int index=calculate_hash(key)&calculate_hash_mod(total_size_);
        int offset=0;
        while (hash_list_[index]!=nullptr&&hash_list_[index].key!=key)
        {
            offset=~offset+1;
            offset++;
            index+=offset;
            if (index>=total_size_)
            {
                index-=total_size_;
            }
            else if (index<0)
            {
                index+=total_size_;
            }
        }
        if (hash_list_[index]==nullptr)
        {
            return false;
        }
        return true;
    }
    
    void clear()
    {
        count_=0;
        for (int i=0;i<total_size_;i++)
        {
            hash_list_[i]=nullptr;
        }
    }
    
    void insert(const TKey& key,const TValue& value)
    {
        unsigned int hash=calculate_hash(key);
        int index=hash&calculate_hash_mod(total_size_);
        int offset=0;
        while (hash_list_[index]!=nullptr&&hash_list_[index].key!=key)
        {
            offset=~offset+1;
            offset++;
            index+=offset;
            if (index>=total_size_)
            {
                index-=total_size_;
            }
            else if (index<0)
            {
                index+=total_size_;
            }
        }
        if (hash_list_[index]==nullptr)
        {
            if (++count_*4>total_size_*3)
            {
                hash_list_[index]=new DictionaryValue<TKey,TValue>(key,nullptr,hash);
                expand_size();
                return;
            }
            hash_list_[index]=new DictionaryValue<TKey,TValue>(key,nullptr,hash);
            return;
        }
        hash_list_[index].value=value;
    }
    
    void erase(const TKey& key)
    {
        int index=calculate_hash(key)&calculate_hash_mod(total_size_);
        int offset=0;
        while (hash_list_[index]!=nullptr&&hash_list_[index].key!=key)
        {
            offset=~offset+1;
            offset++;
            index+=offset;
            if (index>=total_size_)
            {
                index-=total_size_;
            }
            else if (index<0)
            {
                index+=total_size_;
            }
        }
        if (hash_list_[index]==nullptr)
        {
            return;
        }
        hash_list_[index]=nullptr;
        count_--;
    }
private:
    DictionaryValue<TKey,TValue>* hash_list_;
    unsigned int count_;
    unsigned int total_size_;
    
    static unsigned int calculate_hash(TKey& key)
    {
        return std::hash<TKey>()(key);
    }
    
    static int calculate_hash_mod(const unsigned int total_size)
    {
        return total_size-1;
    }
    
    void expand_size()
    {
        if (total_size_==1<<31)
        {
            throw std::out_of_range("Map expansion size is too large");
        }
        total_size_<<=1;
        int hash_mod_=calculate_hash_mod(total_size_);
        DictionaryValue<TKey,TValue>* temp=new DictionaryValue<TKey,TValue>[total_size_];
        for (int i=0;i<(total_size_>>1);i++)
        {
            if (hash_list_[i]!=nullptr)
            {
                int index=hash_list_[i].hash&hash_mod_;
                int offset=0;
                while (temp[index]!=nullptr)
                {
                    offset=~offset+1;
                    offset++;
                    index+=offset;
                    if (index>=total_size_)
                    {
                        index-=total_size_;
                    }
                    else if (index<0)
                    {
                        index+=total_size_;
                    }
                }
                temp[index]=hash_list_[i];
            }
        }
        delete [] hash_list_;
        hash_list_=temp;
    }
};
