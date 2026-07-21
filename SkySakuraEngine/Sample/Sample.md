# Sample - 反射系统示例

## 概述
本文件夹包含反射系统的示例代码，展示了反射宏解析器的输入与输出关系。

## 文件说明
- `ReflectionMacros.h` - 反射宏定义（SCLASS、SPROPERTY、SFUNCTION、GENERATED_BODY）
- `SampleActor.h` - 带反射标记的示例原类（解析器的输入）
- `SampleActor.generated.h` - 解析器生成的反射信息文件（解析器的输出）

## 工作流程
1. 用户在类上标记 `SCLASS()`，在属性上标记 `SPROPERTY()`，在函数上标记 `SFUNCTION()`
2. 类内部放置 `GENERATED_BODY()` 宏
3. ReflectionParser 扫描头文件，生成 `.generated.h` 文件
4. 生成文件包含类的元信息（属性偏移、类型名、函数签名等）
5. `GENERATED_BODY()` 被重定义为实际的反射访问接口

## 开发进度
- [x] 反射宏定义
- [x] 示例原类编写
- [x] 示例生成文件编写
- [ ] ReflectionParser 解析器实现（在 ReflectionParser 项目中）
