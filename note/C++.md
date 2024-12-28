> C++ 是一门全能的编程语言，非常庞大而复杂。我们只需要使用它的一个子集，完成自己的需求就好，而不必过分纠结 C++ 本身的复杂性
>
> 参考：C++ primer、Effective Modern C++、C++ 并发编程实战、cppreference

# 基础知识

## C和C++

C 和 C++ 是两门不同的编程语言，C++ 最早是 C with class，现在 C++ 已经发展成了一个语言联邦

C 是面向过程的，C++ 除了面向过程，还支持面向对象、泛型编程、函数式编程等

C++ 只兼容 C 的一个子集，现代 C 的很多特性 C++ 并不支持

## 基本类型大小

C++ 规定：char 最小 8 位，short 最小 16 位，int 最小 16 位，long 最小 32 位，long long 最小 64 位

float 一般是 IEEE-754 32 位格式，double 一般是 IEEE-754 64 位格式，long double 由实现定义

指针类型的大小是该平台（操作系统）的位宽

## 整数的存储

为了将加法和减法统一成一种运算，负数采用补码表示，因为减去一个数等同于加上该数的补码

有符号数的最高位是符号位，无符号数的所有位都是数值位

char、signed char、unsigned char 是三个不同的类型，char 的符号性由实现定义

有符号数溢出是未定义行为

无符号数溢出，结果相当于对它能表示的数值总数取模

## IEEE-754

存储方式：符号位 - 指数位 - 尾数位

指数位越长，能表示的数值范围越大

尾数位越长，精度越高

32 位浮点数：1 - 8 - 23

64 位浮点数：1 - 11 - 52

指数可能是负数，为了统一表示（而不是使用符号位和补码），指数位保存时要加上一个偏移量（127 / 1023）

科学计数法的整数部分总是 1，所以尾数只保存小数部分，可以节省一位空间，提高精度

误差：尾数位是有限的，对于无法用有限位数表示的数（比如循环小数、无理数），只能用近似值来表示

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
    std::cout << (check() == 1 ? "little\n" : "big\n");
}
```

## 内存对齐

内存对齐就是数据在内存中的起始地址是某个数的整数倍

每个类型都有自己的对齐要求，对齐要求是 2 的幂，可以用 alignof 获取、用 alignas 修改（C++11）

内存对齐的原因：有些平台不能访问非对齐的内存

## inline

inline 的本意是建议编译器优先内联（将函数在调用处展开，避免函数调用的开销，但可能导致更大的可执行文件）

C++98 起，inline 的语义改为允许一个符号在不同编译单元多次定义，链接时只取一份定义，可以用来修饰头文件中定义的全局变量或函数

类中定义的函数是隐式 inline 的

## 内联函数和宏

这里的内联函数指的是编译器对函数采用内联替换

宏就是文本替换，在预处理阶段完成

区别：宏没有类型检查，内联函数有类型检查

## static

static 修饰局部变量：延长生命周期，首次访问时初始化，只会被初始化一次（C++11 保证线程安全）

static 修饰全局变量或函数：内部链接，可以用来防止命名冲突（类似的还有匿名命名空间、const 全局变量）

static 修饰类的数据成员：静态数据成员不与对象绑定，需要单独定义，可以直接用类名访问。static 关键字只用于类中声明

static 修饰类的成员函数：静态成员函数不与对象绑定，没有 this，可以直接用类名访问。static 关键字只用于类中声明

## const

const 表示只读，是一种语义上的约束，保证运行时不被修改

const 可以修饰变量、返回值、函数形参、成员函数等

## volatile

volatile 表示容易改变，可以阻止编译器优化，保证总是从内存中读取

标准 C++ 的 volatile 不是原子的、没有内存定序，不能用于多线程同步

## 引用和指针

引用是变量的别名，指针是用来存储地址的变量

引用不能改变指向，指针可以（注：C++ 的引用不能改变指向，这点与 Java 不同，Java 的引用类似于指针）

没有多级引用，但有多级指针

没有空引用，但有空指针

## #include<>和#include""

`<>` 直接去系统目录查找，一般用于包含标准库头文件

`""` 先在当前工作目录查找，找不到再去系统目录查找，一般用于包含自定义头文件

头文件包含就是将头文件的内容直接拷贝进来

## 头文件重复包含

头文件重复包含是指头文件在一个编译单元中多次包含

可以使用条件编译（`#ifndef` `#define` `#endif`）或 `#pragma once` 来避免重复包含

## 重载

同一个作用域中，名字相同，形参不同的函数构成重载

编译器会根据重载决议的规则选择最合适的函数重载进行调用

运算符重载也是函数重载，只不过函数名的形式是 `operator xxx`

函数重载怎么实现的：符号修饰

## extern"C"

`extern"C"` 告诉编译器按照 C 的方式处理符号，否则链接 C 库的时候会出问题，因为 C++ 的符号修饰一般和 C 不同

## 源文件到可执行文件的过程

每个源文件都是一个编译单元，每个编译单元被独立编译成一个目标文件，由链接器将所有目标文件链接成一个可执行文件

源文件到可执行文件的过程可以分为四个阶段：

- 预处理：执行预处理指令，比如宏替换、头文件展开、条件编译
- 编译：词法分析、语法分析、语义分析、代码优化，生成汇编代码
- 汇编：将汇编代码翻译成二进制代码，生成目标文件
- 链接：符号重定位，将所有目标文件和相关的库链接成可执行文件

## 静态链接和动态链接

静态链接会把库代码拷贝到可执行文件

- 优点：运行时不需要外部库文件
- 缺点：
  - 可执行文件较大
  - 如果有多个进程使用相同的库，每个进程都会包含一份库的副本，浪费内存空间

