#pragma once
#include "Objects/JObject.h"

class JSONConvertor
{
public:
    // 反序列化：JSON 文本 -> JObject
    static JObject ConvertToNode(String jsonStr);
    
    // 序列化：JObject -> JSON 文本（紧凑）
    static String ConvertToString(const JObject& node);
    
    // 序列化：JObject -> JSON 文本（带缩进美化，indent_width 为每层缩进空格数）
    static String ConvertToString(const JObject& node,int indent_width);
};
