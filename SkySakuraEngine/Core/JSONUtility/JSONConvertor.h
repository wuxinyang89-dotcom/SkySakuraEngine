#pragma once
#include <stdexcept>

#include "Objects/JObject.h"
#include "JSONTokenizer.h"

class JSONConvertor
{
public:
    // 反序列化：JSON 文本 -> JObject
    static JObject ConvertToNode(String jsonStr)
    {
        List<JSONToken> tokens=JSONTokenizer::Tokenize(jsonStr);
        if (tokens.size()==0)
        {
            throw std::runtime_error("Empty JSON input!");
        }
        int pos=0;
        JObject result=parse_value(tokens,pos);
        if (pos!=tokens.size())
        {
            throw std::runtime_error("Extra content after JSON value!");
        }
        return result;
    }
    
    // 序列化：JObject -> JSON 文本（紧凑）
    static String ConvertToString(const JObject& node)
    {
        return node.to_string();
    }
    
    // 序列化：JObject -> JSON 文本（带缩进美化，indent_width 为每层缩进空格数）
    static String ConvertToString(const JObject& node,const int indent_width)
    {
        return node.to_string(indent_width);
    }

private:
    // 递归下降解析。pos 为当前待处理的 token 下标，处理后前移。
    static JObject parse_value(const List<JSONToken>& tokens,int& pos)
    {
        if (pos>=tokens.size())
        {
            throw std::runtime_error("Unexpected end of tokens!");
        }
        const JSONToken& token=tokens[pos];
        switch (token.token_type)
        {
        case null_literal:
            pos++;
            return JObject::make_null();
        case bool_literal:
            {
                const bool value=token.str==String("true");
                pos++;
                return JObject::make_bool(value);
            }
        case number:
            {
                String text=token.str;
                pos++;
                if (text.find(".")>=0||text.find("e")>=0||text.find("E")>=0)
                {
                    return JObject::make_float(text.to_float());
                }
                return JObject::make_int(text.to_int());
            }
        case string:
            {
                JObject node=JObject::make_string(token.str);
                pos++;
                return node;
            }
        case left_bracket:
            return parse_array(tokens,pos);
        case left_brace:
            return parse_object(tokens,pos);
        default:
            throw std::runtime_error("Unexpected token in JSON!");
        }
    }
    
    static JObject parse_array(const List<JSONToken>& tokens,int& pos)
    {
        pos++;// 跳过 '['
        JObject node=JObject::make_array();
        if (pos<tokens.size()&&tokens[pos].token_type==right_bracket)
        {
            pos++;
            return node;
        }
        while (true)
        {
            node.push_back(parse_value(tokens,pos));
            if (pos>=tokens.size())
            {
                throw std::runtime_error("Unexpected end of tokens in array!");
            }
            if (tokens[pos].token_type==comma)
            {
                pos++;
                continue;
            }
            if (tokens[pos].token_type==right_bracket)
            {
                pos++;
                break;
            }
            throw std::runtime_error("Expected ',' or ']' in array!");
        }
        return node;
    }
    
    static JObject parse_object(const List<JSONToken>& tokens,int& pos)
    {
        pos++;// 跳过 '{'
        JObject node=JObject::make_object();
        if (pos<tokens.size()&&tokens[pos].token_type==right_brace)
        {
            pos++;
            return node;
        }
        while (true)
        {
            if (pos>=tokens.size()||tokens[pos].token_type!=string)
            {
                throw std::runtime_error("Expected string key in object!");
            }
            String key=tokens[pos].str;
            pos++;
            if (pos>=tokens.size()||tokens[pos].token_type!=colon)
            {
                throw std::runtime_error("Expected ':' in object!");
            }
            pos++;
            node.insert(key,parse_value(tokens,pos));
            if (pos>=tokens.size())
            {
                throw std::runtime_error("Unexpected end of tokens in object!");
            }
            if (tokens[pos].token_type==comma)
            {
                pos++;
                continue;
            }
            if (tokens[pos].token_type==right_brace)
            {
                pos++;
                break;
            }
            throw std::runtime_error("Expected ',' or '}' in object!");
        }
        return node;
    }
};