动态链接只记录所需的符号引用，库文件在程序运行时才载入内存

- 优点：
  - 可执行文件较小
  - 多个进程可以共享同一份库，节省内存空间
- 缺点：运行时需要外部库文件

## 堆和栈的区别

栈从高地址向低地址增长，程序运行时由系统自动分配释放

堆从低地址向高地址增长，由程序动态分配释放，忘记释放会内存泄漏

栈一般比较小，分配速度快

堆比较大，分配速度慢

## static变量和const变量存在哪

static 变量都存在数据段

const 的局部变量存在栈上、全局变量存在数据段

## new和delete的原理

new 先调 operator new() 分配内存（失败会抛出 std::bad_alloc），再调 placement new 构造对象

delete 先调析构函数析构对象，再调 operator delete() 释放内存

## new和delete的使用

new 一个元素：new T{} - delete

new 多个元素：new T[n]{} - delete[]

必须配对使用，否则是未定义行为

## new和malloc的区别

new 是运算符，malloc 是函数

new 不用显式传入分配大小，malloc 要

分配成功，new 返回对应类型的指针，malloc 返回 void*

分配失败，new 抛出异常，malloc 返回空指针

new 支持初始化，malloc 不支持

## placement new

new(address) T{}

placement new 用于在已分配的内存上构造对象

需要手动调用析构函数析构对象

```cpp
char buf[sizeof(T)];
auto p = new(buf)T{};
p->~T();
```

## 内存泄漏

内存泄漏就是程序没有释放已经不再使用的内存，导致这部分内存不能再次被使用

内存泄漏的危害：对于长期运行的程序，内存泄漏会导致系统可用的资源越来越少，最终可能直接卡死

怎么检测内存泄漏：检查每个 new 有没有对应的 delete、使用检测工具比如 valgrind

如何避免内存泄漏：RAII、智能指针

怎么实现一个内存泄漏检测工具：重载内存分配函数，记录每个内存块的信息（地址、大小、是否释放）。程序退出时遍历内存块信息，找到没有释放的内存块，输出对应的信息。

## STL六大组件

容器、算法、迭代器、函数对象、适配器、分配器

### 容器

#### std::array

std::array 是定长数组，开辟在栈上

std::array 是对原生数组的封装，没有额外的空间开销

#### std::vector

std::vector 是动态数组，在堆上分配一块足够大的连续空间来存储元素

插入元素时如果空间不足会扩容（扩容 = 开辟一块更大的空间 + 拷贝数据到新空间 + 释放旧空间）

所以尾部插入是均摊 $O(1)$ 的。尾部删除是 $O(1)$

其他位置插入删除是 $O(n)$ 的，因为需要挪动数据

std::vector 本身一般需要三个指针的空间

#### std::deque

std::deque 是双端队列，元素的存储是分段连续的，一段就是一块连续的内存

当存储空间不足时，std::deque 会分配新的块，并维护新的块和其他块之间的联系

头尾插入删除都是 $O(1)$ 的​，其他位置插入删除只需挪动较少的一半数据

#### std::list

std::list 是带头双向循环链表，结点在堆上申请，没有预留空间

访问某个元素必须遍历 $O(n)$

任意位置插入删除 $O(1)$，不需要挪动数据

每个结点需要两个指针的额外空间，来维持双向链表结构

std::list 本身需要存一个 dummy 指针和一个 size

#### std::forward_list

std::forward_list 是单链表，相当于 std::list 的内存优化版本

每个结点需要一个指针的额外空间，来维持单链表结构

std::forward_list 本身只需要存一个 head 指针

### 算法

### 迭代器

迭代器就是指针的抽象

迭代器给容器提供了统一的遍历方式，并且使得算法不依赖容器

const_iterator 表示指向常量的指针，const iterator 表示指针本身是常量

### 函数对象

函数对象就是重载了 ```operator()``` 的类对象

### 适配器

适配器就是将容器封装成另一种数据结构，只提供必要的接口

#### std::stack

std::stack 将顺序容器封装成栈，后进先出，默认使用 std::deque 容器

#### std::queue

std::queue 将顺序容器封装成队列，先进先出，默认使用 std::deque 容器

#### std::priority_queue

std::priority_queue 将顺序容器封装成堆，默认使用 std::vector 容器构建大顶堆

### 分配器

std::allocator 可以将申请内存和构造对象分离

allocate、deallocate、construct、destroy

## 惯用手法

### CRTP

CRTP（奇特重现模板模式）就是写一个基类模板，在基类暴露接口，派生类继承时将自身作为基类模板的参数，在派生类实现接口，从而实现静态多态

相比动态多态，CRTP 实现的静态多态没有虚函数调用的运行时开销，没有虚表指针的空间开销，并且由于是静态绑定，编译器可以进行内联优化

```cpp
template<typename T>
class Base {
public:
    void name() {
        static_cast<T*>(this)->impl();
    }
};

class X : public Base<X> {
    friend Base<X>;
    void impl() {
        std::cout << "X\n";
    }
};

class Y : public Base<Y> {
    friend Base<Y>;
    void impl() {
        std::cout << "Y\n";
    }
};

template<typename T>
void f(Base<T>& r) {
    r.name();
}

int main() {
    X x;
    Y y;
    f(x);
    f(y);
}
```

### PIMPL

PIMPL（指向实现的指针）可以：

1. 隐藏类的实现细节
2. 减少编译时依赖，提高编译速度

```cpp
//A.h
#pragma once

//外部接口
class A{
public:
    void f();
    
private:
    class AImpl;
    AImpl* impl_; //指向实现的指针
};


//A.cpp
#include "A.h"
//包含需要的头文件

//实现细节
class A::AImpl {
public:
    void f() {
        //...
    }

private:
    int a;
    //...
    
    void _f() {
        //...
    }
    //...
};

//外部接口 调用实现方法
void A::f() {
    impl_->f();
}
```

