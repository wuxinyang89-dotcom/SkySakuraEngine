[< 回到目录](../LinearAlgebra.md)

# 线性代数库——矩阵`Matrix`功能说明

## 数据类型说明

对于这个mini engine，目前对使用的数字类数据类型上的考虑一律从简。即不使用无符号类型以及8个字节、2个字节等的数字类型。故目前使用的数字只有两种类型，即`int`和`float`类型。

对于矩阵类型的类，参考Eigen库命名，以`i`结尾的类表示这个矩阵使用`int`类型存储数据，以`f`结尾的类表示这个矩阵使用`float`类型存储数据。两类矩阵的运算**不互通**以避免可能的混乱，如果需要运算互通必须进行对象转换。就目前的需求考虑，mini engine的大多数矩阵都应为浮点型，而整型矩阵只在特殊情形使用，故目前只支持**从整型向量到浮点型向量的转换**。

## 类实现说明

目前已经做完了下述矩阵对象：

* [x] [`Matrix2i.h`](./Matrix2i.h)：二阶整型矩阵
* [x] [`Matrix2f.h`](./Matrix2f.h)：二阶浮点型矩阵
* [ ] [`Matrix3i.h`](./Matrix3i.h)：三阶整型矩阵
* [ ] [`Matrix3f.h`](./Matrix3f.h)：三阶浮点型矩阵
* [ ] [`Matrix4i.h`](./Matrix4i.h)：四阶整型矩阵（用于齐次坐标）
* [ ] [`Matrix4f.h`](./Matrix4f.h)：四阶浮点型矩阵（用于齐次坐标）

## 函数实现说明

### 构造函数

* 默认构造函数`Matrix()`，所有数值全部置零
* 拷贝构造函数`Matrix(const Matrix& m)`
* 从花括号列表（如`{1,2,3,4}`）构造函数：
  * 从一阶花括号列表构造函数`Matrix(const std::initializer_list<int>& list)`
  * 从二阶花括号列表构造函数`Matrix(const std::initializer_list<std::initializer_list<int>>& list)`

*限浮点矩阵：*
* 从整型矩阵构造函数`Matrixf(Matrixi& mi)`

---
### `=`重载

* 默认`=`重载`Matrix& operator=(const Matrix& m)`
* 从花括号列表（如`{1,2,3,4}`）的`=`重载：
  * 从一阶花括号列表的`=`重载`Matrix& operator=(const std::initializer_list<int>& list)`
  * 从二阶花括号列表的`=`重载`Matrix& operator=(const std::initializer_list<std::initializer_list<int>>& list)`

*限浮点矩阵：*
* 从整型矩阵的`=`重载`Matrixf& operator=(Matrixi& mi)`

---
### 成员访问

* 成员访问函数`T& at(int row,int col)`

不允许使用`[]`进行矩阵成员访问，以避免可能的 bug 。

---

### 四则运算重载

* 矩阵与向量相乘`Vector operator*(Vector& v) const`
* 矩阵与矩阵相乘`Matrix operator*(const Matrix& m) const`

考虑到矩阵乘法不支持结合律，另行还提供相应的乘法函数：
* 矩阵与向量相乘`Vector multiply(Vector& v) const`
* 矩阵与矩阵相乘`Matrix multiply(const Matrix& m) const`

根据相关运算律，不允许向量乘以一个矩阵，故不设计向量乘以矩阵的重载。

---
### 重要矩阵运算

* 求矩阵的转置`Matrix rotate() const`
* 求矩阵的行列式`T determinant() const`
* 获取一个单位矩阵 $I$ `static Matrix identity()`
* 求矩阵的逆 $M^{-1}$ `Matrix inverse() const`