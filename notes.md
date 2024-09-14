> 以下均为个人总结
>
> 参考：C++ primer、Effective Modern C++、C++ 并发编程实战、cppreference

# 基础知识

## C++和C

C++ 最早是 C with class，现在 C++ 已经发展成了一个语言联邦，支持多种编程范式，C 是面向过程的，C++ 除了支持面向过程，还支持面向对象和泛型编程

## 基本类型大小

C++ 规定：char 最小 8 位，short 最小 16 位，int 最小 16 位，long 最小 32 位，long long 最小 64 位

float 一般是 IEEE-754 32 位格式，double 一般是 IEEE-754 64 位格式，long double 由实现定义

指针类型的大小是该平台的位宽

## 整数的存储

为了统一正负数的加减法，负数用补码表示，就是把正数的二进制位全部取反再加一

有符号数的最高位是符号位，无符号数的所有位都是数值位

char、signed char、unsigned char 是三个不同的类型，char 的符号性由实现定义

有符号数溢出是未定义行为

无符号数溢出，结果相当于对它能表示的数值总数取模

## IEEE-754

存储方式：符号位 - 指数位 - 尾数位

指数位越长，能表示的数越大

尾数位越长，能表示的精度更高

32 位浮点数：1 - 8 - 23

64 位浮点数：1 - 11 - 52

指数可能是负数，为了统一，指数位保存时要加上一个偏移量（127 / 1023）

科学计数法的整数部分总是 1，所以尾数只保存小数部分，节省了一位空间，提高了一点精度

误差：有些小数转换成二进制是一个无限循环小数，计算机的资源是有限的，只能用近似值来表示

## 字节序

大端：高位字节存低地址，低位字节存高地址

小端：低位字节存低地址，高位字节存高地址

```C++
//判断当前机器的字节序
#include<iostream>

int check() {
    int i = 1;
    return *reinterpret_cast<char*>(&i);
}

int main() {
    std::cout << (check() == 1 ? "small\n" : "big\n");
}
```

## 内存对齐

内存对齐就是数据在内存中的起始地址是某个数的整数倍

每个类型都有自己的对齐要求，对齐要求总是 2 的幂，可以用 alignof 获取、用 alignas 修改（C++11 起）

内存对齐的原因：1. 平台原因 2. 性能原因

## inline

inline 的本意是建议编译器优先内联，就是将函数在调用处展开，避免函数调用的开销，但可能导致更大的可执行文件

C++98 起，inline 的作用变为允许一个函数或变量在不同翻译单元多次定义，前提是必须在这些翻译单元都声明为 inline，比如用来修饰头文件中定义的函数或变量

类中定义的函数是隐式 inline 的

C++17 起，inline 还可以修饰类的静态数据成员，使得静态数据成员可以在类中定义（类似的还有 constexpr）

## 内联函数和宏

这里的内联函数指的是编译器对函数采用内联替换

宏就是文本替换，在预处理阶段完成

区别：宏没有类型检查，内联函数有类型检查

## static

static 修饰局部变量：延长生命周期，首次访问时初始化，只会被初始化一次

static 修饰全局变量或函数：内部链接，可以用来防止命名冲突或者在头文件定义全局变量或函数（类似的还有匿名命名空间）

static 修饰类的数据成员：静态数据成员不与对象绑定，需要单独定义，可以直接用类名访问。static 关键字只用于类中声明

static 修饰类的成员函数：静态成员函数不与对象绑定，没有 this，可以直接用类名访问。static 关键字只用于类中声明

## const

const 表示不能修改，不允许放开修改的权限

const 可以修饰变量、返回值、函数形参、成员函数等

## volatile

volatile 表示容易改变，保证总是从内存中读取

一般用于嵌入式中，与多线程同步无关

## 引用和指针

引用是对象的别名，指针可以存储对象的地址

引用必须初始化，指针没有要求

引用不能改变指向，指针可以

没有多级引用，但有多级指针

没有空引用，但有空指针

## 什么是迭代器

迭代器就是指针的抽象

迭代器给容器提供了统一的遍历方式，并且使得泛型算法不依赖容器

const_iterator 表示指向常量的指针

const iterator 表示指针本身是常量

## #include<>和#include""

`<>` 直接去系统目录查找，一般用于包含标准库头文件

`""` 先在当前工作目录查找，找不到再去系统目录查找，一般用于包含自定义头文件

头文件包含就是将头文件的内容直接复制进来

## 头文件重复包含

头文件重复包含是指头文件在一个翻译单元中多次包含

可以使用条件编译（`#ifndef` `#define` `#endif`）或 `#pragma once` 来避免重复包含

## extern"C"