### RAII

RAII（资源获取即初始化）就是把资源和对象的生命周期绑定，构造函数获取资源，析构函数释放资源，这样可以保证当对象的生命周期结束时，所有资源都能正确释放

## 池

池是一组资源的集合，这组资源提前被创建好，用户获取资源直接从池中分配，回收回来的资源无需释放，而是放回池中供下一次的分配

池主要用于避免频繁执行系统调用来创建和销毁资源。系统调用有用户态内核态之间的上下文切换和数据拷贝，开销较大

常见的池有内存池、进程池、线程池、连接池

# 面向对象

## 封装

封装就是把数据和操作数据的方法捆绑在一起，隐藏实现细节，选择性对外提供接口

封装可以降低耦合度、提高安全性

## 成员访问

C++ 有三种成员访问：public、protected、private

类作用域外只能访问公开成员，友元可以突破类作用域的限制

基类的私有成员对派生类总是不可访问的

基类的公开成员和保护成员在派生类的成员访问 = min(在基类的成员访问，继承方式)

## struct和class

C++ 的 struct 和 class 都是类

区别：

- struct 的成员访问默认是 public，class 的成员访问默认是 private
- struct 的继承方式默认是 public，class 的继承方式默认是 private

## 友元

友元可以突破封装，但不宜多用

友元关系是单向的、不能传递、不能继承

## this

对象调用成员函数时，会隐式将自己的地址传给成员函数，这个隐藏的形参就是 this

成员函数会通过解引用 this 来访问对象的成员

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

成员函数的 cv 限定，限定的是 this 指向的对象，不同 cv 限定的函数可以重载

const 成员函数可以被 const 对象和非 const 对象调用，非 const 成员函数只能被非 const 对象调用

如果成员函数重载了 const 版本和非 const 版本，const 对象会匹配到 const 版本，非 const 对象会匹配到非 const 版本

因此：

- 如果一个成员函数不修改对象的状态，使用 const 限定可以提高代码的可读性和健壮性
- 如果一个成员函数对于 const 对象和非 const 对象有不同的行为，则应重载 const 版本和非 const 版本

## 继承

继承是建立类之间关系的一种方式，派生类和基类是 is-a 的关系

继承可以提高代码的复用性

## 组合

组合就是一个类包含另一个类的对象，是 has-a 的关系

继承的耦合度高，组合的耦合度低

is-a 关系用继承，has-a 关系用组合，其他情况优先用组合

## 派生类和基类

向上转换（切片）：派生类对象/指针/引用转换为基类对象/指针/引用。向上转换是安全的隐式转换

向下转换：基类指针/引用转换为派生类指针/引用

派生类会隐藏基类的同名成员

虚基类：被虚继承的基类叫虚基类，虚基类在最终派生对象中仅含一个子对象。虚基类子对象会优先初始化

## 显式转换

C++ 的显式转换有四种：

- static_cast 用于基本的转换，允许向下转换但是没有运行时检查
- dynamic_cast 用于安全的向下转换，不合法会返回空指针/抛出异常
- const_cast 用于添加或移除 const 属性
- reinterpret_cast 用于重新解释底层的转换，比如指针类型和整型之间的转换、不同指针类型之间的转换等

C++ 的显式转换相比 C 的显式转换有更好的安全性和可读性

## explicit

explicit 表示显式，不允许隐式转换

explicit 可以修饰构造函数、转换函数（C++11）

## 转换函数

转换函数是一种成员函数，可以启用从类类型到另一类型的转换

```cpp
struct X {
    operator T(){}          //X 可以隐式或显式转换为 T
    explicit operator T(){} //X 可以显式转换为 T
};
```

## 静态类型和动态类型

静态类型就是声明的类型，是编译期确定的

动态类型就是实际的类型，是运行期确定的

只有基类指针/引用的动态类型可能和静态类型不一致

判断静态类型是否相同可以用 `std::is_same_v<A, B>`，判断动态类型是否相同可以用 `typeid(A) == typeid(B)`

## 多态

多态就是不同类型的对象调用同一接口，产生不同的行为

多态可以给同一接口提供灵活的实现

> C++ 的多态分为静态多态和动态多态，静态多态有重载、模板，动态多态是通过虚函数来实现的

## 虚函数

以 virtual 声明的成员函数叫做虚函数

虚函数的行为可以在派生类被覆盖（派生类的成员函数，拥有和基类的虚函数完全相同的接口，那么该成员函数也是虚函数并且会覆盖基类的虚函数）

当使用基类指针或引用调用虚函数时，会发生动态绑定，在运行时确定要调用的函数

## 虚函数的实现机制

有虚函数的类会有一张虚函数表，派生类会继承基类的虚函数表，如果派生类重写了基类的虚函数，那么派生类虚函数表中对应的虚函数地址会被覆盖

有虚函数表的类，对象的头部会存一个指针指向虚函数表，虚函数调用会通过这个指针转到虚函数表，然后查虚函数表确定要调用函数的地址

虚函数表在编译期生成，存在数据段

虚函数的性能开销：

- 虚函数是动态绑定的（运行时确定要调用的函数），编译器不能内联优化
- 每次调用虚函数都需要通过虚函数表间接跳转，频繁的跳转对 cache 不友好
- 每个对象需要额外存储一个指向虚函数表的指针

## 对象的内存布局

如果类中有虚函数，对象的头部会存一个虚表指针

剩下按类的继承顺序和成员的声明顺序来布局，遵循内存对齐

为了保证不同对象的地址不同，空类的大小一般为 1 字节（因为一个地址标识 1 字节，更大会浪费）

