[< 回到核心层目录](../Core.md)

# 高性能数据结构说明

目前，对于每个数据结构的功能说明属于待补的状态。

## 内容进度备忘

- [X] 定长数组`Array.h`
- [X] 变长数组`List.h`
- [X] 栈`Stack.h`
- [X] 队列`Queue.h`
- [X] 优先队列（大根堆小根堆）`PriorityQueue.h`
  - [X] 默认比较模板类`Greater.h`、`Less.h`
- [X] 字典（无序哈希表）`Map.h`
  - [X] 封装的键值对类`DictionaryValue.h`
- [X] 集合（无序单键哈希表）`Set.h`
  - [X] 封装的键值类`SetValue.h`
- [ ] 定长字符串`String.h`
  - 用于静态文本，拼接开销大 
  - 缺 KMP 匹配算法的实现。同时正在考虑是否实现对于小字符串的SSO机制
- [ ] 变长字符串`DynamicString.h`
  - 用于动态文本，拼接开销小。对标C#的`StringBuilder`
  - 未做

有序哈希表由于实现极其复杂，暂时不做。

## 数据结构说明

待补