`extern"C"` 是一个链接指示，告诉编译器按照 C 的方式处理符号，否则链接 C 库的时候会出问题

这是因为 C++ 的符号修饰一般和 C 不同

## 源文件到可执行文件的过程

每个源文件都是一个编译单元，每个编译单元被独立编译成一个目标文件，再由链接器将所有目标文件链接成一个可执行文件

源文件到可执行文件的过程可以分为四个阶段：

- 预处理：执行预处理指令，比如宏替换、头文件展开、条件编译
- 编译：词法分析、语法分析、语义分析、代码优化，生成汇编代码
- 汇编：将汇编代码翻译成二进制代码，生成目标文件
- 链接：将所有目标文件以及相关的库链接成可执行文件

## 静态链接和动态链接

静态链接会把库代码复制到可执行文件

- 优点：运行时不需要外部库文件

- 缺点：
  - 可执行文件较大
  - 如果库有改动，需要重新编译整个程序
  - 如果有多个进程使用相同的库，每个进程都会包含一份库的副本，浪费内存空间

动态链接在程序运行时才将程序与库链接

- 优点：
  - 可执行文件较小
  - 如果库有改动，只需重新编译库即可
  - 多个进程可以共享同一份库，节省内存空间
- 缺点：
  - 运行时需要外部库文件

# 面向对象

## 封装

封装就是把数据和操作数据的方法结合在一起，隐藏实现细节，选择性对外提供接口

封装可以降低代码的耦合度，提高安全性

## 成员访问

C++ 有三种成员访问：public、protected、private

类作用域外只能访问公开成员，友元可以突破类作用域的限制

基类的私有成员对派生类总是不可访问的

基类的公开成员和保护成员在派生类的成员访问 = min(在基类的成员访问，继承方式)

## struct和class

C++ 的 struct 和 class 都是类，它们唯一的区别就是 struct 的成员访问默认是 public，class 的成员访问默认是 private，struct 的继承方式默认是 public，class 的继承方式默认是 private

## this

对象调用成员函数时，会隐式将自己的地址传给成员函数，这个隐藏的形参就是 this，成员函数会通过解引用 this 来访问对象的成员

```C++
class A {
    int x;
    void f() {
        x = 1; // this->x = 1;
    }
};
```

this 一般存在寄存器里

## 成员函数的cv限定

非静态成员函数可以有 cv 限定，不同 cv 限定的函数可以重载

成员函数的 cv 限定，限定的是 this 指向的对象

不修改数据成员的成员函数，建议加上 const，可以提高可读性和健壮性

## 继承

继承是建立类之间关系的一种方式，派生类和基类是 is-a 的关系

继承可以提高代码的复用性

## 组合

组合就是一个类包含另一个类的对象，组合是 has-a 的关系

继承的耦合度高，组合的耦合度低

is-a 关系用继承，has-a 关系用组合，其他情况优先用组合

## 派生类和基类

存在派生类到基类的隐式类型转换

派生类会隐藏基类的同名成员

虚基类：被虚继承的基类叫虚基类，虚基类在最终派生对象中仅含一个子对象，虚基类子对象会优先初始化

## 静态类型和动态类型

静态类型就是声明的类型，是编译期确定的

动态类型就是实际的类型，是运行期确定的

只有基类指针或引用的动态类型可能和静态类型不一致

判断静态类型是否相同可以用 `std::is_same_v<A, B>`，判断动态类型是否相同可以用 `typeid(A) == typeid(B)`

## 多态

多态就是不同类型的对象调用同一接口，产生不同的行为

多态可以提供相同的接口和灵活的实现

## 静态多态

静态多态有重载、模板（CRTP）

### 重载

同一个作用域中，名字相同，形参不同的函数构成重载

编译器会根据重载决议的规则选择最合适的函数重载进行调用

### CRTP

CRTP 就是是基类是一个模板，派生类继承基类并指定基类的模板参数为派生类类型

CRTP 可以用于在基类暴露接口，在派生类实现接口，从而实现静态多态，静态多态是静态绑定的，没有运行时开销

```cpp
template<typename derived>
class base {
public:
    void name() {
        static_cast<derived*>(this)->impl();
    }
};

class X : public base<X> {
    friend base<X>;
    void impl() {
        std::cout << "X\n";
    }
};

class Y : public base<Y> {
    friend base<Y>;
    void impl() {
        std::cout << "Y\n";
    }
};

template<typename T>
void f(base<T>& r) {
    r.name();
}

int main() {
    X x;
    Y y;
    f(x);
    f(y);
}
```

## 动态多态

动态多态通过虚函数来实现，虚函数是一种成员函数，虚函数的行为可以在派生类被覆盖

