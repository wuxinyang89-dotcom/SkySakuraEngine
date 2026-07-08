#pragma once
#include <stdexcept>

#include "../../DataStructures/List.h"
#include "../../DataStructures/String.h"

// JSON 值的类型标记
enum JValueType
{
    json_null, json_bool, json_int, json_float, json_string, json_array, json_object
};

// JSON 文档对象模型（DOM）节点。
// 采用类型标记 + 联合语义：一个 JObject 依据 type_ 表示 6 种 JSON 值之一。
// 数组用 array_value_ 存储；对象用 keys_ 与 values_ 两个平行数组保序存储。
// 子容器统一使用裸指针 + 手动 RAII 管理，与引擎其他数据结构风格保持一致。
class JObject
{
public:
    JObject()
    {
        type_=json_null;
        bool_value_=false;
        int_value_=0;
        float_value_=0;
        array_value_=nullptr;
        keys_=nullptr;
        values_=nullptr;
    }
    
    JObject(const JObject& other)
    {
        array_value_=nullptr;
        keys_=nullptr;
        values_=nullptr;
        copy_from_(other);
    }
    
    JObject(JObject&& other) noexcept
    {
        move_from_(other);
    }
    
    JObject& operator=(const JObject& other)
    {
        if (this!=&other)
        {
            release_();
            copy_from_(other);
        }
        return *this;
    }
    
    JObject& operator=(JObject&& other) noexcept
    {
        if (this!=&other)
        {
            release_();
            move_from_(other);
        }
        return *this;
    }
    
    ~JObject()
    {
        release_();
    }
    
    // ---------------- 工厂函数 ----------------
    
    static JObject make_null()
    {
        return {};
    }
    
    static JObject make_bool(const bool value)
    {
        JObject node;
        node.type_=json_bool;
        node.bool_value_=value;
        return node;
    }
    
    static JObject make_int(const int value)
    {
        JObject node;
        node.type_=json_int;
        node.int_value_=value;
        return node;
    }
    
    static JObject make_float(const float value)
    {
        JObject node;
        node.type_=json_float;
        node.float_value_=value;
        return node;
    }
    
    static JObject make_string(const String& value)
    {
        JObject node;
        node.type_=json_string;
        node.string_value_=value;
        return node;
    }
    
    static JObject make_array()
    {
        JObject node;
        node.type_=json_array;
        node.array_value_=new List<JObject>();
        node.array_value_->clear();
        return node;
    }
    
    static JObject make_object()
    {
        JObject node;
        node.type_=json_object;
        node.keys_=new List<String>();
        node.keys_->clear();
        node.values_=new List<JObject>();
        node.values_->clear();
        return node;
    }
    
    // ---------------- 类型判定 ----------------
    
    [[nodiscard]] JValueType type() const
    {
        return type_;
    }
    
    [[nodiscard]] bool is_null() const
    {
        return type_==json_null;
    }
    
    [[nodiscard]] bool is_bool() const
    {
        return type_==json_bool;
    }
    
    [[nodiscard]] bool is_int() const
    {
        return type_==json_int;
    }
    
    [[nodiscard]] bool is_float() const
    {
        return type_==json_float;
    }
    
    [[nodiscard]] bool is_number() const
    {
        return type_==json_int||type_==json_float;
    }
    
    [[nodiscard]] bool is_string() const
    {
        return type_==json_string;
    }
    
    [[nodiscard]] bool is_array() const
    {
        return type_==json_array;
    }
    
    [[nodiscard]] bool is_object() const
    {
        return type_==json_object;
    }
    
    // ---------------- 取值 ----------------
    
    [[nodiscard]] bool as_bool() const
    {
        if (type_!=json_bool)
        {
            throw std::runtime_error("JObject is not a bool!");
        }
        return bool_value_;
    }
    
    [[nodiscard]] int as_int() const
    {
        if (type_==json_int)
        {
            return int_value_;
        }
        if (type_==json_float)
        {
            return static_cast<int>(float_value_);
        }
        throw std::runtime_error("JObject is not a number!");
    }
    