## 纯虚函数和抽象类

有纯虚函数的类叫做抽象类，抽象类不能实例化出对象，一般用作接口类

纯虚函数要求派生类必须重写，否则派生类也是抽象类

## 构造函数

### 初始化列表

初始化列表是构造函数定义的一部分，会初始化类的所有非静态数据成员

成员的初始化顺序和声明顺序一致，与初始化列表的顺序无关

### 默认构造函数

默认构造函数就是不需要提供实参的构造函数（没有形参 / 每个形参都有默认值）

> 如果没有显式定义（或弃置）构造函数 / 拷贝构造 / 移动构造，编译器会隐式定义一个默认构造函数

### 转换构造函数

非 explicit 的构造函数叫做转换构造函数

### 拷贝构造函数

拷贝构造的形参类型一般为 const T&

### 移动构造函数

移动构造的形参类型一般为 T&&

## 五个特殊成员函数

### 析构函数

> 如果没有显式定义（或弃置）析构函数，编译器会隐式定义一个析构函数

基类的析构函数需要公开且虚，否则通过基类指针 delete 对象是未定义行为，可能造成内存泄漏

### 拷贝语义

浅拷贝：只拷贝句柄，不拷贝底层资源

深拷贝：拷贝底层资源

> 如果没有显式定义（或弃置）拷贝构造 / 移动构造 / 移动赋值，编译器会隐式定义拷贝构造
>
> 如果没有显式定义（或弃置）拷贝赋值 / 移动构造 / 移动赋值，编译器会隐式定义拷贝赋值
>
> 隐式定义的拷贝构造 / 拷贝赋值，对于自定义类型的成员调用它的拷贝构造 / 拷贝赋值，对于基本类型的成员采用浅拷贝

### 移动语义

移动语义（C++11）：转移资源的所有权，减少不必要的拷贝

被移动对象的资源已被转移，我们不应再对其做任何操作

> 如果五个特殊成员函数都没有显式定义（或弃置），编译器会隐式定义移动构造和移动赋值
>
> 隐式定义的移动构造和移动赋值会逐成员移动，能移动则移动，否则拷贝

## 为什么不能虚构造

虚函数的地址存在虚函数表，如果构造函数是虚函数就要到虚函数表去调用，但是对象还没构造就没有虚表指针找不到虚函数表，所以不允许虚构造

## 构造或析构中调虚函数

构造或析构中进一步的派生类并不存在，虚函数调用不会下传到派生类，不能达到预期的动态多态的效果

## 虚函数可以是内联函数吗

虚函数可以被 inline 修饰，因为 inline 的语义最早是建议编译器内联优化，C++98 起改为允许一个符号在不同编译单元多次定义、链接时只取一份定义，一般用来修饰头文件中定义的全局变量或函数，防止链接时重定义

当虚函数是动态绑定时（使用基类指针或引用调用虚函数）不会被内联优化，因为动态绑定在运行时才确定要调用的函数

# C++11

## auto

auto 的作用是编译期根据初始值推导类型

auto 推导和模板推导的区别是 auto 会把 {} 推导成 initializer_list，而模板无法推导 {}

auto 用于函数的形参类型或返回类型时，本质是模板推导

## decltype

decltype 的作用是编译期推导实体的类型，或表达式的类型+值类别

decltype 是不求值语境

## 范围for

一个类型如果满足范围的概念，就可以使用范围 for

范围：可以用 `begin()` 和 `end()` 获取起始和终止位置，`begin()` 返回的对象必须支持 `前置++`、`*`、`!=`

## lambda

>可调用对象有函数、函数指针、函数对象

lambda 表达式是一个匿名函数对象，可以捕获**当前作用域**的变量，定义一个 lambda 表达式，同时也隐式定义了一个重载 ```operator()``` 的类，捕获的变量就是这个类的数据成员，函数参数就是 `operator()` 的参数，函数体就是 `operator()` 的函数体

值捕获的变量默认是只读的

使用引用捕获，要注意悬垂引用的问题

## 左值和右值

左值和右值是表达式的属性，左值可以取地址，右值不能取地址

常见的左值有：变量、字符串字面量、前置++/--、返回左值引用的函数调用、赋值表达式、解引用表达式等等

常见的右值有：临时量、后置++/--、传值返回的函数调用、lambda、this；返回右值引用的函数调用、转换到右值引用的表达式

左值引用只能绑定左值，右值引用只能绑定右值，const 左值引用和万能引用既可以绑定左值也可以绑定右值

引用变量的值类别是左值，不要混淆类型和值类别，它们是两个独立的属性

## 智能指针

智能指针可以自动管理资源，防止内存泄漏

### std::unique_ptr

独占指针，独占所管理的对象

常用成员函数：

```cpp
release //返回对象指针 释放所有权
reset   //释放原对象 接管新对象
swap    //交换管理对象
get     //返回对象指针
get_deleter   //返回删除器
operator bool //是否接管对象
```

相比共享指针，独占指针无需维护引用计数，性能更优，一般优先使用独占指针

对于非轮子项目，建议把所有 `new T{}` 改成 `std::make_unique<T>()`，对应的 `T*` 改成 `std::unique_ptr<T>`

### std::shared_ptr

共享指针，共享所管理的对象，只能通过共享指针的拷贝构造或拷贝赋值来共享所有权，用共享指针持有的原始指针创建新的共享指针是未定义行为

常用成员函数：

```cpp
reset   //释放原对象 接管新对象
swap    //交换管理对象
get     //返回对象指针
use_count     //返回管理对象关联的共享指针个数
operator bool //是否接管对象
```

多线程访问不同的共享指针是线程安全的（即使这些共享指针管理同一个对象），因为引用计数是线程安全的

