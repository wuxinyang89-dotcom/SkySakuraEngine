#pragma once
#include "Objects/JObject.h"

class JSONConvertor
{
public:
    static JObject ConvertToNode(String jsonStr);
};