    [[nodiscard]] float as_float() const
    {
        if (type_==json_float)
        {
            return float_value_;
        }
        if (type_==json_int)
        {
            return static_cast<float>(int_value_);
        }
        throw std::runtime_error("JObject is not a number!");
    }
    
    [[nodiscard]] String as_string() const
    {
        if (type_!=json_string)
        {
            throw std::runtime_error("JObject is not a string!");
        }
        return string_value_;
    }
    
    // ---------------- 数组操作 ----------------
    
    void push_back(const JObject& value)
    {
        if (type_!=json_array)
        {
            throw std::runtime_error("JObject is not an array!");
        }
        array_value_->push_back(value);
    }
    
    JObject& operator[](const int index)
    {
        if (type_!=json_array)
        {
            throw std::runtime_error("JObject is not an array!");
        }
        return (*array_value_)[index];
    }
    
    // ---------------- 对象操作 ----------------
    
    void insert(const String& key,const JObject& value)
    {
        if (type_!=json_object)
        {
            throw std::runtime_error("JObject is not an object!");
        }
        const int index=find_key_(key);
        if (index>=0)
        {
            (*values_)[index]=value;
            return;
        }
        keys_->push_back(key);
        values_->push_back(value);
    }
    
    void insert(const char* key,const JObject& value)
    {
        insert(String(key),value);
    }
    
    [[nodiscard]] bool contains(const String& key) const
    {
        return type_==json_object&&find_key_(key)>=0;
    }
    
    JObject& operator[](const String& key)
    {
        if (type_!=json_object)
        {
            throw std::runtime_error("JObject is not an object!");
        }
        const int index=find_key_(key);
        if (index>=0)
        {
            return (*values_)[index];
        }
        keys_->push_back(key);
        values_->push_back(JObject());
        return (*values_)[values_->size()-1];
    }
    
    JObject& operator[](const char* key)
    {
        return (*this)[String(key)];
    }
    
    // 键的数量（对象）或元素数量（数组）
    [[nodiscard]] int size() const
    {
        if (type_==json_array)
        {
            return array_value_->size();
        }
        if (type_==json_object)
        {
            return keys_->size();
        }
        throw std::runtime_error("JObject is neither an array nor an object!");
    }
    
    // 按下标访问对象的键（用于遍历）
    [[nodiscard]] String key_at(const int index) const
    {
        if (type_!=json_object)
        {
            throw std::runtime_error("JObject is not an object!");
        }
        return (*keys_)[index];
    }
    
    // ---------------- 序列化 ----------------
    
    // 紧凑输出
    [[nodiscard]] String to_string() const
    {
        return build_(false,0,0);
    }
    
    // 带缩进的美化输出，indent_width 为每层缩进的空格数
    [[nodiscard]] String to_string(const int indent_width) const
    {
        return build_(true,indent_width,0);
    }
private:
    JValueType type_;
    bool bool_value_;
    int int_value_;
    float float_value_;
    String string_value_;
    List<JObject>* array_value_;// json_array 时有效
    List<String>* keys_;        // json_object 的键
    List<JObject>* values_;     // json_object 的值
    
    // 释放全部子容器并复位为 null
    void release_()
    {
        delete array_value_;
        delete keys_;
        delete values_;
        array_value_=nullptr;
        keys_=nullptr;
        values_=nullptr;
    }
    
    // 深拷贝 other 的内容（调用前当前对象的指针应已释放或为空）
    void copy_from_(const JObject& other)
    {
        type_=other.type_;
        bool_value_=other.bool_value_;
        int_value_=other.int_value_;
        float_value_=other.float_value_;
        string_value_=other.string_value_;
        array_value_=nullptr;
        keys_=nullptr;
        values_=nullptr;
        if (other.array_value_!=nullptr)
        {
            array_value_=new List<JObject>(*other.array_value_);
        }
        if (other.keys_!=nullptr)
        {
            keys_=new List<String>(*other.keys_);
        }
        if (other.values_!=nullptr)
        {
            values_=new List<JObject>(*other.values_);
        }
    }
    