但是多线程读写同一个共享指针不是线程安全的

### std::weak_ptr

弱指针指向共享指针管理的对象，不控制对象的生命周期，访问对象前必须转换为共享指针（临时所有权）

常用成员函数：

```cpp
reset     //放弃原对象 接管新对象
swap      //交换管理对象
use_count //返回管理对象关联的共享指针个数
expired   //检查对象是否存在
lock      //尝试创建一个共享指针
```

弱指针还可以用来打破共享指针管理的对象组成的循环引用（共享指针的引用计数无法抵达 0 导致内存泄漏）

```cpp
struct Node {
    std::shared_ptr<Node> prev_;
    std::shared_ptr<Node> next_;
};

//双向链表
{
    //创建结点 用共享指针管理
    auto node1 = std::make_shared<Node>();
    auto node2 = std::make_shared<Node>();
    
    node1->next_ = node2;
    node2->prev_ = node1;
}
//两个共享指针销毁 两个结点的引用计数都为1 没有销毁

//解决方法: 令环中的共享指针之一为弱指针
struct Node {
    std::weak_ptr<Node> prev_;
    std::weak_ptr<Node> next_;
};
```

### 实现说明

在典型实现中，共享指针会保有两个指针：get() 所返回的指针和指向控制块的指针

控制块中包含了被管理对象或指向被管理对象的指针、删除器、分配器、共享指针计数和弱指针计数

使用 std::make_shared 创建共享指针，会一次性创建控制块和被管理对象（并且异常安全），控制块存储的是被管理对象

使用构造函数创建共享指针，控制块和被管理对象只能分别创建（可能抛出异常），控制块存储的是指向被管理对象的指针

当共享指针计数为 0 时，控制块会调用被管理对象的析构函数。控制块本身会等到弱指针计数为 0 时销毁（实现中共享指针会增加弱指针计数）

### std::enable_shared_from_this

std::enable_shared_from_this 是一个类模板，允许派生类通过 `shared_from_this()` 生成指向自己的共享指针。生成的共享指针和已有的共享指针共享控制块（如果不存在已有的共享指针会出错），这是通过 std::enable_shared_from_this 的一个弱指针成员实现的

而如果直接用 this 构造共享指针，新的共享指针维护自己的控制块，会有 double-free 的问题

## nullptr

C++11 前，一般用 0 或 NULL 表示空指针，NULL 可能被定义成 0

那么 0 既可以表示整数又可以表示空指针，有些重载的场景会有问题

nullptr 是专门用来表示空指针的，它的类型是一个独立的类型，用 nullptr 表示空指针更安全

## constexpr

constexpr 表示常量表达式，允许在编译期求值，从而提高运行时效率

const 只保证运行时不被修改，constexpr 是真正的编译期常量

## override

override 可以检查函数是否覆盖基类的虚函数

## final

final 可以防止一个类被继承或者防止一个虚函数被覆盖

## push和emplace

push 是先构造一个副本，然后拷贝或移动构造一个新元素

emplace 是直接构造一个新元素

## std::bind

```cpp
auto newCallable = std::bind(callable, args);
```

std::bind 是一个函数模板，可以接收一个可调用对象，绑定某些参数，生成一个新的可调用对象

std::placeholders 命名空间中定义了一些形如 _n 的占位符，用于表示新的可调用对象的参数位置

现代 C++ 中建议使用 lambda 表达式代替 std::bind，语法更简洁、强大

## std::function

```cpp
std::function<ret(args)> f = callable;
```

std::function 是一个类模板，可以存储一个可调用对象，通常用于回调的场景

## 内联命名空间

内联命名空间会把命名空间的成员导出到父命名空间

```cpp
inline namespace A{}
```

## using

using 通常用来：

- 引入类型别名
- 引入命名空间
- 引入命名空间的成员

## 成员函数的引用限定

成员函数的引用限定会影响重载决议，& 限定的成员函数只能被左值调用，&& 限定的成员函数只能被右值调用

与 cv 限定不同，引用限定不会改变 this 的性质

# 模板编程

## 什么是模板

模板分为函数模板、类模板、变量模板（_v）、别名模板（_t）

模板必须实例化才会生成实际的代码（隐式实例化、显式实例化）

```cpp
template<typename T>
void f(T) {}

template void f<int>(int); //显式实例化 f<int>(int)

int main() {
    f(1);         //隐式实例化 f<int>(int)
    f<int>(1);    //显式实例化 f<int>(int)
}
```

模板是编译期的

## 模板特化

模板特化分为全特化和偏特化

全特化是给某个具体的类型定制行为，偏特化是给一组类型定制行为

```cpp
template<typename T>
struct X {
    X() {
        std::cout << "主模板\n";
    }
};

template<>
struct X<int> {
    X() {
        std::cout << "全特化\n";
    }
};

template<typename T>
struct X<T*> {
    X() {
        std::cout << "偏特化\n";
    }
};
```

## 模板分文件

模板必须实例化才会生成实际的代码（编译期）

模板声明和定义分别写在头文件和源文件，就没有隐式实例化了，链接就会报错

可以在模板定义的地方显式实例化，让编译器生成对应的代码，来解决这个问题（模板全特化有类似的作用）

建议：模板不要分文件，而是直接写在头文件，比如 boost 里的 hpp 文件

## 引用折叠

右值引用的右值引用折叠成右值引用，其余折叠成左值引用

## 万能引用

万能引用接收左值则为左值引用，接收右值则为右值引用

万能引用的本质是模板推导，模板推导会忽略实参的引用性（除了万能引用接收左值表达式的情况）

```C++
//(1)
template<typename T>
void f(T&& x){}

//(2)
auto&& x = i;
```

