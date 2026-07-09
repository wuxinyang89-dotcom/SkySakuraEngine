[< 回到核心层目录](../Core.md)

# 高性能数据结构说明

## 内容进度备忘

- [X] 定长数组[`Array.h`](#定长数组arrayh)
- [X] 变长数组[`List.h`](./List.h)
- [X] 栈[`Stack.h`](./Stack.h)
- [X] 队列[`Queue.h`](./Queue.h)
- [X] 优先队列（大根堆小根堆）[`PriorityQueue.h`](./PriorityQueue.h)
  - [X] 默认比较模板类[`Greater.h`](./Compare/Greater.h)、[`Less.h`](./Compare/Less.h)
- [X] 字典（无序哈希表）[`Map.h`](./Map.h)
  - [X] 封装的键值对类[`DictionaryValue.h`](./Dictionary/DictionaryValue.h)
- [X] 集合（无序单键哈希表）[`Set.h`](./Set.h)
  - [X] 封装的键值类[`SetValue.h`](./Dictionary/SetValue.h)
- [X] 定长字符串[`String.h`](./String.h)
  - 用于静态文本，拼接开销大。
  - 使用 Boyer-Moore 算法实现字符串匹配。
  - 已实现 `to_float()`、`to_int()` 数值解析方法。
  - 已实现基础 UTF-8 `next()` 导航。
  - 待定：SSO 小字符串优化、更完善的 Unicode 支持。
- [X] 变长字符串[`DynamicString.h`](./DynamicString.h)
  - 用于动态文本，拼接开销小（均摊 O(1)）。对标C#的`StringBuilder`。
  - 已实现完整 API：`append`、`find`、`substr`、`to_float`、`to_int`、`operator+/+=`。

有序哈希表由于实现极其复杂，暂时不做。

---

## 数据结构说明

### 定长数组`Array.h`

*跳转至：[`./Array.h`](./Array.h)*

该数据结构为一个定长数组，即在堆上申请一个初始容量为固定长度的数组，在后续的使用中不允许更改数组的容量。相当于C++的原生数组，而在此将其对象化。

**成员变量：**

```c++
T* data_; // 指向堆上数组的指针
int size_; // 数组的大小
```

**已实现功能：** 构造/析构、拷贝/移动语义、`operator[]`访问。

---

### 变长数组`List.h`

*跳转至：[`./List.h`](./List.h)*

基于 `Array<T>` 的动态数组，支持自动扩容（2倍策略）。对标 C# 的 `List<T>` / C++ 的 `std::vector<T>`。

**成员变量：**

```c++
int count_; // 当前元素数量（区别于底层容量 size_）
```

**已实现功能：** `add`、`remove_at`、`insert`、`clear`、自动扩缩容。

---

### 栈`Stack.h`

*跳转至：[`./Stack.h`](./Stack.h)*

基于 `List<T>` 的 LIFO 栈。

**已实现功能：** `push`、`pop`、`peek`、`count`。

---

### 队列`Queue.h`

*跳转至：[`./Queue.h`](./Queue.h)*

基于 `List<T>` 的 FIFO 队列。

**已实现功能：** `enqueue`、`dequeue`、`peek`、`count`。

---

### 优先队列`PriorityQueue.h`

*跳转至：[`./PriorityQueue.h`](./PriorityQueue.h)*

基于二叉堆的优先队列，通过模板参数 `Compare` 决定为大根堆或小根堆。

**已实现功能：** `push`、`pop`、`peek`、堆上浮/下沉。

**辅助类：** [`Greater.h`](./Compare/Greater.h)、[`Less.h`](./Compare/Less.h)

---

### 字典`Map.h`

*跳转至：[`./Map.h`](./Map.h)*

无序哈希表，使用开放寻址法解决冲突。

**已实现功能：** `set`、`get`、`remove`、`contains_key`、自动rehash。

**辅助类：** [`DictionaryValue.h`](./Dictionary/DictionaryValue.h)（键值对封装）

---

### 集合`Set.h`

*跳转至：[`./Set.h`](./Set.h)*

无序单键哈希表，仅存储键。

**已实现功能：** `add`、`remove`、`contains`、自动rehash。

**辅助类：** [`SetValue.h`](./Dictionary/SetValue.h)（键封装）

---

### 定长字符串`String.h`

*跳转至：[`./String.h`](./String.h)*

基于 `Array<char>` 的定长字符串，创建后容量不可变，拼接会产生新对象。

**已实现功能：**

| 方法 | 说明 |
|------|------|
| `append` | 拼接（返回新字符串） |
| `substr(start, end)` | 子串截取 |
| `find(pattern)` | Boyer-Moore 字符串匹配 |
| `next(index)` | UTF-8 码点导航 |
| `to_float()` | 解析为 `float`（支持小数与科学计数法） |
| `to_int()` | 解析为 `int` |
| `c_str()` | 转 C 风格字符串（调用者负责释放） |
| `operator+` | 拼接运算符 |

---

### 变长字符串`DynamicString.h`

*跳转至：[`./DynamicString.h`](./DynamicString.h)*

基于 `List<char>` 的动态字符串，对标 C# 的 `StringBuilder`。利用 `List` 的 2 倍扩容策略，`append` 为均摊 O(1) 操作，适合频繁拼接场景。

**已实现功能：**

| 方法 | 说明 |
|------|------|
| `append(const char*)` | 追加 C 字符串（原地修改） |
| `append(const DynamicString&)` | 追加另一个动态字符串 |
| `append(char)` | 追加单个字符 |
| `substr(start, end)` | 子串截取 |
| `find(pattern)` | Boyer-Moore 字符串匹配 |
| `next(index)` | UTF-8 码点导航 |
| `to_float()` | 解析为 `float`（支持小数与科学计数法） |
| `to_int()` | 解析为 `int` |
| `c_str()` | 转 C 风格字符串（调用者负责释放） |
| `operator+` | 拼接运算符（返回新对象） |
| `operator+=` | 原地追加运算符 |