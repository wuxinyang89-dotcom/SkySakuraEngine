[< 回到核心层目录](../Core.md)

# 高性能数据结构说明

目前，对于每个数据结构的功能说明属于待补的状态。

## 内容进度备忘

- [X] [定长数组`Array.h`](#定长数组arrayh)
- [X] 变长数组[`List.h`](./List.h)
- [X] 栈[`Stack.h`](./Stack.h)
- [X] 队列[`Queue.h`](./Queue.h)
- [X] 优先队列（大根堆小根堆）[`PriorityQueue.h`](./PriorityQueue.h)
  - [X] 默认比较模板类[`Greater.h`](./Compare/Greater.h)、[`Less.h`](./Compare/Less.h)
- [X] 字典（无序哈希表）[`Map.h`](./Map.h)
  - [X] 封装的键值对类[`DictionaryValue.h`](./Dictionary/DictionaryValue.h)
- [X] 集合（无序单键哈希表）[`Set.h`](./Set.h)
  - [X] 封装的键值类[`SetValue.h`](./Dictionary/SetValue.h)
- [ ] 定长字符串[`String.h`](./String.h)
  - 用于静态文本，拼接开销大 
  - 简单实现了 KMP 匹配算法，但还需要检查。同时正在考虑是否实现对于小字符串的SSO机制
- [ ] 变长字符串[`DynamicString.h`](./DynamicString.h)
  - 用于动态文本，拼接开销小。对标C#的`StringBuilder`
  - 未做

有序哈希表由于实现极其复杂，暂时不做。

## 数据结构说明

---

# 定长数组`Array.h`

*跳转至：[`./Array.h`](./Array.h)*

该数据结构为一个定长数组，即在堆上申请一个初始容量为固定长度的数组，在后续的使用中不允许更改数组的容量。相当于C++的原生数组，而在此将其对象化。

## 变量说明

```c++
T* data_;//指向堆上数组的指针
int size_;//数组的大小
```

## 函数说明

### 构造函数与析构函数

### `=`重载函数

### 数组工具函数

### 工具函数