## 完美转发

完美转发可以将接收到的实参原封不动地转发给另一个函数，保持实参的类型和值类别

```C++
//可变参数模板 + 万能引用 + std::forward函数模板
template<typename...T>
void forward_func(T&&...arg) {
    dest_func(std::forward<T>(arg)...);
}
```

`std::forward<type&>(arg)` 返回 type&，是一个左值表达式

`std::forward<type>(arg)` 返回 type&&，是一个右值表达式

## std::move

`std::move()` 是一个函数模板，返回实参类型的右值引用，一般用来触发移动语义

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
template<typename T> //类型模板参数 T 是一个类型
```

非类型模板参数，形如

```cpp
template<size_t T>   //非类型模板参数T 是一个常量
```

## 形参包展开

形参包可以接收零个或多个参数

模板递归

```cpp
void f(){}
template<typename T, typename...Args>
void f(T t, Args...args) {
    std::cout << t << ' ';
    f(args...);
}
```

折叠表达式（C++17）

````cpp
(E 运算符 ...)           //一元右折叠
(... 运算符 E)           //一元左折叠
(E 运算符 ... 运算符 I)  //二元右折叠
(I 运算符 ... 运算符 E)  //二元左折叠
````

`...` 展开的语法形式不变，类似递归

左折叠还是右折叠，取决于 `...` 在形参包的左边还是右边

左折叠从左边开始加括号，右折叠从右边开始加括号

```cpp
template<typename...Args>
void f(Args...args) {
    ((std::cout << args << ' '), ...);
}
```

## SFINAE

SFINAE（替换失败不是错误）：当编译器进行模板推导时，替换失败不是错误，只是不选择这个重载

SFINAE 可以用来约束模板的传入类型，减少不必要的实例化（提高编译速度），同时可以使报错更清晰

例子：写一个函数模板，要求传入的对象支持 `+`

```cpp
template<typename T>
auto f(const T& a, const T& b) -> decltype(a + b) {
    return a + b;
}
```

`std::void_t` 是一个别名模板，可以配合 SFINAE 使用

```cpp
template<typename...>
using void_t = void;
```

例子：写一个函数模板，要求传入的对象支持 `+`

```cpp
template<typename T, typename SFINAE = std::void_t<decltype(std::declval<T>() + std::declval<T>())>>
auto f(const T& a, const T& b) {
    return a + b;
}
```

例子：检查类型是否有某些成员

```cpp
template<typename T, typename SFINAE = std::void_t<
        typename T::type,      //类型
        decltype(&T::value),   //成员  decltype(std::declval<T>().value)
        decltype(&T::fun)      //成员  decltype(std::declval<T>().fun())
    >
>
void f(T){}


template<typename T>
std::void_t<
    typename T::type,      //类型
    decltype(&T::value),   //成员  decltype(std::declval<T>().value)
    decltype(&T::fun)      //成员  decltype(std::declval<T>().fun())
> //代换失败-不选择这个重载  代换成功-别名模板实例化-void
f(T){}
```

`std::enable_if` 是一个类模板，可以配合 SFINAE 使用

```cpp
template<bool B, typename T = void>
struct enable_if{};

//偏特化
template<typename T>
struct enable_if<true, T> {
    using type = T;
};
```

`std::enable_if_t` 是一个别名模板，可以更方便地配合 SFINAE 使用

```cpp
template<bool B, typename T = void>
using enable_if_t = typename enable_if<B, T>::type;
```

例子：写一个函数模板，要求传入对象的类型是 int

```cpp
template<typename T, typename SFINAE = std::enable_if_t<std::is_same_v<T, int>>>
                   //typename SFINAE = std::enable_if_t<true> -> typename SFINAE = void
void f(T){}


template<typename T, std::enable_if_t<std::is_same_v<T, int>, int> = 0>
                   //std::enable_if_t<true, int> = 0 -> int = 0
void f(T){}


template<typename T>
std::enable_if_t<std::is_same_v<T, int>> //std::enable_if<true> -> void
f(T){}
```

## 概念

概念在编译期求值，可以约束模板接口

和 SFINAE 一样，概念可以使报错更清晰，但比 SFINAE 更简单、强大

```cpp
//定义概念
template</*模板参数*/>
concept 概念名 = /*编译期的bool表达式*/;
```

例子：写一个函数模板，要求传入对象的类型是 int

```cpp
#include <type_traits>

//定义概念
template<typename T>
concept Int = std::is_same_v<T, int>;

//概念作为模板参数类型
template<Int T>
void f(T){}

//概念配合requires子句
template<typename T>
    requires Int<T>
void f(T){}

//直接使用requires子句
template<typename T>
    requires std::is_same_v<T, int>
void f(T){}
```

例子：写一个函数模板，要求传入的对象支持 `+`

```cpp
//定义概念（用requires表达式）
template<typename T>
concept Addable = requires(T a) { 
    a + a;
};

//概念作为模板参数类型
template<Addable T>
auto f(const T& a, const T& b) {
    return a + b;
}

//概念配合requires子句
template<typename T>
    requires Addable<T>
auto f(const T& a, const T& b) {
    return a + b;
}

//requires子句配合requires表达式
template<typename T>
    requires requires(T a) {a + a;}
void f(T){}
```

例子：概念可以约束 auto

```cpp
#include <concepts>