覆盖：派生类的成员函数，拥有和基类的虚函数完全相同的接口，那么该成员函数也是虚函数并且会覆盖基类的虚函数

当使用基类指针或引用调用虚函数时，会发生动态绑定，也就是在运行时确定要调用的函数

## 虚函数的实现机制

有虚函数的类会有一张虚函数表，派生类会继承基类的虚函数表

如果派生类覆盖了基类的虚函数，那么派生类的虚函数表中对应虚函数的地址也会覆盖

如果一个类有虚函数表，对象的头部会存一个虚表指针指向虚函数表

虚函数调用时，会通过虚表指针找到虚函数表，然后查虚函数表确定要调用函数的地址

虚函数表在编译期生成，存在数据段

## 对象的内存模型

如果类中有虚函数，对象的头部会存一个虚表指针

剩下按类的继承顺序和成员的声明顺序来布局，遵循内存对齐

为了保证不同对象的地址不同，空类对象大小通常为 1

## explicit

explicit 表示显式，不能隐式转换和复制初始化

explicit 可以修饰构造函数、转换函数（C++11 起）

## 转换函数

转换函数是一种成员函数，可以启用从类类型到另一类型的转换

```cpp
struct X {
    operator T(){}
    explicit operator T(){} //C++11起
};
```

## 构造函数

### 初始化列表

初始化列表是构造函数定义的一部分，初始化列表是初始化，函数内是赋值

成员的初始化顺序和声明顺序一致，和初始化列表的顺序无关

### 默认构造函数

默认构造函数就是不需要提供实参的构造函数（没有形参 / 每个形参都有默认值）

如果没有定义或弃置构造函数，编译器会隐式定义一个默认构造函数

### 转换构造函数

非 explicit 的构造函数叫做转换构造函数

## 五个特殊成员函数

### 析构函数

如果没有定义或弃置析构函数，编译器会隐式定义一个析构函数

基类的析构函数需要公开且虚，否则通过基类指针 delete 对象是未定义行为

### 深复制和浅复制

浅复制：只复制句柄，不复制底层资源

深复制：复制底层资源

### 复制构造函数

复制构造函数的形参类型一般为 const T&

如果没有定义或弃置复制构造函数，编译器会隐式定义一个复制构造函数（浅复制）

如果类中有申请资源，我们必须自己写深复制

### 复制赋值运算符

如果没有定义或弃置复制赋值运算符，编译器会隐式定义一个复制赋值运算符（浅复制）

如果类中有申请资源，我们必须自己写深复制

```C++
T& operator=(const T& other) {
    //保证不是自赋值
    //深复制
    return *this;
}
```

另一种方法（copy-and-swap）：

```C++
T& operator=(T other) {
    //交换*this和other的资源
    return *this;
}
```

### 移动语义

移动语义：通过转移资源所有权，减少不必要的复制

被移动对象的资源已被转移，我们不应再对其做任何操作

如果五个特殊成员函数都没有定义或弃置，编译器会隐式定义移动构造函数和移动赋值运算符（逐对象移动）

### 移动构造函数

移动构造函数的形参类型一般为 T&&

### 移动赋值运算符

```C++
T& operator=(T&& other) {
    //保证不是自赋值
    //释放*this中的资源
    //窃取other中的资源
    //保证other处于可析构的状态
    return *this;
}
```

### 三五零法则

如果一个类需要写析构函数或复制构造函数或复制赋值运算符，那么大概率三个都要写

想要移动语义的类，五个都要写

其他情况能不写就不写

## 为什么不能虚构造

虚函数的地址存在虚函数表，如果构造函数是虚函数就要到虚函数表去调用，但是对象还没构造就没有虚表指针找不到虚函数表，所以不允许虚构造

## 构造或析构中调用虚函数

构造或析构中进一步的派生类并不存在，虚函数调用不会下传到派生类

# C++11

## nullptr

C++11 前，一般用 0 或 NULL 表示空指针，NULL 可能被定义成 0

那么 0 既可以表示整数又可以表示空指针，有些重载的场景会有问题

所以 C++11 引入了 nullptr，nullptr 是专门用来表示空指针的，它的类型是一个独立的类型，所以用 nullptr 表示空指针更安全

## 左值和右值

左值可以取地址，右值不能取地址

常见的左值：变量、前置自增/自减表达式、返回左值引用的函数调用、赋值表达式、解引用表达式

常见的右值：临时量、大部分字面量、后置自增/自减表达式、传值返回的函数调用、this、Lambda 表达式；返回右值引用的函数调用、转换成右值引用的转型表达式

左值引用只能绑定左值，右值引用只能绑定右值，const 左值引用和万能引用既可以绑定左值也可以绑定右值