    // 窃取 other 的资源并将其复位为 null
    void move_from_(JObject& other)
    {
        type_=other.type_;
        bool_value_=other.bool_value_;
        int_value_=other.int_value_;
        float_value_=other.float_value_;
        string_value_=other.string_value_;
        array_value_=other.array_value_;
        keys_=other.keys_;
        values_=other.values_;
        other.type_=json_null;
        other.array_value_=nullptr;
        other.keys_=nullptr;
        other.values_=nullptr;
    }
    
    // 在对象的键数组中查找 key，返回下标；不存在返回 -1
    [[nodiscard]] int find_key_(const String& key) const
    {
        if (keys_==nullptr)
        {
            return -1;
        }
        for (int i=0;i<keys_->size();i++)
        {
            if ((*keys_)[i]==key)
            {
                return i;
            }
        }
        return -1;
    }
    
    // 生成 count 个空格
    static String make_indent_(const int count)
    {
        String result;
        for (int i=0;i<count;i++)
        {
            result.append(" ");
        }
        return result;
    }
    
    // 对字符串内容做 JSON 转义（不含外层引号）
    static String escape_(const String& raw)
    {
        String result;
        for (int i=0;i<raw.size();i++)
        {
            const char c=raw[i];
            switch (c)
            {
            case '\"':
                result.append("\\\"");
                break;
            case '\\':
                result.append("\\\\");
                break;
            case '\n':
                result.append("\\n");
                break;
            case '\r':
                result.append("\\r");
                break;
            case '\t':
                result.append("\\t");
                break;
            case '\b':
                result.append("\\b");
                break;
            case '\f':
                result.append("\\f");
                break;
            default:
                {
                    char tmp[2]={c,'\0'};
                    result.append(tmp);
                }
                break;
            }
        }
        return result;
    }
    
    // 递归构建 JSON 文本
    String build_(const bool pretty,const int indent_width,const int depth) const
    {
        switch (type_)
        {
        case json_null:
            return String("null");
        case json_bool:
            return String(bool_value_?"true":"false");
        case json_int:
            return String::from_int(int_value_);
        case json_float:
            return String::from_float(float_value_);
        case json_string:
            return String("\"")+escape_(string_value_)+"\"";
        case json_array:
            return build_array_(pretty,indent_width,depth);
        case json_object:
            return build_object_(pretty,indent_width,depth);
        }
        return String("null");
    }
    
    String build_array_(const bool pretty,const int indent_width,const int depth) const
    {
        const int count=array_value_->size();
        if (count==0)
        {
            return String("[]");
        }
        String result("[");
        for (int i=0;i<count;i++)
        {
            if (pretty)
            {
                result.append("\n");
                result.append(make_indent_(indent_width*(depth+1)));
            }
            result.append((*array_value_)[i].build_(pretty,indent_width,depth+1));
            if (i<count-1)
            {
                result.append(",");
            }
        }
        if (pretty)
        {
            result.append("\n");
            result.append(make_indent_(indent_width*depth));
        }
        result.append("]");
        return result;
    }
    
    String build_object_(const bool pretty,const int indent_width,const int depth) const
    {
        const int count=keys_->size();
        if (count==0)
        {
            return String("{}");
        }
        String result("{");
        for (int i=0;i<count;i++)
        {
            if (pretty)
            {
                result.append("\n");
                result.append(make_indent_(indent_width*(depth+1)));
            }
            result.append("\"");
            result.append(escape_((*keys_)[i]));
            result.append("\"");
            result.append(pretty?": ":":");
            result.append((*values_)[i].build_(pretty,indent_width,depth+1));
            if (i<count-1)
            {
                result.append(",");
            }
        }
        if (pretty)
        {
            result.append("\n");
            result.append(make_indent_(indent_width*depth));
        }
        result.append("}");
        return result;
    }
};