auto square(std::integral auto x) {
    return x * x;
}
```

# 并发编程

## std::thread

### 构造函数

`std::thread`(C++11) 线程对象是独占所有权的，只能移动、不能拷贝

```cpp
template<typename F, typename...Args>
explicit thread(F&& f, Args&&...args);
```

std::thread 构造函数的传参，传的是实参的副本，如果想传引用需要用 `std::ref()` 包装

在 MSVC STL 的实现中，所有参数会经过完美转发构造一个叫 decay_copied  的元组，然后创建线程，通过形参包遍历这个元组，每个元素 `std::move()` 成右值传给 `std::invoke()` ，由 `std::invoke()` 对可调用对象进行调用

建议：只传 lambda 表达式，在 lambda 表达式中调用对应的函数，函数参数用 lambda 表达式捕获

### 执行策略

`join()`：等待关联线程执行完毕

`detach()`：放弃关联线程的所有权，不再管理此线程，让其独立运行

在线程对象的生存期结束之前，必须决定它的执行策略是合并还是分离，否则会调用 `std::terminate()` 终止程序

`joinable()`：线程对象是否有关联活跃线程。`joinable()` 返回 true 才能 `join()` 或 `detach()`

## std::this_thread

`std::this_thread` 命名空间中有一些常用的函数：

- `get_id()`：获取当前线程的 id
- `yield()`：让出时间片
- `sleep_for()`：使当前线程停止一段时间
- `sleep_until()`：使当前线程停止到指定的时间点

## thread_local

thread_local 用于声明具有线程存储期的变量，也叫线程局部存储

## 数据竞争

一个线程正在写，另一线程读或写同一内存位置时，会发生数据竞争，除非操作是原子的或者操作有固定的顺序

数据竞争是未定义行为（即程序的结果是不确定的），访问共享资源时，要注意数据竞争的问题

## 互斥锁

互斥锁获取失败时，线程会进入阻塞然后切换

### mutex

`std::mutex`(C++11)

`std::recursive_mutex`(C++11)：允许一个线程多次锁定，一般用于递归

### shared_mutex

`std::shared_mutex`(C++17)：允许独占锁定和共享锁定，可以用来实现读写锁

### 管理类

`std::lock_guard`(C++11)：独占锁定，不能拷贝和移动

`std::scoped_lock`(C++17)：用来代替 `std::lock_guard`，可以一次独占锁定多个锁

`std::unique_lock`(C++11)：独占锁定，可以移动

`std::shared_lock`(C++14)：共享锁定，可以移动

## 自旋锁

自旋锁获取失败时，线程不会进入阻塞然后切换，而是进行忙等待，直到获取到锁

自旋锁适合锁的持有时间很短的场景，可以减少线程切换的开销，如果锁的持有时间较长，自旋锁会导致 CPU 资源浪费

## 读写锁

读写锁允许多个读线程同时持有锁，在读多于写的场景下，使用读写锁可以提高读的并发性

根据实现的不同，读写锁可以分为读优先锁、写优先锁和公平读写锁，公平读写锁不会出现饥饿的现象

读写锁可以根据场景来选择用互斥锁还是自旋锁进行实现

```cpp
class Example {
private:
    std::map<std::string, std::string> data_;
    mutable std::shared_mutex mtx_; // mutex 一般为 mutable 否则 const 成员函数中无法修改 mutex 状态

public:
    void set(const std::string& key, const std::string& value) {
        std::lock_guard<std::shared_mutex> lock{mtx_};  //独占锁定
        data_[key] = value;
    }

    std::string get(const std::string& key) const {
        std::shared_lock<std::shared_mutex> lock{mtx_}; //共享锁定
        auto it = data_.find(key);
        return it != data_.end() ? it->second : "";
    }
};
```

## 乐观锁

前面的互斥锁、自旋锁、读写锁都属于悲观锁，悲观锁认为多线程冲突概率很高，在访问共享资源前都需要先加锁

乐观锁先假设多线程没有冲突，等线程修改完共享资源，再验证这段时间有没有其他线程修改了资源，如果没有则成功，否则放弃本次操作、后面再重试

乐观锁全程没有加锁，但是一旦发生冲突，重试的成本非常高，所以只有在冲突概率很低且加锁成本很高的场景，才考虑用乐观锁

使用乐观锁的例子有 Git、多人在线文档等等

## 锁的使用策略

1. 锁的粒度尽可能小（减少临界区大小、共享资源分桶等）

2. 根据场景使用合适的锁

## 条件变量

条件变量（也叫管程），一般与互斥锁一起使用，允许线程在条件不满足时释放锁并等待，直到另一个线程使条件满足并唤醒等待的线程，被唤醒的线程将重新持有锁

C++11 提供了两种条件变量：

- `std::condition_variable`：只能与 `std::unique_lock<std::mutex>` 搭配使用
- `std::condition_variable_any`：可以与任何锁搭配使用

例子：实现一个阻塞队列（阻塞队列常用于实现生产者消费者模型）

```cpp
template<typename T>
class BlockingQueue {
    mutable std::mutex mtx_;
    std::condition_variable cv_;
    std::queue<T> q_;

public:
    void push(const T& val) {
        {
            std::lock_guard<std::mutex> lock(mtx_);
            q_.push(val);
        }
        cv_.notify_one();
    }

    T pop() {
        std::unique_lock<std::mutex> lock(mtx_);
        cv_.wait(lock, [this]{return !q_.empty();});
        auto res = q_.front();
        q_.pop();
        return res;
    }

    bool empty() const {
        std::lock_guard<std::mutex> lock(mtx_);
        return q_.empty();
    }
};
```

例子：两个线程交替打印 1 - 10

```cpp
int x = 1;
bool flag = true;
std::mutex mtx;
std::condition_variable cv;


std::thread t1([&]{
    for (int i = 0; i < 5; i++) {
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, [&]{return flag;});
        std::cout << x << ' ';
        ++x;
        flag = false;
        cv.notify_one();
    }
});