引用变量的值类别是左值，不要混淆类型和值类别，它们是两个独立的属性

## C++类型转换

C++ 的类型转换有四种：

- static_cast 是静态类型转换，没有运行时类型检查，用于基本类型的转换和向上转换
- dynamic_cast 是动态类型转换，用于基类指针或引用到派生类的指针或引用的安全转换，不合法会返回空指针或抛出异常
- const_cast 用于添加或删除 const 属性
- reinterpret_cast 用于无关类型之间的转换

C++ 的类型转换相比 C 有更好的安全性和可读性

## 范围for

范围 for 就是迭代器遍历的语法糖

要支持范围 for，需要有 begin()、end() 两个函数，迭代器需要支持前置 ++、!=、*

## 可调用对象

可调用对象有函数、函数指针、重载了 ```operator()``` 的类对象（也叫函数对象）、Lambda 表达式

返回 bool 的可调用对象叫做谓词

## Lambda表达式

Lambda 表达式是一个匿名函数对象，可以捕获当前函数内的变量

定义一个 Lambda 表达式，同时也隐式定义了一个重载 ```operator()``` 的类，捕获的变量就是这个类的数据成员

值捕获的变量默认是 const 的

使用引用捕获，要注意悬垂引用的问题

## std::bind

```cpp
auto newCallable = std::bind(callable, args);
```

std::bind 可以接受一个可调用对象，绑定某些参数，生成一个新的可调用对象

std::placeholders 命名空间中定义了一些形如 _n 的占位符，用于表示新的可调用对象的参数位置

## std::function

```cpp
std::function<ret(args)> f = callable;
```

std::function 可以存储一个可调用对象，通常用于回调的场景

## emplace_back和push_back

push_back 是先构造一个临时对象，然后复制构造或者移动构造一个新元素，最后析构临时对象

emplace_back 是直接构造一个新元素

## using

using 通常用来：

- 引入类型别名
- 引入命名空间
- 引入命名空间的成员

## 成员函数的引用限定

成员函数的引用限定会影响重载决议

与 const 限定不同，引用限定不改变 this 的性质

## decltype

decltype 的作用是编译期推导实体的类型，或表达式的类型和值类别

## constexpr

constexpr 的作用是声明一个函数或变量为编译期常量，允许在编译期求值

## override

override 可以检查函数是否覆盖基类的虚函数

## final

final 可以防止一个类被继承或防止一个虚函数被覆盖

## 属性



# C++17

## std::string_view

std::string_view 是一个字符串视图，只包含一个指针和 size

对于函数形参，推荐使用 std::string_view 替换 const std::string&（实参是字符串字面量时，能显著提高效率）

字符串字面量的类型是 const char[N]，传参时会退化成 const char*，通过 std::string 的转换构造函数构造出一个 std::string 临时量

## 强制的复制消除

C++17 保证不再有用于复制或移动的临时量

# 内存管理

## 堆和栈的区别

栈内存在运行时由系统自动分配释放，栈空间一般比较小，分配速度快，用于存放局部变量、函数参数和返回值等

堆内存是动态分配释放，忘记释放会导致内存泄漏，堆空间一般比较大，分配速度慢

## new和delete的原理

new 先调 operator new() 分配内存（失败则抛出异常），再调构造函数构造对象

delete 先调析构函数析构对象，再调 operator delete() 释放内存

## new和delete的使用

new 一个元素：new T{} - delete

new 一个数组：new T[n]{} - delete[]

必须配对使用，否则是未定义行为

### 动态数组

C++ 中数组的大小必须是编译期常量，除非是动态分配的数组

```cpp
int n = 10;
auto arr = new int[n]; //只有第一维可以不是常量
delete[] arr;
```

不过这种情况一般使用 std::vector，更好用且 RAII

## new和malloc的区别

new 是运算符，malloc 是函数

new 不用显式传入分配大小，malloc 要

分配成功，new 返回对应类型的指针，malloc 返回 void*

分配失败，new 抛出异常，malloc 返回空指针

new 支持初始化，malloc 不支持

## placement new

new(address) T{}

placement new 用于在已分配的内存上构造对象

一般需要手动调用析构函数析构对象

```cpp
char buf[sizeof(T)];
auto p = new(buf)T{};
p->~T();
```

## RAII

RAII 就是将资源（堆内存、文件句柄、套接字等）和对象绑定，在构造函数获取资源，在析构函数释放资源

RAII 可以保证当对象生存期结束时，所有资源都能正确释放

## 智能指针

智能指针是可以自动管理资源的指针

### unique_ptr 

unique_ptr 表示独占所有权的指针，只能移动，不能复制

模拟实现：

