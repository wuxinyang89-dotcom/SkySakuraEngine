#pragma once
// ============================================================
// 此文件由 ReflectionParser 自动生成，请勿手动修改
// 源文件: SampleActor.h
// ============================================================

#include <cstddef>
#include <cstring>

// 属性元信息
struct PropertyInfo
{
    const char* name;
    const char* typeName;
    size_t offset;
    size_t size;
};

// 函数元信息
struct FunctionInfo
{
    const char* name;
    const char* returnType;
    int paramCount;
};

// 类元信息
struct ClassInfo
{
    const char* className;
    size_t classSize;
    const PropertyInfo* properties;
    int propertyCount;
    const FunctionInfo* functions;
    int functionCount;
};

// ============================================================
// SampleActor 的反射数据
// ============================================================

namespace Generated_SampleActor
{
    inline constexpr PropertyInfo Properties[] =
    {
        { "health_",   "float",       offsetof(SampleActor, health_),   sizeof(float) },
        { "name_",     "const char*", offsetof(SampleActor, name_),     sizeof(const char*) },
        { "isActive_", "bool",        offsetof(SampleActor, isActive_), sizeof(bool) },
    };

    inline constexpr FunctionInfo Functions[] =
    {
        { "TakeDamage", "void",  1 },
        { "GetHealth",  "float", 0 },
        { "SetName",    "void",  1 },
    };

    inline constexpr ClassInfo StaticClass =
    {
        "SampleActor",
        sizeof(SampleActor),
        Properties,
        3,
        Functions,
        3,
    };
}

// GENERATED_BODY() 宏展开内容
// 重定义 GENERATED_BODY 使其在 SampleActor 类内展开为以下代码
#undef GENERATED_BODY
#define GENERATED_BODY()                                                        \
public:                                                                         \
    static const ClassInfo* GetStaticClass()                                    \
    {                                                                           \
        return &Generated_SampleActor::StaticClass;                             \
    }                                                                           \
    const ClassInfo* GetClass() const                                           \
    {                                                                           \
        return GetStaticClass();                                                \
    }                                                                           \
    /* 通过属性名获取属性值的指针 */                                              \
    void* GetPropertyPtr(const char* propName)                                  \
    {                                                                           \
        auto* info = GetStaticClass();                                          \
        for (int i = 0; i < info->propertyCount; ++i)                           \
        {                                                                       \
            if (strcmp(info->properties[i].name, propName) == 0)                 \
            {                                                                   \
                return reinterpret_cast<char*>(this) + info->properties[i].offset;\
            }                                                                   \
        }                                                                       \
        return nullptr;                                                         \
    }                                                                           \
private:                                                                        \
    friend struct Generated_SampleActor;
