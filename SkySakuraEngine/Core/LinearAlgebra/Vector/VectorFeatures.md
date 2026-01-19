[< 回到目录](../LinearAlgebra.md)

# 线性代数库——向量`Vector`功能说明

## 数据类型说明

对于这个mini engine，目前对使用的数字类数据类型上的考虑一律从简。即不使用无符号类型以及8个字节、2个字节等的数字类型。故目前使用的数字只有两种类型，即`int`和`float`类型。

对于向量类型的类，参考Eigen库命名，以`i`结尾的类表示这个向量使用`int`类型存储数据，以`f`结尾的类表示这个向量使用`float`类型存储数据。两类向量的运算**不互通**以避免可能的混乱，如果需要运算互通必须进行对象转换。就目前的需求考虑，mini engine的大多数向量都应为浮点型，而整型向量只在特殊情形使用，故目前只支持**从整型向量到浮点型向量的转换**。

## 类实现说明

目前已经做完了下述向量对象：

* [x] [`Vector2i.h`](Vector2i.h)：二阶整型向量
* [x] [`Vector2f.h`](Vector2f.h)：二阶浮点型向量
* [x] [`Vector3i.h`](Vector3i.h)：三阶整型向量
* [x] [`Vector3f.h`](Vector3f.h)：三阶浮点型向量
* [x] [`Vector4i.h`](Vector4i.h)：四阶整型向量（用于齐次坐标）
* [ ] [`Vector4f.h`](Vector4f.h)：四阶浮点型向量（用于齐次坐标）

## 函数实现说明

目前考虑到实际需求，实现下述线性代数的功能。若还需要则后补。之后实现的四维向量也实现如下功能：

---
### 构造函数

* 默认构造函数`Vector()`，此时数值均为0
* 拷贝构造函数`Vector(const Vector& v)`
* 从数值构造函数`Vector(T t1,T t2...)`，
  * 对于三维向量，z值默认置0（即支持由两数构造对象）
  * 对于四维向量，z值与w值默认置0（即支持由两数构造对象与由三数构造对象）

*限浮点型向量：*
* 从整型构造函数`Vectorf(Vectori& vi)`

*限四维向量：*
* 从三维向量构造函数`Vector4(Vector3& v3)`

目前暂不考虑从低维向量向高维的构造，低维向量与高维向量的向量运算也不互通。

---
### `=`重载 

* 默认`=`重载`Vector& operator=(const Vector& v)`

*限浮点型向量：*
* 从整型`=`重载`Vectorf& operator=(Vectori& vi)`

*限四维向量：*
* 从三维向量`=`重载`Vector4(Vector3& v3)`

---
### 成员访问

* x、y成员访问`T& x()`、`T& y()`
* 对于三维向量，z 成员的访问`T& z()`
* 对于四维向量，z 成员的访问`T& z()`与 w 成员的访问`T& w()`

为避免不必要的潜在 bug ，不允许使用`[]`重载进行数组访问。

---
### 四则运算重载

* 向量加减重载`Vector operator+(const Vector& v) const`、`Vector operator-(const Vector& v) const`
* 向量加减赋值重载`void operator+=(const Vector& v)`、`void operator-=(const Vector& v)`
* 向量自取负重载`void operator-()`
* 向量相等与不等重载`bool operator==(const Vector& v) const`、`bool operator!=(const Vector& v) const`
* 向量数乘数除重载（同型重载）`VectorT operator*(const T num) const`、`VectorT operator\(const T num) const`
* 向量友元数乘数除重载（同型重载）`friend VectorT operator*(const T num, const Vector& v)`、`friend VectorT operator\(const T num, const Vector& v)`
  * 即可以实现 a*Vector的运算
* 向量数乘数除赋值重载（同型重载）`void operator*=(const T num)`、`void operator/=(const T num)`
  * 对于数乘数除赋值，不实现友元重载，以免形式上的混乱。

*限浮点型向量：*
* 向量数乘数除重载（从整数运算）`Vectorf operator*(const int num) const`、`Vectorf operator\(const int num) const`
* 向量友元数乘数除重载（从整数运算）`friend Vectorf operator*(const int num, const Vectorf& v)`、`friend Vectorf operator\(const int num, const Vectorf& v)`
* 向量数乘数除赋值重载（从整数运算）`void operator*=(const int num)`、`void operator/=(const int num)`

对于四维函数的四则运算，做出了一定要求：
* 对于加减运算：
  1. 不允许两个点相加（即两个向量的 $w\neq0$ ）
  2. 不允许一个方向减去一个点（即被减数向量的 $w=0$ 而减数向量的 $w\neq0$）
  3. 对于两个点的减法运算，会先标准化再相减，并将 w 值置零
* 对于取等以及不等运算：
  1. 对于两个点的取等以及不等运算，会先标准化再进行比较

---
### 重要向量运算

* 模长之平方（省去开根号以节省性能）`T squaredMagnitude()`
* 点乘`T dot(const VectorT& v) const`
* 叉乘`VectorT cross(const VectorT& v) const`
  * 对于二维向量，仍实现叉乘，返回类型为`Vector3`

*限浮点型向量：*
* 取模运算`float magnitude()`
* 归一化`Vectorf normalized()`

*限二维向量：*

* 叉乘之模长`T crossMagnitude(const VectorT& v)`
  * 考虑到 $xy-yx$ 可能是一个常用运算，以及实现简易，此处先予实现

*限四维向量：*

* 向量为点的判断`bool isPoint() const`
* 向量为方向的判断`bool isDirection() const`
* 对于四维向量的重要向量运算，只允许两个方向间的运算

---