#pragma once
#include <functional>
#include <stdexcept>

#include "Dictionary/SetValue.h"

template<class TValue>
class Set
{
public:
    Set()
    {
        count_=0;
        total_size_=4;
        hash_list_=new SetValue<TValue>[total_size_];
    }
    
    Set(const Set& other)
    {
        count_=other.count_;
        total_size_=other.total_size_;
        hash_list_=other.hash_list_;
        for(int i=0;i<total_size_;i++)
        {
            hash_list_[i]=other.hash_list_[i];
        }
    }
    
    Set& operator=(const Set& other)
    {
        if(this!=&other)
        {
            count_=other.count_;
            total_size_=other.total_size_;
            hash_list_=other.hash_list_;
            for(int i=0;i<total_size_;i++)
            {
                hash_list_[i]=other.hash_list_[i];
            }
        }
        return *this;
    }
    
    bool empty() const
    {
        return count_==0;
    }
    
    unsigned int size() const
    {
        return count_;
    }
    
    bool exists(const TValue& value)
    {
        int index=calculate_hash(value)&calculate_hash_mod(total_size_);
        int offset=0;
        while (hash_list_[index]!=nullptr&&hash_list_[index].value!=value)
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
    
    void insert(const TValue& value)
    {
        unsigned int hash=calculate_hash(value);
        int index=hash&calculate_hash_mod(total_size_);
        int offset=0;
        while (hash_list_[index]!=nullptr&&hash_list_[index].value!=value)
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
                hash_list_[index]=new SetValue<TValue>(value,hash);
                expand_size();
                return;
            }
            hash_list_[index]=new SetValue<TValue>(value,hash);
        }
    }
    
    void erase(const TValue& value)
    {
        int index=calculate_hash(value)&calculate_hash_mod(total_size_);
        int offset=0;
        while (hash_list_[index]!=nullptr&&hash_list_[index].value!=value)
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
    SetValue<TValue>* hash_list_;
    unsigned int count_;
    unsigned int total_size_;
    
    static unsigned int calculate_hash(TValue& value)
    {
        return std::hash<TValue>()(value);
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
        SetValue<TValue>* temp=new SetValue<TValue>[total_size_];
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
