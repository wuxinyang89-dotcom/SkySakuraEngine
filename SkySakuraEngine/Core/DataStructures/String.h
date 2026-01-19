#pragma once
#include "List.h"

class String :public Array<char>
{
public:
    String()=default;
    
    explicit String(const char* src_str)
    {
        auto ptr=src_str;
        size_=0;
        while (ptr!=nullptr)
        {
            ptr++;
            size_++;
        }
        data_=new char[size_];
        auto data_ptr=data_;
        for (int i=0;i<size_;i++)
        {
            data_ptr[i]=src_str[i];
        }
    }

    String& operator=(const char* src_str)
    {
        delete [] data_;
        auto ptr=src_str;
        size_=0;
        while (ptr!=nullptr)
        {
            ptr++;
            size_++;
        }
        data_=new char[size_];
        auto data_ptr=data_;
        for (int i=0;i<size_;i++)
        {
            data_ptr[i]=src_str[i];
        }
        return *this;
    }
    
    String& operator=(const String& str)
    {
        delete [] data_;
        size_=str.size_;
        data_=new char[size_];
        for (int i=0;i<size_;i++)
        {
            data_[i]=str[i];
        }
        return *this;
    }
    
    String& operator=(String&& str) noexcept
    {
        delete [] data_;
        size_=str.size_;
        data_=str.data_;
        return *this;
    }
    
    void append(const char* src_str)
    {
        auto ptr=src_str;
        int count=0;
        while (ptr!=nullptr)
        {
            ptr++;
            count++;
        }
        auto temp=new char[size_+count];
        for (int i=0;i<size_;i++)
        {
            temp[i]=data_[i];
        }
        auto temp_ptr=temp+size_;
        delete [] data_;
        ptr=src_str;
        while (ptr!=nullptr)
        {
            *temp_ptr=*ptr;
            temp_ptr++;
            ptr++;
        }
        size_+=count;
        data_=temp;
    }
    
    void append(const String& str)
    {
        auto temp=new char[size_+str.size_];
        for (int i=0;i<size_;i++)
        {
            temp[i]=data_[i];
        }
        delete [] data_;
        for (int i=0;i<str.size_;i++)
        {
            temp[size_+i]=str[i];
        }
        size_+=str.size_;
        data_=temp;
    }
    
    String substr(const int start,const int end) const
    {
        if (start>=end||start<0||start>=size_||end<0||end>size_)
        {
            throw std::out_of_range("substr out of range");
        }   
        String new_str;
        new_str.size_=end-start;
        new_str.data_=new char[new_str.size_];
        for (int i=start;i<end;i++)
        {
            new_str.data_[i-start]=data_[i];
        }
        return new_str;
    }
    
    int find(const String& str) const
    {
        int* pre_process=new int[size_];
        int count=0;
        for (int i=1;i<str.size_;i++)
        {
            if (str.data_[count]==data_[i])
            {
                count++;
                pre_process[i]=count;
            }
            else if (count!=0)
            {
                count--;
                for (int j=count;j>0;j--)
                {
                    
                }
            }
        }
        delete[] pre_process;
    }
    
    int find(const char* str) const
    {
        int* pre_process=new int[size_];
        
        delete[] pre_process;
    }
    
    String operator+ (const String& str) const
    {
        String new_str;
        new_str.data_=new char[size_+str.size_];
        for (int i=0;i<size_;i++)
        {
            new_str.data_[i]=data_[i];
        }
        new_str.size_=size_+str.size_;
        for (int i=0;i<str.size_;i++)
        {
            new_str.data_[size_+i]=str[i];
        }
        return new_str;
    }
    
    String operator+ (const char* src_str) const
    {
        auto ptr=src_str;
        String new_str;
        while (ptr!=nullptr)
        {
            ptr++;
            new_str.size_++;
        }
        new_str.size_+=size_;
        auto temp=new char[new_str.size_];
        for (int i=0;i<size_;i++)
        {
            temp[i]=data_[i];
        }
        auto temp_ptr=temp+size_;
        ptr=src_str;
        while (ptr!=nullptr)
        {
            *temp_ptr=*ptr;
            temp_ptr++;
            ptr++;
        }
        return new_str;
    }
    
    friend String operator+(const char* src_str,const String& str)
    {
        auto ptr=src_str;
        String new_str;
        while (ptr!=nullptr)
        {
            ptr++;
            new_str.size_++;
        }
        new_str.size_+=str.size_;
        auto temp=new char[new_str.size_];
        auto temp_ptr=temp;
        ptr=src_str;
        while (ptr!=nullptr)
        {
            *temp_ptr=*ptr;
            temp_ptr++;
            ptr++;
        }
        for (int i=0;i<str.size_;i++)
        {
            *(temp_ptr+i)=str[i];
        }
        return new_str;
    }
};