std::thread t2([&]{
    for (int i = 0; i < 5; i++) {
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, [&]{return !flag;});
        std::cout << x << ' ';
        ++x;
        flag = true;
        cv.notify_one();
    }
});

t1.join();
t2.join();
```

## std::future

std::future 可以获取任务的返回值

### std::async

std::async 是一个函数模板，可以启动一个异步任务，返回一个 std::future 对象

传参方式类似于 std::thread 的构造函数，任务的返回值保存在返回的 std::future 对象中

### std::packaged_task

std::packaged_task 是一个类模板，与 std::function 类似，可以存储一个可调用对象

区别在于：std::packaged_task 只能移动、不能拷贝，并且 std::packaged_task 可以通过关联的 std::future 对象获取任务的返回值

```cpp
auto f = []{};
std::function<void()> func = f;
std::packaged_task<void()> task = f;

std::jthread t1{f};    //ok
std::jthread t2{func}; //ok
std::jthread t3{task}; //error
std::jthread t4{std::move(task)}; //ok

auto g = []{ return 0; };
std::packaged_task<int()> task2 = g;
auto fu = task2.get_future();
std::jthread t{std::move(task2)};
std::cout << fu.get();
```

## std::atomic

std::atomic 有两个功能：原子操作、内存序

原子操作：不可分割的操作。std::atomic 的成员函数都是原子的

内存序：保证操作之间的顺序

## 线程池

```cpp
class ThreadPool {
    using task_t = std::function<void()>;

public:
    ThreadPool(int numThreads = std::thread::hardware_concurrency()) : isRunning_{true} {
        int maxNum = std::thread::hardware_concurrency();
        int minNum = 2;
        numThreads = std::min(numThreads, maxNum);
        numThreads = std::max(numThreads, minNum);

        numThreads_ = numThreads;
        start();
    }

    ~ThreadPool() {
        stop();
    }
    
    void start() {
        for(int i = 0; i < numThreads_; ++i) {
            threads_.emplace_back([this]{
                while(isRunning_) {
                    task_t task;
                    { //consume a task from taskQueue_
                        std::unique_lock<std::mutex> lock{mtx_};
                        cv_.wait(lock, [this]{
                            return !isRunning_ || !taskQueue_.empty();
                        });
                        if(taskQueue_.empty()) {
                            return;
                        }
                        task = taskQueue_.front();
                        taskQueue_.pop();
                    }
                    task();
                }
            });
        }
    }
    
    void stop() {
        isRunning_ = false;
        cv_.notify_all();
        for(auto& t : threads_) {
            if (t.joinable()) {
                t.join();
            }
        }
        threads_.clear();
    }

    template<typename F, typename...Args>
    auto submit(F&& f, Args&&...args) {
        using RT = std::invoke_result_t<std::decay_t<F>, std::decay_t<Args>...>;
        if (!isRunning_) {
            throw std::runtime_error("ThreadPool is stopped");
        }
        auto pt = std::make_shared<std::packaged_task<RT()>>(std::bind(std::forward<F>(f), std::forward<Args>(args)...));
        auto fu = pt->get_future();
        { //produce a task to taskQueue_
            std::lock_guard<std::mutex> lock{mtx_};
            taskQueue_.emplace([pt]{
                (*pt)();
            });
        }
        cv_.notify_one();
        return fu;
    }
    
private:
    std::vector<std::thread> threads_;
    int numThreads_;
    
    std::queue<task_t> taskQueue_;
    std::mutex mtx_;
    std::condition_variable cv_;
    
    std::atomic_bool isRunning_;
};
```

## 协程

协程是可以暂停和恢复的函数（C++20）

只要函数定义包含 `co_await`、`co_yield`、`co_return` 三个关键字的任意一个，这个函数就是协程

# 常见手撕

## 模拟实现strstr

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
        if(success) {
            return s + i;
        }
    }
    return {};
}
```

## 模拟实现memcpy

```C++
void* my_memcpy(void* dst, const void* src, size_t n) {
    //1. 考虑内存重叠
    //src <= dst && src + n - 1 >= dst 从后往前
    //否则从前往后

    //2. 不考虑重叠 直接从前往后即可
}
```

# 常见需求

## 单例模式

单例模式可以保证一个类只有一个实例，并提供一个全局访问点来访问这个实例

```cpp
class Singleton {
    //构造函数私有
    Singleton() = default;

public:
    //提供全局访问点
    static Singleton& getInstance() {
        static Singleton instance; //C++11保证静态局部变量的初始化是线程安全的 
        return instance;
    }
    
    //弃置拷贝和移动
    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;
    Singleton(Singleton&&) = delete;
    Singleton& operator=(Singleton&&) = delete;
};
```

## 工厂模式

工厂模式可以将对象的创建和具体的业务逻辑分离，降低耦合度，提高可扩展性

简单工厂模式：一个工厂创建多种对象，根据传入的参数创建

工厂方法模式：工厂父类提供一个公共接口，每个工厂子类只创建一种对象

抽象工厂模式：工厂父类可以提供多个接口，每个工厂子类可以创建多种对象

## 模拟实现split

```C++
std::vector<std::string> split(const std::string& s, char sep = ' ') {
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

## 生成随机数

```cpp
thread_local std::mt19937 gen(std::random_device{}()); //线程安全

int randInt(int min, int max) {
    return std::uniform_int_distribution<int>(min, max)(gen);
}

float randFloat(float min, float max) {
    return std::uniform_real_distribution<float>(min, max)(gen);
}
```

# C++14

二进制字面量、返回类型推导、泛型 lambda、变量模板、std::make_unique

# C++17

结构化绑定、if 初始化语句、编译期 if、折叠表达式、CTAD、std::filesystem、std::string_view、std::optional、std::variant、std::any

# C++20

概念、协程、模块、范围
