# JSONUtility 开发进度

## 模块概述
提供 JSON 序列化/反序列化功能，全部为 header-only 实现。

## 文件结构
- `JSONConvertor.h` — 入口工具类，提供 `ConvertToNode`（反序列化）和 `ConvertToString`（序列化）静态接口，内部使用递归下降解析器。
- `JSONTokenizer.h` — 词法分析器，将 JSON 文本拆分为 token 列表。
- `Tokens/JSONToken.h` — Token 数据结构定义。
- `Objects/JObject.h` — JSON DOM 节点类，支持 null / bool / int / float / string / array / object 类型。

## 当前状态
- 所有文件均为 header-only，无 .cpp 文件。
- 支持完整的 JSON 解析与生成（含缩进美化输出）。
- 错误处理通过 `std::runtime_error` 抛出异常。
