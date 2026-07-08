#pragma once
#include <cstdint>

#include "../../DataStructures/String.h"

enum TokenType
{
    left_bracket,   // [
    right_bracket,  // ]
    left_brace,     // {
    right_brace,    // }
    colon,          // :
    comma,          // ,
    string,         // 字符串字面量
    number,         // 数字字面量
    bool_literal,   // true / false
    null_literal    // null
};

class JSONToken
{
public:
    TokenType token_type;
    String str;
    
};