```cpp
template <typename T>
class unique_ptr {
    T* p_{};  //空表示无所有权
    std::function<void(T*)> del_{[](T* p) { delete p; }};

public:
    unique_ptr() = default;

    explicit unique_ptr(T* p) : p_(p) {}

    template <typename D>
    unique_ptr(T* p, D del) : p_(p), del_(del) {}

    ~unique_ptr() {
        if (p_) {
            del_(p_);
        }
    }

    unique_ptr(const unique_ptr<T>&) = delete;
    unique_ptr<T>& operator=(const unique_ptr<T>&) = delete;

    unique_ptr(unique_ptr<T>&& other) : p_(other.p_), del_(other.del_) {
        other.p_ = nullptr;
    }

    unique_ptr<T>& operator=(unique_ptr<T>&& other) {
        if (this != &other) [[likely]] {
            if (p_) {
                del_(p_);
            }
            p_ = other.p_;
            del_ = other.del_;
            other.p_ = nullptr;
        }
        return *this;
    }

    T& operator*() const {
        return *p_; 
    }

    T* operator->() const { 
        return p_; 
    }

    T* get() const { 
        return p_; 
    }
    
    T* release() {
        auto res = p_;
        p_ = nullptr;
        return res;
    }

    explicit operator bool() const { 
        return p_ != nullptr; 
    }
};
```

常用成员函数

```cpp
.operator*()
.operator->()
.get()
.release()    //释放所有权 + 返回原指针
.operator bool()

std::make_unique
```

### shared_ptr 

shared_ptr 表示共享所有权的指针，通过引用计数实现，引用计数是线程安全的，访问资源不是

模拟实现：

```cpp
template <typename T>
class shared_ptr {
    T* p_{};
    std::atomic_int* pcount_{new std::atomic_int{1}};
    std::function<void(T*)> del_{[](T* p) { delete p; }};

    void incref() {
        ++*pcount_;
    }

    void decref() {
        if (--*pcount_ == 0) {
            if(p_) {
                del_(p_);
            }
            delete pcount_;
        }
    }

public:
    shared_ptr() = default;

    explicit shared_ptr(T* p) : p_(p) {}

    template <typename D>
    shared_ptr(T* p, D del) : p_(p), del_(del) {}

    ~shared_ptr() {
        if(pcount_) {
            decref();
        }
    }

    shared_ptr(const shared_ptr<T>& other)
        : p_(other.p_), pcount_(other.pcount_), del_(other.del_) {
        if(pcount_) {
            incref();
        }
    }

    shared_ptr<T>& operator=(const shared_ptr<T>& other) {
        if(this != &other) [[likely]] {
            if(pcount_) {
                decref();
            }
            p_ = other.p_;
            pcount_ = other.pcount_;
            if(pcount_) {
                incref();
            }
        }
        return *this;
    }
    
    shared_ptr(shared_ptr<T>&& other)
        : p_(other.p_), pcount_(other.pcount_), del_(other.del_) {
        other.p_ = nullptr;
        other.pcount_ = nullptr;
    }

    shared_ptr<T>& operator=(shared_ptr<T>&& other) {
        if(this != &other) [[likely]] {
            if(pcount_) {
                decref();
            }
            p_ = other.p_;
            pcount_ = other.pcount_;
            other.p_ = nullptr;
            other.pcount_ = nullptr;
        }
        return *this;
    }

    T& operator*() const {
        return *p_;
    }

    T* operator->() const {
        return p_;
    }
    
    T* get() const {
        return p_;
    }

    int use_count() const {
        if(pcount_) {
            return *pcount_;
        } else {
            return 0;
        }
    }

    explicit operator bool() const {
        return p_ != nullptr;
    }
};
```

常用成员函数

```cpp
.operator*()
.operator->()
.get()
.use_count()   //返回引用计数
.operator bool()

std::make_shared
```

### weak_ptr 

weak_ptr 是 shared_ptr 的弱引用，一般和 shared_ptr 搭配使用

weak_ptr 可以解决 shared_ptr 环状引用的问题

```cpp
//环状引用
struct node {
    std::shared_ptr<node> next_;
    std::shared_ptr<node> prev_;
};

auto node1 = std::make_shared<node>();
auto node2 = std::make_shared<node>();

node1->next_ = node2;
node2->prev_ = node1;

//解决方法: 令环中的指针之一为弱指针
struct node {
    std::weak_ptr<node> next_;
    std::weak_ptr<node> prev_;
};
```

常用成员函数

```cpp
.use_count()  //返回shared_ptr的引用计数
.expired()    //等价于use_count() == 0
.reset()
```

## 内存泄漏

内存泄漏就是没有释放已经不再使用的内存

