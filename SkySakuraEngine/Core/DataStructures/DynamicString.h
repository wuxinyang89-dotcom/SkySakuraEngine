#pragma once
#include "List.h"

class String;

class DynamicString:public List<char>
{
public:
    DynamicString()=default;

    explicit DynamicString(const char* src_str)
    {
        const int len=c_str_length(src_str);
        for (int i=0;i<len;i++)
        {
            push_back(src_str[i]);
        }
    }

    DynamicString(const DynamicString& str):List<char>(str)
    {
    }

    DynamicString(DynamicString&& str) noexcept
    {
        *this=static_cast<List<char>&&>(str);
    }

    DynamicString& operator=(const char* src_str)
    {
        clear();
        const int len=c_str_length(src_str);
        for (int i=0;i<len;i++)
        {
            push_back(src_str[i]);
        }
        return *this;
    }

    DynamicString& operator=(const DynamicString& str)
    {
        if (this==&str)
        {
            return *this;
        }
        List<char>::operator=(str);
        return *this;
    }

    DynamicString& operator=(DynamicString&& str) noexcept
    {
        if (this==&str)
        {
            return *this;
        }
        List<char>::operator=(static_cast<List<char>&&>(str));
        return *this;
    }

    // 追加 C 字符串（原地修改，利用 List 的动态扩容，无需重新分配整个数组）
    void append(const char* src_str)
    {
        const int len=c_str_length(src_str);
        for (int i=0;i<len;i++)
        {
            push_back(src_str[i]);
        }
    }

    // 追加另一个 DynamicString
    void append(const DynamicString& str)
    {
        for (int i=0;i<str.size();i++)
        {
            push_back(str[i]);
        }
    }

    // 追加单个字符
    void append(const char ch)
    {
        push_back(ch);
    }

    // 子串截取，返回新的 DynamicString
    [[nodiscard]] DynamicString substr(const int start,const int end) const
    {
        if (start>=end||start<0||start>=size()||end<0||end>size())
        {
            throw std::out_of_range("substr out of range");
        }
        DynamicString new_str;
        for (int i=start;i<end;i++)
        {
            new_str.push_back((*this)[i]);
        }
        return new_str;
    }

    // Boyer-Moore 字符串匹配
    [[nodiscard]] int find(const DynamicString& str) const
    {
        return boyer_moore(begin(),size(),str.begin(),str.size());
    }

    [[nodiscard]] int find(const char* src_str) const
    {
        const int pattern_size=c_str_length(src_str);
        return boyer_moore(begin(),size(),src_str,pattern_size);
    }

    // 拼接运算符
    DynamicString operator+(const DynamicString& str) const
    {
        DynamicString new_str(*this);
        new_str.append(str);
        return new_str;
    }

    DynamicString operator+(const char* src_str) const
    {
        DynamicString new_str(*this);
        new_str.append(src_str);
        return new_str;
    }

    friend DynamicString operator+(const char* src_str,const DynamicString& str)
    {
        DynamicString new_str(src_str);
        new_str.append(str);
        return new_str;
    }

    // += 运算符（原地追加）
    DynamicString& operator+=(const DynamicString& str)
    {
        append(str);
        return *this;
    }

    DynamicString& operator+=(const char* src_str)
    {
        append(src_str);
        return *this;
    }

    // 返回从 index 开始的下一个 UTF-8 码点的起始下标
    int next(int index) const
    {
        if (index<0||index>=size())
        {
            throw std::out_of_range("next index out of range");
        }
        auto origin_char=static_cast<unsigned char>((*this)[index]);
        index++;
        while ((origin_char&0x80)==0x80)
        {
            if (index>=size())
            {
                throw std::runtime_error("Illegal UTF-8 string!");
            }
            if ((static_cast<unsigned char>((*this)[index])&0xC0)!=0x80)
            {
                throw std::runtime_error("Illegal UTF-8 string!");
            }
            index++;
            origin_char<<=1;
        }
        return index;
    }

    // 将字符串解析为 float
    [[nodiscard]] float to_float() const
    {
        const int len=size();
        float result=0.0f;
        float fraction=0.0f;
        float divisor=1.0f;
        int exp=0;
        bool negative=false;
        bool exp_negative=false;
        int i=0;

        while (i<len&&((*this)[i]==' '||(*this)[i]=='\t')) i++;

        if (i<len&&((*this)[i]=='-'||(*this)[i]=='+'))
        {
            negative=((*this)[i]=='-');
            i++;
        }

        while (i<len&&(*this)[i]>='0'&&(*this)[i]<='9')
        {
            result=result*10.0f+static_cast<float>((*this)[i]-'0');
            i++;
        }

        if (i<len&&(*this)[i]=='.')
        {
            i++;
            while (i<len&&(*this)[i]>='0'&&(*this)[i]<='9')
            {
                divisor*=10.0f;
                fraction+=static_cast<float>((*this)[i]-'0')/divisor;
                i++;
            }
        }

        result+=fraction;

        if (i<len&&((*this)[i]=='e'||(*this)[i]=='E'))
        {
            i++;
            if (i<len&&((*this)[i]=='-'||(*this)[i]=='+'))
            {
                exp_negative=((*this)[i]=='-');
                i++;
            }
            while (i<len&&(*this)[i]>='0'&&(*this)[i]<='9')
            {
                exp=exp*10+((*this)[i]-'0');
                i++;
            }
            float power=1.0f;
            for (int j=0;j<exp;j++) power*=10.0f;
            if (exp_negative) result/=power;
            else result*=power;
        }

        return negative?-result:result;
    }

    // 将字符串解析为 int
    [[nodiscard]] int to_int() const
    {
        const int len=size();
        int result=0;
        bool negative=false;
        int i=0;

        while (i<len&&((*this)[i]==' '||(*this)[i]=='\t')) i++;

        if (i<len&&((*this)[i]=='-'||(*this)[i]=='+'))
        {
            negative=((*this)[i]=='-');
            i++;
        }

        while (i<len&&(*this)[i]>='0'&&(*this)[i]<='9')
        {
            result=result*10+((*this)[i]-'0');
            i++;
        }

        return negative?-result:result;
    }

    // 返回以 '\0' 结尾的 C 字符串副本，调用者需自行 delete[] 释放
    [[nodiscard]] char* c_str() const
    {
        const int len=size();
        char* result=new char[len+1];
        for (int i=0;i<len;i++)
        {
            result[i]=(*this)[i];
        }
        result[len]='\0';
        return result;
    }

    // 转换为定长 String（需要在使用时 #include "String.h"）
    // String to_string() const;

private:
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

    // Boyer-Moore 字符串匹配
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

        int bad_char[256];
        for (int& slot : bad_char)
        {
            slot=-1;
        }
        for (int i=0;i<m;i++)
        {
            bad_char[static_cast<unsigned char>(pattern[i])]=i;
        }

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
        int s=0;
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

    static int move_by_good_suffix(const int j,const int m,const int* suffix,const bool* prefix)
    {
        const int k=m-1-j;
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
