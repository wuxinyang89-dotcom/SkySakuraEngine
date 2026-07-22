#pragma once

// 标记一个类需要生成反射信息
// 用法: SCLASS() 放在 class 声明上方
#define SCLASS()

// 标记一个属性需要反射
// 用法: SPROPERTY() 放在成员变量上方
#define SPROPERTY()

// 标记一个函数需要反射
// 用法: SFUNCTION() 放在成员函数上方
#define SFUNCTION()

// 生成体宏 - 由解析器在生成文件中展开为实际反射代码
// 用法: GENERATED_BODY() 放在类体内部的第一行
#define GENERATED_BODY()

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
