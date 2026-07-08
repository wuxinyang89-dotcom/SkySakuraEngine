#pragma once
#include "List.h"

class String :public Array<char>
{
public:
    String()=default;

    explicit String(const char* src_str)
    {
        size_=c_str_length(src_str);
        data_=new char[size_];
        for (int i=0;i<size_;i++)
        {
            data_[i]=src_str[i];
        }
    }

    String(const String& str)
    {
        size_=str.size_;
        data_=new char[size_];
        for (int i=0;i<size_;i++)
        {
            data_[i]=str.data_[i];
        }
    }

    String(String&& str) noexcept
    {
        size_=str.size_;
        data_=str.data_;
        str.data_=nullptr;
        str.size_=0;
    }

    String& operator=(const char* src_str)
    {
        const int new_size=c_str_length(src_str);
        char* new_data=new char[new_size];
        for (int i=0;i<new_size;i++)
        {
            new_data[i]=src_str[i];
        }
        delete [] data_;
        data_=new_data;
        size_=new_size;
        return *this;
    }

    String& operator=(const String& str)
    {
        if (this==&str)
        {
            return *this;
        }
        char* new_data=new char[str.size_];
        for (int i=0;i<str.size_;i++)
        {
            new_data[i]=str.data_[i];
        }
        delete [] data_;
        data_=new_data;
        size_=str.size_;
        return *this;
    }

    String& operator=(String&& str) noexcept
    {
        if (this==&str)
        {
            return *this;
        }
        delete [] data_;
        size_=str.size_;
        data_=str.data_;
        str.data_=nullptr;
        str.size_=0;
        return *this;
    }

    void append(const char* src_str)
    {
        const int count=c_str_length(src_str);
        auto temp=new char[size_+count];
        for (int i=0;i<size_;i++)
        {
            temp[i]=data_[i];
        }
        for (int i=0;i<count;i++)
        {
            temp[size_+i]=src_str[i];
        }
        delete [] data_;
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
        for (int i=0;i<str.size_;i++)
        {
            temp[size_+i]=str.data_[i];
        }
        delete [] data_;
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

    [[nodiscard]] int find(const String& str) const
    {
        return boyer_moore(data_,size_,str.data_,str.size_);
    }

    [[nodiscard]] int find(const char* src_str) const
    {
        const int pattern_size=c_str_length(src_str);
        return boyer_moore(data_,size_,src_str,pattern_size);
    }

    String operator+ (const String& str) const
    {
        String new_str;
        new_str.size_=size_+str.size_;
        new_str.data_=new char[new_str.size_];
        for (int i=0;i<size_;i++)
        {
            new_str.data_[i]=data_[i];
        }
        for (int i=0;i<str.size_;i++)
        {
            new_str.data_[size_+i]=str.data_[i];
        }
        return new_str;
    }

    String operator+ (const char* src_str) const
    {
        const int count=c_str_length(src_str);
        String new_str;
        new_str.size_=size_+count;
        new_str.data_=new char[new_str.size_];
        for (int i=0;i<size_;i++)
        {
            new_str.data_[i]=data_[i];
        }
        for (int i=0;i<count;i++)
        {
            new_str.data_[size_+i]=src_str[i];
        }
        return new_str;
    }

    friend String operator+(const char* src_str,const String& str)
    {
        const int count=c_str_length(src_str);
        String new_str;
        new_str.size_=count+str.size_;
        new_str.data_=new char[new_str.size_];
        for (int i=0;i<count;i++)
        {
            new_str.data_[i]=src_str[i];
        }
        for (int i=0;i<str.size_;i++)
        {
            new_str.data_[count+i]=str.data_[i];
        }
        return new_str;
    }

    // 返回从 index 开始的下一个 UTF-8 码点的起始下标
    int next(int index) const
    {
        if (index<0||index>=size_)
        {
            throw std::out_of_range("next index out of range");
        }
        auto origin_char=static_cast<unsigned char>(data_[index]);
        index++;
        while ((origin_char&0x80)==0x80)
        {
            if (index>=size_)
            {
                throw std::runtime_error("Illegal UTF-8 string!");
            }
            if ((static_cast<unsigned char>(data_[index])&0xC0)!=0x80)
            {
                throw std::runtime_error("Illegal UTF-8 string!");
            }
            index++;
            origin_char<<=1;
        }
        return index;
    }

    // 返回一个以 '\0' 结尾的 C 字符串副本，可直接传给 C 接口。
    // 注意：返回的内存由本函数新分配，调用者使用完毕后需自行 delete[] 释放。
    [[nodiscard]] char* c_str() const
    {
        char* result=new char[size_+1];
        for (int i=0;i<size_;i++)
        {
            result[i]=data_[i];
        }
        result[size_]='\0';
        return result;
    }

private:
    // 计算 C 字符串（以 '\0' 结尾）的长度，不含结尾符
    static int c_str_length(const char* src_str)
    {
        int length=0;
        if (src_str==nullptr)
        {
            return length;
        }
        while (src_str[length]!='\0')
        {
            length++;
        }
        return length;
    }

    // Boyer-Moore 字符串匹配，返回 pattern 在 text 中首次出现的起始下标，找不到返回 -1
    static int boyer_moore(const char* text,const int n,const char* pattern,const int m)
    {
        if (m==0)
        {
            return 0;
        }
        if (n<m||text==nullptr||pattern==nullptr)
        {
            return -1;
        }

        // 坏字符规则：记录每个字符在 pattern 中最后出现的位置
        int bad_char[256];
        for (int& slot : bad_char)
        {
            slot=-1;
        }
        for (int i=0;i<m;i++)
        {
            bad_char[static_cast<unsigned char>(pattern[i])]=i;
        }

        // 好后缀规则预处理
        int* suffix=new int[m];
        bool* prefix=new bool[m];
        for (int i=0;i<m;i++)
        {
            suffix[i]=-1;
            prefix[i]=false;
        }
        for (int i=0;i<m-1;i++)
        {
            int j=i;
            int k=0;
            while (j>=0&&pattern[j]==pattern[m-1-k])
            {
                j--;
                k++;
                suffix[k]=j+1;
            }
            if (j==-1)
            {
                prefix[k]=true;
            }
        }

        int result=-1;
        int s=0;// text 中与 pattern 首字符对齐的位置
        while (s<=n-m)
        {
            int j=m-1;
            while (j>=0&&pattern[j]==text[s+j])
            {
                j--;
            }
            if (j<0)
            {
                result=s;
                break;
            }
            const int bad_char_shift=j-bad_char[static_cast<unsigned char>(text[s+j])];
            int good_suffix_shift=1;
            if (j<m-1)
            {
                good_suffix_shift=move_by_good_suffix(j,m,suffix,prefix);
            }
            int shift=bad_char_shift>good_suffix_shift?bad_char_shift:good_suffix_shift;
            if (shift<1)
            {
                shift=1;
            }
            s+=shift;
        }
        delete[] suffix;
        delete[] prefix;
        return result;
    }

    // 根据好后缀规则计算移动步长
    static int move_by_good_suffix(const int j,const int m,const int* suffix,const bool* prefix)
    {
        const int k=m-1-j;// 好后缀的长度
        if (suffix[k]!=-1)
        {
            return j-suffix[k]+1;
        }
        for (int r=j+2;r<=m-1;r++)
        {
            if (prefix[m-r])
            {
                return r;
            }
        }
        return m;
    }
};
