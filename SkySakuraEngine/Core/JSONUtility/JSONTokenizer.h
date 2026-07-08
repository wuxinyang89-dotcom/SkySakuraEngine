#pragma once
#include <stdexcept>

#include "Tokens/JSONToken.h"
#include "../DataStructures/List.h"
#include "../DataStructures/String.h"

// 词法分析器：将 JSON 文本切分为 JSONToken 序列
class JSONTokenizer
{
public:
    static List<JSONToken> Tokenize(const String& json_str)
    {
        List<JSONToken> tokens;
        int index=0;
        const int length=json_str.size();
        while (index<length)
        {
            const char c=json_str[index];
            if (c==' '||c=='\t'||c=='\n'||c=='\r')
            {
                index++;
                continue;
            }
            
            JSONToken token;
            switch (c)
            {
            case '[':
                token.token_type=left_bracket;
                tokens.push_back(token);
                index++;
                break;
            case ']':
                token.token_type=right_bracket;
                tokens.push_back(token);
                index++;
                break;
            case '{':
                token.token_type=left_brace;
                tokens.push_back(token);
                index++;
                break;
            case '}':
                token.token_type=right_brace;
                tokens.push_back(token);
                index++;
                break;
            case ':':
                token.token_type=colon;
                tokens.push_back(token);
                index++;
                break;
            case ',':
                token.token_type=comma;
                tokens.push_back(token);
                index++;
                break;
            case '\"':
                read_string_(json_str,index,length,token);
                tokens.push_back(token);
                break;
            default:
                if (c=='-'||(c>='0'&&c<='9'))
                {
                    read_number_(json_str,index,length,token);
                    tokens.push_back(token);
                }
                else if (c=='t'||c=='f')
                {
                    read_bool_(json_str,index,length,token);
                    tokens.push_back(token);
                }
                else if (c=='n')
                {
                    read_null_(json_str,index,length,token);
                    tokens.push_back(token);
                }
                else
                {
                    throw std::runtime_error("Unexpected character in JSON!");
                }
                break;
            }
        }
        return tokens;
    }
private:
    static void append_char_(String& s,const char c)
    {
        char tmp[2]={c,'\0'};
        s.append(tmp);
    }
    
    // 读取字符串字面量（index 指向起始引号），结果为解转义后的内容
    static void read_string_(const String& json_str,int& index,const int length,JSONToken& token)
    {
        index++;// 跳过起始引号
        String value;
        while (index<length&&json_str[index]!='\"')
        {
            const char ch=json_str[index];
            if (ch=='\\')
            {
                index++;
                if (index>=length)
                {
                    throw std::runtime_error("Unterminated escape in JSON string!");
                }
                const char esc=json_str[index];
                switch (esc)
                {
                case '\"':
                    append_char_(value,'\"');
                    break;
                case '\\':
                    append_char_(value,'\\');
                    break;
                case '/':
                    append_char_(value,'/');
                    break;
                case 'n':
                    append_char_(value,'\n');
                    break;
                case 't':
                    append_char_(value,'\t');
                    break;
                case 'r':
                    append_char_(value,'\r');
                    break;
                case 'b':
                    append_char_(value,'\b');
                    break;
                case 'f':
                    append_char_(value,'\f');
                    break;
                default:
                    throw std::runtime_error("Unsupported escape sequence in JSON string!");
                }
                index++;
            }
            else
            {
                append_char_(value,ch);
                index++;
            }
        }
        if (index>=length)
        {
            throw std::runtime_error("Unterminated JSON string!");
        }
        index++;// 跳过结束引号
        token.token_type=string;
        token.str=value;
    }
    
    // 读取数字字面量，原样保留文本供后续判定 int / float
    static void read_number_(const String& json_str,int& index,const int length,JSONToken& token)
    {
        const int start=index;
        if (json_str[index]=='-')
        {
            index++;
        }
        while (index<length)
        {
            const char ch=json_str[index];
            if ((ch>='0'&&ch<='9')||ch=='.'||ch=='e'||ch=='E'||ch=='+'||ch=='-')
            {
                index++;
            }
            else
            {
                break;
            }
        }
        token.token_type=number;
        token.str=json_str.substr(start,index);
    }
    
    static void read_bool_(const String& json_str,int& index,const int length,JSONToken& token)
    {
        if (index+4<=length&&json_str[index]=='t'&&json_str[index+1]=='r'&&json_str[index+2]=='u'&&json_str[index+3]=='e')
        {
            token.token_type=bool_literal;
            token.str=String("true");
            index+=4;
            return;
        }
        if (index+5<=length&&json_str[index]=='f'&&json_str[index+1]=='a'&&json_str[index+2]=='l'&&json_str[index+3]=='s'&&json_str[index+4]=='e')
        {
            token.token_type=bool_literal;
            token.str=String("false");
            index+=5;
            return;
        }
        throw std::runtime_error("Invalid boolean literal in JSON!");
    }
    
    static void read_null_(const String& json_str,int& index,const int length,JSONToken& token)
    {
        if (index+4<=length&&json_str[index]=='n'&&json_str[index+1]=='u'&&json_str[index+2]=='l'&&json_str[index+3]=='l')
        {
            token.token_type=null_literal;
            token.str=String("null");
            index+=4;
            return;
        }
        throw std::runtime_error("Invalid null literal in JSON!");
    }
};