内存泄漏的危害：对于长期运行的程序，内存泄漏会导致系统可用的资源越来越少，最终可能直接卡死

如何避免内存泄漏：RAII，智能指针，检测工具比如 Valgrind

# 模板编程

## 什么是模板

模板分为函数模板、类模板、变量模板（C++14 起）

模板必须实例化才会生成实际的代码

模板是编译期的

模板编程其实是面向编译器编程

## 模板特化

模板特化分为全特化和偏特化

全特化是给某个具体的类型定制行为，偏特化是给一类类型定制行为（函数模板不支持偏特化）

类型萃取就是用模板特化实现的

## 函数模板参数推导

在函数模板的模板参数推导中，函数实参的引用性会被忽略

特例：函数形参是万能引用且函数实参为左值

## auto

auto 的作用是编译期根据初始值推导类型

auto 推导和模板推导的区别是 auto 会把 {} 推导成 initializer_list，而模板无法推导 {}

auto 用于函数的形参类型或返回类型时，本质是模板推导

## 引用折叠

通过模板推导或类型别名可能构成引用的引用，此时：右值引用的右值引用折叠成右值引用，其余折叠成左值引用

## 万能引用

万能引用：接受左值则为左值引用，接受右值则为右值引用

万能引用的本质是模板推导

```C++
//(1)
template<class T>
void f(T&& x){}

//(2)
auto&& x = i;
```

## 完美转发

完美转发可以将接收到的实参原封不动地转发给另一个函数，保持实参的类型和值类别

```C++
template<typename...T>
void forward_func(T&&...arg) {
    dest_func(std::forward<T>(arg)...);
}
```

## std::move

std::move 将实参转换为右值，一般用来触发移动语义

参考实现：

```C++
template<typename T>
std::remove_reference_t<T>&& move(T&& param) {
    return static_cast<std::remove_reference_t<T>&&>(param);
}
```

## 非类型模板参数

类型模板参数，形如

```cpp
template<typename T> //模板参数 T 是一个类型
```

非类型模板参数，形如

```cpp
template<size_t T> //模板参数 T 是一个常量
```

## 形参包展开

模板递归

```cpp
void f(){}
template<typename T, typename...Args>
void f(T t, Args...args) {
    std::cout << t << ' ';
    f(args...);
}


//编译期if C++17起
template<typename T, typename...Args>
void f(T t, Args...args) {
    std::cout << t << ' ';
    if constexpr(sizeof...(args) > 0) {
        f(args...);
    }
}
```

折叠表达式（C++17 起）

语法形式：`(E 运算符 ...)`

`...` 展开的语法形式不变，类似递归

左折叠还是右折叠，取决于 `...` 在形参包的左边还是右边

左折叠从左边开始加括号，右折叠从右边开始加括号

```cpp
template<typename...Args>
void f(Args...args) {
    ((std::cout << args << ' '), ...);
}
```

## 函数调用禁止隐式转换

