#pragma once
#include <cstdint>

#include "../../DataStructures/String.h"

enum TokenType
{
    left_bracket, right_bracket, left_brace, right_brace, colon, comma, string, number
};

class JSONToken
{
public:
    TokenType token_type;
    String str;
    
};
