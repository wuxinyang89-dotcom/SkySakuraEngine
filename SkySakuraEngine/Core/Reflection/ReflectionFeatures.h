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

// 标记一个枚举需要反射
// 用法: SENUM() 放在成员枚举上方
#define SENUM()

// 生成体宏 - 由解析器在生成文件中展开为实际反射代码
// 用法: GENERATED_BODY() 放在类体内部的第一行
#define GENERATED_BODY()

enum PackageType{
    Public,Private,Protected    
};

// 属性元信息
struct PropertyInfo
{
    bool isStatic;
    const char* name;
    const char* typeName;
    PackageType packageType;
    size_t offset;
    size_t size;
};

// 参数表信息
struct ParamInfo
{
    const char* const* paramTypeName;    // 参数类型列表
    int paramCount;                // 参数数量
};

// 函数元信息
struct FunctionInfo
{
    const char* name;
    const char* const* returnType;
    const ParamInfo* paramInfo;
    int reloadCount;
    PackageType packageType;
};

// 继承信息
struct InheritanceInfo
{
    const char* const* parentClassName;    // 基类名字列表
    int parentCount;                // 基类数量（多继承时 > 1）
};

// 类元信息
struct ClassInfo
{
    const char* namespaceName;
    const char* className;
    size_t classSize;
    const PropertyInfo* properties;
    int propertyCount;
    const FunctionInfo* functions;
    int functionCount;
    InheritanceInfo inheritance;
};

// 枚举信息
struct EnumInfo
{
    const char* namespaceName;
    const char* enumName;
    const char* enumTypeName;
    int enumCount;
};