[函数调用禁止隐式转换](https://github.com/Mq-b/Loser-HomeWork/blob/main/src/卢瑟日经/函数调用禁止隐式转换.md)

# 并发编程

## 使用线程

### 启动线程

`std::thread` 线程对象是独占所有权的，不能复制，只能移动

```cpp
std::thread t;
```

默认构造 `std::thread`，线程对象是没有关联线程的

```cpp
template<typename F, typename...Args>
explicit thread(F&& f, Args&&...args);
```

`f` 是一个可调用对象，线程对象构造成功后，会自动在新线程开始执行 `f`

向可调用对象传递的参数，会被复制到新线程的内存空间中，转换成右值传递给可调用对象。如果想引用传参，要用 `std::ref()` 包装

### 执行策略

在线程对象的生存期结束之前，必须决定它的执行策略是合并还是分离

当线程对象调用 `join()`，会等待关联线程执行完毕，然后修改对象的状态，让 `joinable()` 返回 false，表示线程对象当前没有关联活跃线程

当线程对象调用 `detach()`，会放弃线程资源的所有权，允许此线程独立执行，退出时自动释放资源。放弃了线程资源的所有权，线程对象就没有关联活跃线程了，此时 `joinable()` 返回 false

只有 `joinable()` 返回 true，才能 `join()` 或 `detach()`

### std::thread::hardware_concurrency

`std::thread::hardware_concurrency()` 是 `std::thread` 的静态成员函数，可以获取当前硬件支持的并发线程数量

### std::this_thread

`std::this_thread` 命名空间中有一些常用的函数：

`get_id`：获取当前线程的 id

`sleep_for`：建议使用 `std::chrono_literals` 命名空间中的时间字面量

`yield`：让出时间片

### std::jthread

`std::jthread` 是 C++20 引入的，相当于 RAII 版的 `std::thread`

模拟实现：

```cpp
class jthread {
    std::thread t_;

public:
    jthread() = default;

    template <typename F, typename... Args>
    explicit jthread(F&& f, Args&&... args)
        : t_(std::forward<F>(f), std::forward<Args>(args)...) {}

    ~jthread() {
        if (joinable()) {
            join();
        }
    }

    jthread(jthread&& other) : t_(std::move(other.t_)) {}

    jthread& operator=(jthread&& other) {
        if (joinable()) {
            join();
        }
        t_ = std::move(other.t_);
        return *this;
    }

    bool joinable() const { return t_.joinable(); }

    std::thread::id get_id() const { return t_.get_id(); }

    void join() { t_.join(); }

    void detach() { t_.detach(); }

    void swap(jthread& other) { t_.swap(other.t_); }

    jthread(const jthread&) = delete;
    jthread& operator=(const jthread&) = delete;
};
```

## 共享数据

### 数据竞争

一个线程正在写，另一线程读或写同一内存位置时，会发生数据竞争，除非操作是原子的或者操作有固定的顺序

数据竞争是未定义行为，访问共享资源时，要注意数据竞争的问题

### std::mutex

`std::mutex` 是 C++11 提供的互斥量，可以用来保护临界区，相当于一个公共的标志位，可以锁定和解锁

### std::lock_guard

`std::lock_guard` 可以 RAII 的管理互斥量的锁定和解锁，不可复制，不可移动

模拟实现：

```cpp
template<typename T>
class lock_guard {
	T& mtx_;

public:
    explicit lock_guard(T& mtx) : mtx_(mtx) {
        mtx_.lock();
    }

    ~lock_guard() {
        mtx_.unlock();
    }

    //弃置复制构造和复制赋值 同时阻止了隐式定义的移动构造和移动赋值
    lock_guard(const lock_guard<T>&) = delete;
    lock_guard& operator=(const lock_guard<T>&) = delete;
};
```

### std::unique_lock

`std::unique_lock` 比 `std::lock_guard` 更灵活，可以提前解锁互斥量，还可以转移互斥量的所有权

`std::unique_lock` 的实现一般比 `std::lock_guard` 多一个 bool，通常优先使用  `std::lock_guard`

### 读写锁

为了防止多线程数据竞争，读写之间和写写之间必须互斥，读读之间是没有数据竞争的，此时可以使用读写锁

C++14 提供了 `std::shared_timed_mutex`，C++17 提供了 `std::shared_mutex`，前者额外支持超时功能，后者性能更好

对于写线程，可以用 `std::lock_guard` 或 `std::unique_lock` 保证独占访问

对于读线程，可以用 `std::shared_lock` 获取访问权，多个线程可以一起读取

例子：

```cpp
class Settings {
private:
    std::map<std::string, std::string> data_;
    mutable std::shared_mutex mtx_; // mutex 一般为 mutable 否则 const 成员函数中无法修改 mutex 状态

public:
    void set(const std::string& key, const std::string& value) {
        std::lock_guard<std::shared_mutex> lock{ mtx_ };
        data_[key] = value;
    }

    std::string get(const std::string& key) const {
        std::shared_lock<std::shared_mutex> lock(mtx_);
        auto it = data_.find(key);
        return (it != data_.end()) ? it->second : "";
    }
};
```

### std::recursive_mutex

`std::recursive_mutex` 允许一个线程多次锁定同一个互斥量，一般用于递归

不同线程对同一个 `std::recursive_mutex` 进行锁定时，一样会按照互斥量的规则进行阻塞

例子：

```cpp
void recursive_function(int count) {
    std::lock_guard<std::recursive_mutex> lc{ mtx };
    std::cout << "Locked by " << std::this_thread::get_id() << ", count = " << count << '\n';
    if (count > 0) {
        recursive_function(count - 1);
    }
}
```

### 线程安全

线程安全：多线程下，程序的结果是确定的

C++11 起的 new、operator new、delete、operator delete 以及 C 语言的动态分配函数都是线程安全的

自己重载 new 或 delete 时，需要自己保证线程安全：

- new 要考虑三方面： operator new、构造函数、修改指针
- delete 要考虑两方面：析构函数、operator delete

### thread_local

thread_local 关键字用于声明具有线程存储期的对象，也叫线程局部存储

类似线程局部存储，CPU 也可以有局部存储，避免了数据竞争，增强了并行处理的能力

## 同步操作

### 自旋锁

自旋锁就是，当线程发现锁已被占用时，会不断检查锁的状态，直到锁被释放

自旋锁适合锁的持有时间很短的场景，可以避免不必要的上下文切换，如果锁的持有时间较长，自旋锁会导致 CPU 空转，浪费计算资源

```cpp
class spin_lock {
    std::atomic_flag lock_{};

public:
    void lock() {
        while (lock_.test_and_set(std::memory_order_acquire)) {}
    }

    bool try_lock() { 
        return !lock_.test_and_set(std::memory_order_acquire); 
    }

    void unlock() { 
        lock_.clear(std::memory_order_release); 
    }
};
```

### 条件变量

条件变量允许线程在条件不满足时释放锁并等待，直到另一个线程使条件满足并唤醒等待的线程，被唤醒的线程将重新持有锁

条件变量适合需要长时间等待或者条件变化不频繁的场景，可以防止 CPU 空转，节省计算资源

我们可以实现一个线程安全的队列，访问队列是互斥的，pop 操作可以用条件变量确保队列不为空

```cpp
template<typename T>
class queue {
    mutable std::mutex mtx_;
    std::condition_variable cv_;
    std::queue<T> q_;

public:
    void push(const T& val) {
        {
            std::lock_guard<std::mutex> lc(mtx_);
            q_.push(val);
        }
        cv_.notify_one();
    }

    T pop() {
        std::unique_lock<std::mutex> lc(mtx_);
        cv_.wait(lc, [this]{return !q_.empty();});
        auto res = q_.front();
        q_.pop();
        return res;
    }

    bool empty() const {
        std::lock_guard<std::mutex> lc(mtx_);
        return q_.empty();
    }
};
```

### 两个线程交替打印

```cpp
std::mutex mtx;
std::condition_variable cv;
int x = 1;
bool flag = true;
std::thread t1([&] {
    for (int i = 0; i < 50; i++) {
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, [&] { return flag; });
        std::cout << std::this_thread::get_id() << ": print " << x << '\n';
        ++x;
        flag = false;
        cv.notify_one();
    }
});
std::thread t2([&] {
    for (int i = 0; i < 50; i++) {
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, [&] { return !flag; });
        std::cout << std::this_thread::get_id() << ": print " << x << '\n';
        ++x;
        flag = true;
        cv.notify_one();
    }
});
t1.join();
t2.join();
```

### std::future

`std::thread` 没有提供从线程获取返回值的机制，这个时候可以使用 `std::async` 启动一个异步任务，它会返回一个 `std::future` 对象，这个对象和任务关联，将持有函数的返回值，可以用 `valid()` 检查是否有关联任务，有关联任务可以用 `get()` 获取返回值

`std::async` 用法和 `std::thread` 类似，执行策略有 `std::launch::async` 和 `std::launch::deferred`，默认策略由实现定义



## 协程

协程是可以暂停和恢复的函数

C++20 起，只要函数定义包含 `co_await`、`co_yield`、`co_return` 三个关键字的任意一个，这个函数就是协程

# 一些八股

## 只能在栈上创建对象的类

```C++
class stack_only {
    stack_only() {}
    void* operator new(size_t) = delete;
public:
    static stack_only create() {
        return stack_only{};
    }
};
int main() {
    auto x = stack_only::create();
}
```

## 只能在堆上创建对象的类

```C++
class heap_only {
    heap_only() {}
    ~heap_only() {}
    heap_only(const heap_only&) = delete;
public:
    static heap_only* create() {
        return new heap_only{};
    }
    void destroy() {
        delete this;
    }
};
int main() {
    auto p = heap_only::create();
    p->destroy();
}
```

## 单例模式

```cpp
class Singleton {
    Singleton() = default;

public:
    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;

    static Singleton& getInstance() {
        static Singleton instance; //C++11保证静态局部变量的初始化是线程安全的 
        return instance;
    }
};
```

## strstr

模拟实现：

```C++
const char* my_strstr(const char* s, const char* p) {
    for(int i = 0; s[i]; i++) {
        bool success = true;
        for(int j = 0; p[j]; j++) {
            if(s[i + j] != p[j]) {
                success = false;
                break;
            }
        }
        if(success) return s + i;
    }
    return nullptr;
}
```

## memcpy

模拟实现：

```C++
void* my_memcpy(void* dst, const void* src, size_t n) {
    //1. 考虑内存重叠
    //src <= dst && src + n - 1 >= dst 从后往前
    //否则从前往后

    //2. 不考虑重叠 直接从前往后即可
}
```

## split

模拟实现：

```C++
auto split(const std::string& s, char sep = ' ') {
    std::vector<std::string> res;
	std::stringstream ssin(s);
    std::string t;
    while(std::getline(ssin, t, sep)) {
        if(!t.empty()) {
            res.push_back(t);
        }
    }
    return res;
}
```