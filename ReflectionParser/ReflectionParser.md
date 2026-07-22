# ReflectionParser

## 概述
ReflectionParser 是一个基于 .NET 的反射代码生成工具，用于解析 C++ 头文件中的反射宏（SCLASS/SPROPERTY/SFUNCTION）并生成对应的 `.generated.h` 文件。

## 开发进度
- 目标框架：`net9.0`（支持无 VS2026 的远程协作机器）
- 当前 SDK 版本：.NET 9.0.316
- **Program.cs** — 控制台入口，实现 4 步显性流程：确定根目录 → 扫描头文件 → 输出结果 → 生成代码（待实现）
- **HeaderScanner.cs** — 头文件扫描器，递归搜索项目目录中的 `.h` 文件，筛选出包含反射宏的文件（自动排除 `.generated.h`、宏定义文件等）

## 文件结构
```
ReflectionParser/
├── Program.cs          # 控制台入口与流程控制
├── HeaderScanner.cs    # 头文件扫描与筛选
├── ReflectionParser.csproj
└── ReflectionParser.md
```

## 环境要求
- .NET 9.0 SDK 或更高版本

## 使用方式
```bash
dotnet run                          # 自动从当前目录向上查找 .sln
dotnet run -- <solution_root_path>  # 指定解决方案根目录
```
