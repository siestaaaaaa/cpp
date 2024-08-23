> C++ 是一门全能的编程语言，非常庞大而复杂。我们只需要使用它的一个子集，完成自己的需求就好，而不必过分纠结 C++ 本身的复杂性
>
> 参考：C++ primer、Effective Modern C++、C++ 并发编程实战、cppreference

# 基础知识

## C和C++

C 和 C++ 是两门不同的编程语言，C++ 最早是 C with class，现在 C++ 已经发展成了一个语言联邦

C 是面向过程的，C++ 除了面向过程，还支持面向对象、泛型编程、函数式编程

C++ 只兼容 C 的一个子集，现代 C 的很多特性 C++ 并不支持

## 基本类型大小

C++ 规定：char 最小 8 位，short 最小 16 位，int 最小 16 位，long 最小 32 位，long long 最小 64 位

float 一般是 IEEE-754 32 位格式，double 一般是 IEEE-754 64 位格式，long double 由实现定义

指针类型的大小一般为当前系统的位宽

## 整数的存储

为了将加法和减法统一成一种运算，计算机中的负数是补码表示的，减去一个数等同于加上这个数的补码

有符号数的最高位是符号位，无符号数的所有位都是数值位

- char、signed char、unsigned char 是三个不同的类型，char 的符号性由实现定义
- 有符号数溢出是未定义行为
- 无符号数溢出，结果相当于对它能表示的数值总数取模

## IEEE-754

IEEE-754 是一种浮点数存储格式：符号位 - 指数位 - 尾数位

- 32 位（1 - 8 - 23），64 位（1 - 11 - 52）
- 指数位越长，能表示的数值范围越大
- 尾数位越长，精度越高
- 指数可能是负数，为了统一表示（而不是使用符号位和补码），指数位保存时要加上一个偏移量（127 / 1023）
- 科学计数法的整数部分总是 1，所以尾数位只保存小数部分，可以节省一位空间，提高精度

误差：尾数位是有限的，对于无法用有限位数表示的数（如循环小数、无理数等），只能用近似值来表示

## 字节序

大端：高位字节存低地址，低位字节存高地址

小端：低位字节存低地址，高位字节存高地址

```cpp
// 判断当前机器的字节序
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

内存对齐是指数据在内存中的起始地址符合特定的对齐要求

每个类型都有自己的对齐要求，可以用 alignof 获取、alignas 设置

内存对齐的原因：

1. 平台原因：有些平台不能访问非对齐的内存
2. 性能原因：访问对齐的内存，缓存命中更高

## 内联函数

内联函数是编译器的一种优化，可以将函数在调用处展开，避免函数调用的开销，同时对缓存更友好

缺点：可能导致更大的可执行文件（代码膨胀）

内联函数和宏的区别：宏只是文本替换，没有类型检查，内联函数有类型检查

## inline

C++ 的 inline 最早是建议编译器内联，实际是否内联还是由编译器决定

而在现代 C++ 中，inline 的作用是允许一个符号在不同编译单元多次定义，链接时只取一份定义，一般用来修饰头文件中定义的全局变量或函数。**函数模板**和类中定义的函数是隐式 inline 的

如果想指示编译器启用内联优化，应该使用编译器的扩展，而不是标准 C++ 的 inline 关键字

## static

static 修饰局部变量：

- 延长生命周期（静态存储期）
- 首次访问时初始化，只会被初始化一次
- C++11 起，静态局部变量的初始化是线程安全的

static 修饰全局变量或函数：

- 内部链接，可以用来防止命名冲突
- 内部链接的还有匿名命名空间、const 全局变量

static 修饰类的成员变量：

- 静态成员变量不与对象绑定，需要单独定义，相当于类作用域限制的全局变量
- 可以直接用类名访问
- static 关键字只用于类中声明

static 修饰类的成员函数：

- 静态成员函数不与对象绑定，没有 this，相当于类作用域限制的普通函数
- 可以直接用类名访问
- static 关键字只用于类中声明

## const

const 表示不能修改，是一种语义上的约束，保证运行时不被修改

const 可以修饰变量、返回值、函数形参、成员函数等

## volatile

volatile 可以阻止编译器优化，保证总是从内存中读取

标准 C++ 的 volatile 不是原子的，且没有内存定序，不能用于多线程同步

## 引用和指针

引用是变量的别名，指针是用来存储地址的变量

引用不能改变指向，指针可以（注：C++ 的引用不能改变指向，这点与很多语言不同）

没有多级引用，但有多级指针

没有空引用，但有空指针

## #include<>和#include""

`<>` 直接去系统路径查找，一般用于包含标准库头文件

`""` 先在当前目录查找，找不到再去系统路径查找，一般用于包含自定义头文件

头文件包含就是将头文件的内容直接拷贝进来

## 重复包含

重复包含是指头文件在一个编译单元中被包含多次

可以使用条件编译（`#ifndef` `#define` `#endif`）或 `#pragma once` 来避免重复包含

## 函数重载

相同作用域中，形参不同的同名函数构成重载

不同作用域的同名函数，局部优先，而非重载

函数重载的原理：符号修饰

## extern"C"

C++ 的符号修饰和 C 不同，如果需要 C/C++ 代码互调，就需要使用 `extern"C"` 指示编译器按照 C 的方式处理符号

## 源文件到可执行文件的过程

每个源文件都是一个编译单元，每个编译单元被独立编译成一个目标文件，由链接器将所有目标文件链接成一个可执行文件

源文件到可执行文件的过程可以分为四个阶段：

- 预处理：执行预处理指令，比如宏替换、头文件展开、条件编译
- 编译：词法分析、语法分析、语义分析、代码优化，生成汇编代码
- 汇编：将汇编代码翻译成二进制代码，生成目标文件
- 链接：将所有目标文件和相关的库链接成可执行文件

## 静态链接和动态链接

静态链接会把库代码（.a 或 .lib）直接拷贝到可执行文件

- 优点：程序运行时不需要外部库文件
- 缺点：
  - 可执行文件较大，浪费硬盘空间
  - 如果有多个进程使用相同的库，每个进程都会包含一份库的副本，浪费内存空间
  - 更新库文件后，整个程序需要重新编译

动态链接只记录一些元信息，库文件在程序运行时才载入内存

- 优点：
  - 可执行文件较小，节省硬盘空间
  - 多个进程可以共享同一份库，节省内存空间
  - 更新库文件后，替换对应的库文件即可
- 缺点：运行时需要外部库文件

## new和delete的原理

new 先调 operator new() 分配内存（分配失败会抛 std::bad_alloc 异常），再调构造函数构造对象

delete 先调析构函数析构对象，再调 operator delete() 释放内存

operator new()、operator delete() 这两个函数是可以重载的，可以全局重载也可以按类重载

## new和delete的使用

new 一个元素：new T{} - delete

new 多个元素：new T[n]{} - delete[]

- new T[n]{} 先分配 n 个元素大小的连续内存，再对每个元素调用构造函数
- delete[] 先对每个元素调用析构函数，再释放 n 个元素大小的连续内存

这些运算符必须配对使用，否则是未定义行为

## placement new

new(address) T{}

placement new 是 operator new() 的一个重载，用于在已分配的内存上构造对象（需手动调析构函数析构对象）

```cpp
char buf[sizeof(T)];
auto p = new(buf)T{};
// ...
p->~T();
```

## new和malloc的区别

new 是 C++ 运算符，malloc 是 C 库函数

- new 支持初始化，malloc 不支持
- new 不用手动传入分配大小，malloc 要
- 分配成功，new 返回类型指针，malloc 返回 void*
- 分配失败，new 抛 std::bad_alloc 异常，malloc 返回空指针

## 内存泄漏

内存泄漏是指程序没有释放已经不再使用的内存，导致这部分内存不能再次被使用

- 对于长期运行的程序，内存泄漏会导致系统可用的内存越来越少，最后 OOM
- 如何避免：RAII、智能指针
- 检测工具：AddressSanitizer（GCC/Clang）、​Valgrind​（Linux/macOS）
- 手动检测：重载 operator new() 和 operator delete()，记录每次内存分配的信息，释放时从记录中删除，程序退出时输出记录中的信息

## 惯用手法

### CRTP

CRTP 是模板编程的一种惯用手法，就是写一个基类模板，派生类继承时将自身作为模板参数传递给基类

CRTP 可以实现静态多态：在基类暴露一个方法，在这个方法中 this 向下转换调用派生类的实现

```cpp
template<typename T>
class Base {
public:
    void func() {
        static_cast<T*>(this)->impl();
    }

    // 默认实现
    void impl() {
        std::cout << "Base\n";
    }
};

class X : public Base<X> {
public:
    // 派生类的实现
    void impl() {
        std::cout << "X\n";
    }
};

class Y : public Base<Y> {
public:
    // 派生类的实现
    void impl() {
        std::cout << "Y\n";
    }
};

template<typename T>
void f(Base<T>& r) {
    r.func();
}

int main() {
    X x;
    Y y;
    // 多态：不同类型的对象 调用同一个接口 产生不同的行为
    f(x);
    f(y);
}
```

### PIMPL

PIMPL 是库开发者常用的编程技巧，可以：

1. 隐藏实现细节
2. 减少编译依赖，提高编译速度（现在更好的是用预编译头或模块）
3. 维持稳定的 ABI（类的内存布局和函数签名）

```cpp
// A.h
class A {
public:
    void f();
    
private:
    class AImpl;
    AImpl* impl_; //指向实现的指针
};


// A.cpp
#include "A.h"
// 包含需要的头文件

class A::AImpl {
public:
    void f() {
        // ...
    }
};

void A::f() {
    impl_->f();
}
```

缺点：调用方法多一次跳转，有一定的性能损失

### RAII

RAII 是什么：

- 把资源和对象的生命周期绑定，在构造函数获取资源、析构函数释放资源
- 当异常抛出时，C++ 会自动调对象的析构函数（栈回溯）

RAII 可以保证对象的生命周期结束时，持有的资源都能正确释放

## 栈回溯

栈回溯就是 C++ 抛出异常或函数返回时，逆序销毁局部对象

如果没有捕获抛出的异常，C++ 会自动调用 std::terminate 终止程序，是否进行栈回溯由实现定义

## 池

池是一种预先创建并复用资源的技术，用户从池中获取资源，用完归还到池中，可以减少资源创建销毁的开销（空间换时间）

常见的池有内存池、对象池、进程池、线程池、连接池等

# STL

六大组件：容器、迭代器、算法、函数对象、适配器、分配器

## 容器

### std::array

定长数组，开辟在栈上

- 是对原生数组的封装
- 没有额外的空间开销

### std::vector

动态数组，在堆上分配一块连续空间来存储元素

- 随机访问 $O(1)$，尾部插入删除均摊 $O(1)$（得益于扩容的倍增策略），任意位置插入删除 $O(n)$（需要挪动数据）
- 扩容机制：插入元素时如果容量已满，就开辟一块更大的空间，将数据拷贝/移动到新空间，释放旧空间
- 扩容倍率：gcc 2 倍，msvc 1.5 倍，建议 reserve 预分配避免频繁扩容
- 迭代器失效：插入可能使所有迭代器失效（扩容），删除使后续迭代器失效
- std::vector 本身一般需要三个指针的空间

reserve 和 resize 的区别：

- reserve 是预分配内存，保证容量不小于 n，不会改变 size
- resize 是调整 size，为此可能需要扩容、添加元素或删除元素

### std::list

带头双向循环链表，结点按需在堆上分配，没有空间浪费

- 随机访问 $O(n)$，任意位置插入删除 $O(1)$
- 迭代器失效：只有被删除元素的迭代器失效
- 每个结点需要两个指针的额外空间，来维持双向链表结构
- std::list 本身需要一个 dummy 指针和一个 size

和 vector 的区别：参考数组和链表的区别

### std::deque

双端队列，分块连续存储，通过中控数组管理多块内存

- 随机访问 $O(1)$（略差于 std::vector），头尾插入删除 $O(1)$​，任意位置插入删除 $O(n)$（只需挪动较少的一半数据）
- 是 std::stack 和 std::queue 的默认容器

## 迭代器

迭代器就是指针的抽象，给容器提供了一套统一的访问方式，同时将算法与容器解耦

- Input Iterator：只读
- Output Iterator：只写
- Forward Iterator：++
- Bidirectional Iterator：++、--
- Random Access Iterator：支持所有指针运算

## 算法

### std::sort

接受 Random Access Iterator，对区间进行排序，默认升序

正常用快排，小区间用插排，递归过深用堆排

## 函数对象

函数对象就是重载了 ```operator()``` 的类对象

谓词：返回 bool 的函数对象

## 适配器

适配器就是将容器封装成另一种数据结构，只提供必要的接口

- std::stack 将顺序容器封装成栈
- std::queue 将顺序容器封装成队列
- std::priority_queue 将顺序容器封装成堆，默认使用 std::vector 容器构建大顶堆

## 分配器

std::allocator 可以将容器和内存管理解耦

- allocate：调 operator new() 分配内存
- construct：调 placement new 构造对象
- destroy：调析构函数释放析构对象
- deallocate：调 operator delete() 释放内存

# 面向对象

## 封装

封装就是把数据和操作数据的方法捆绑在一起，隐藏实现细节，选择性对外提供接口

封装可以降低代码的耦合度、提高访问的安全性

## struct和class

C++ 的 struct 和 class 都是类

区别：

- struct 的成员访问默认是 public，class 的成员访问默认是 private
- struct 的继承方式默认是 public，class 的继承方式默认是 private

## this

对象调用成员函数时，会隐式将自己的地址传给成员函数，这个隐藏的形参就是 this（一般存在寄存器里）

成员函数会通过解引用 this 来访问对象的成员

```cpp
class A {
    int x;
    void f() {
        x = 1; // this->x = 1;
    }
};
```

## 成员函数的cv限定

成员函数的 cv 限定，限定的是 this 指向的对象，不同 cv 限定的函数可以重载

const 成员函数可以被 const 对象和非 const 对象调用，非 const 成员函数只能被非 const 对象调用

如果成员函数重载了 const 版本和非 const 版本，const 对象会匹配到 const 版本，非 const 对象会匹配到非 const 版本

因此：

- 如果一个成员函数不修改对象的状态，使用 const 限定可以提高代码的可读性和健壮性
- 如果一个成员函数对于 const 对象和非 const 对象有不同的行为，则应重载 const 版本和非 const 版本

## 继承

继承是建立类之间关系的一种方式，比如 public 继承的派生类和基类是 is-a 关系

继承可以提高代码的复用性、构建清晰的层次关系，同时为多态提供基础

## 组合

组合就是一个类包含另一个类的对象，是 has-a 的关系

继承的耦合度高，组合的耦合度低，类之间是 is-a 的关系可以用继承（构建层次关系），否则优先使用组合（功能插件化）

## 派生类和基类

向上转换（切片）：派生类对象/指针/引用可以隐式转换成基类对象/指针/引用

向下转换：基类指针/引用转换为派生类指针/引用

虚基类：被虚继承的基类叫虚基类，虚基类在最终派生对象中仅含一个子对象

## 显式转换

C++ 的显式转换有四种：

- static_cast 用于基本的转换，允许向下转换但是没有运行时检查
- dynamic_cast 用于安全的向下转换，不合法会返回空指针/抛出异常
- const_cast 用于添加或移除 const 属性
- reinterpret_cast 用于重新解释底层的转换，比如指针类型和整型之间的转换、不同指针类型之间的转换等

C++ 的显式转换相比 C 的显式转换有更好的安全性和可读性

## explicit

explicit 表示显式，不允许隐式转换

explicit 可以修饰构造函数、转换函数

## 转换函数

转换函数是一种成员函数，可以启用从类类型到另一类型的转换

```cpp
struct X {
    operator T(){}          // X 可以隐式或显式转换为 T
    explicit operator T(){} // X 可以显式转换为 T
};
```

## 静态类型和动态类型

静态类型就是声明的类型，是编译期确定的

动态类型就是实际的类型，是运行期确定的

只有基类指针/引用的静态类型和动态类型可能不同

判断静态类型是否相同可以用 `std::is_same_v<A, B>`，判断动态类型是否相同可以用 `typeid(A) == typeid(B)`

## 虚函数

以 virtual 声明的成员函数叫做虚函数

- 虚函数可以在派生类重写：如果派生类成员函数拥有和基类虚函数完全相同的接口，那么该函数也是虚函数并且会覆盖基类的虚函数
- **当使用基类指针或引用调用虚函数时，会发生动态绑定**，根据对象的实际类型调用对应的函数

虚函数怎么实现的

- 有虚函数的类会有一张虚函数表，派生类会继承基类的虚函数表，如果派生类重写了基类虚函数，那么派生类虚函数表中对应的函数地址会被覆盖
- 有虚函数表的类，对象的头部会存一个指针指向虚函数表，虚函数调用会通过这个指针找到虚函数表，然后查虚函数表确定要调用函数的地址
- 虚函数表在编译期生成，存在数据段

## 虚函数可以是内联函数吗

虚函数可以被 inline 修饰，是否内联由编译器决定，当虚函数是动态绑定（通过基类指针或引用调用虚函数）的时候不会被内联优化，因为动态绑定在运行时才确定要调用的函数

顺带一提，inline 最早是建议编译器内联，但在现代 C++ 中，inline 的作用是允许一个符号在不同编译单元多次定义，链接时只取一份定义，一般用来修饰头文件中定义的全局变量或函数

## 多态

多态就是不同类型的对象调用同一接口，产生不同的行为

多态可以给同一接口提供灵活的实现

C++ 的多态分为静态多态和动态多态

- 静态多态有重载、模板、CRTP
  - 静态多态没有虚函数调用的运行时开销，没有虚表指针的空间开销，并且编译器可以进行内联优化
  - 静态多态适用于编译时对象类型已知且需要高性能的场景
- 动态多态是通过虚函数来实现的
  - 动态多态在运行时才确定要调用的函数，编译器无法内联优化，且每次调用都需要通过虚函数表跳转，对 cache 不友好
  - 动态多态每个对象都需要额外存储一个指向虚函数表的指针
  - 动态适用于运行时才确定对象类型的场景（比如程序行为依赖用户输入的时候）

## 对象的内存布局

如果类中有虚函数，对象的头部会存一个虚表指针

剩下按类继承顺序和成员声明顺序来布局，遵循内存对齐

C++ 要求每个对象都要有一个唯一的地址，所以空类的大小一般为 1 字节，但是空基类可以不占空间（空基类优化）

## 纯虚函数和抽象类

有纯虚函数的类叫做抽象类，抽象类不能实例化出对象，一般用作接口类

纯虚函数要求派生类必须重写，否则派生类也是抽象类

## 构造函数

如果没有显式定义（或弃置）构造函数/拷贝构造/移动构造，编译器会隐式定义一个默认构造函数

默认构造：不需要提供实参的构造函数（没有形参/每个形参都有默认值）

拷贝构造：形参类型一般为 const T&

移动构造：形参类型一般为 T&&

## 初始化列表

初始化列表是构造函数定义的一部分，会初始化类的所有非静态成员变量

成员的初始化顺序和声明顺序一致，与初始化列表的顺序无关

## 可以虚构造吗

不可以

虚函数的地址存在虚函数表，如果构造函数是虚函数就要到虚函数表去调用，但是对象还没构造就没有虚表指针找不到虚函数表，所以不允许虚构造

## 五个特殊成员函数

五个特殊成员函数：析构函数、拷贝构造、拷贝赋值、移动构造、移动赋值

- 如果没有显式定义（或弃置）析构函数，编译器会隐式定义一个析构函数
- 如果没有显式定义（或弃置）拷贝构造/移动构造/移动赋值，编译器会隐式定义拷贝构造
- 如果没有显式定义（或弃置）拷贝赋值/移动构造/移动赋值，编译器会隐式定义拷贝赋值
- 如果五个特殊成员函数都没有显式定义（或弃置），编译器会隐式定义移动构造和移动赋值

隐式定义的拷贝构造/拷贝赋值

- 对于自定义类型的成员调它的拷贝构造/拷贝赋值
- 对于基本类型的成员采用浅拷贝（只拷贝句柄，不拷贝底层资源）

隐式定义的移动构造/移动赋值：成员能移动则移动，否则拷贝

## 可以虚析构吗

基类的析构函数必须是虚函数，否则通过基类指针 delete 派生类对象是未定义行为

其他情况没必要虚析构，因为虚函数有额外开销（时间、空间）

## 构造或析构中调虚函数

构造或析构中进一步的派生类并不存在，虚函数调用不会下传到派生类，不能达到预期的动态多态的效果

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

lambda 表达式是一个匿名函数对象，可以捕获外部变量

定义一个 lambda 表达式，同时也隐式定义了一个重载 ```operator()``` 的类，捕获的变量就是这个类的成员变量，参数就是 `operator()` 的参数，函数体就是 `operator()` 的函数体

## std::bind

```cpp
auto newCallable = std::bind(callable, args);
```

std::bind 是一个函数模板，接收一个可调用对象，绑定某些参数，生成一个新的可调用对象

std::placeholders 命名空间中定义了一些形如 _n 的占位符，表示新的可调用对象的参数位置

现代 C++ 中建议使用 lambda 表达式代替 std::bind，语法更简洁、强大

## std::function

```cpp
std::function<ret(args)> f = callable;
```

std::function 是一个类模板，可以存储一个可调用对象，通常用于回调的场景

## 左值和右值

左值和右值是表达式的一种属性，左值可以取地址，右值不能取地址

- 常见的左值：变量、字符串字面量、前置++/--、返回左值引用的函数调用、赋值表达式、解引用表达式
- 常见的右值：临时量、后置++/--、传值返回的函数调用、lambda 表达式、this；返回右值引用的函数调用、转换到右值引用的表达式

左值引用只能绑定左值，右值引用只能绑定右值，const 左值引用和万能引用既可以绑定左值也可以绑定右值

引用变量是左值，不要混淆类型和值类别，它们是两个独立的属性

## 智能指针

智能指针是 RAII 的一种应用，可以管理对象的生命周期，防止内存泄漏

### std::unique_ptr

独占指针，独占所管理的对象

常用成员函数：

```cpp
reset   //释放原对象 接管新对象
get     //返回对象指针
operator bool //是否接管对象
release //返回对象指针 释放所有权
get_deleter   //返回删除器
```

相比共享指针，独占指针无需维护引用计数，性能更优，一般优先使用独占指针

### std::shared_ptr

共享指针，共享所管理的对象，只能通过共享指针的拷贝构造或拷贝赋值来共享所有权，用共享指针持有的原始指针创建新的共享指针是未定义行为

常用成员函数：

```cpp
reset   //释放原对象 接管新对象
get     //返回存储的指针（不一定是被管理对象的指针）
operator bool //是否接管对象
use_count     //返回对象关联的共享指针个数
```

在典型实现中，共享指针会保有两个指针：get() 返回的指针和指向控制块的指针。控制块才是对象的管理者，控制对象的生命周期

控制块中包含了被管理对象、共享指针计数、弱指针计数，以及分配器、删除器

- 共享指针计数为 0 时，自动销毁被管理对象
- 弱指针计数为 0 时，自动销毁控制块
- 共享指针也会参与弱指针计数

共享指针不是线程安全的，控制块是线程安全的

### std::weak_ptr

弱指针是共享指针的弱引用，不控制对象的生命周期，访问对象前必须转换为共享指针（临时所有权）

在典型实现中，弱指针只保有一个指向控制块的指针，与共享指针共享控制块，参与弱指针计数

常用成员函数：

```cpp
reset     //放弃原对象 接管新对象
use_count //返回对象关联的共享指针个数
expired   //检查对象是否存在
lock      //尝试创建一个共享指针
```

弱指针还可以用来解决共享指针的循环引用问题（共享指针计数无法抵达 0 导致内存泄漏）

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
//两个共享指针销毁
//但由于循环引用 两个结点没有销毁

//解决方法: 令环中的共享指针之一为弱指针
struct Node {
    std::weak_ptr<Node> prev_;
    std::weak_ptr<Node> next_;
};
```

### std::make_shared

使用构造函数创建共享指针，控制块和被管理对象只能被分别创建

使用 std::make_shared 创建共享指针，会一次性创建控制块和被管理对象，性能更好

### std::enable_shared_from_this

std::enable_shared_from_this 是一个类模板，允许派生类通过 `shared_from_this()` 生成指向自己的共享指针，生成的共享指针和已有的共享指针共享控制块

而如果直接用 this 构造共享指针，被管理对象可能存在多个控制块，会有 double-free 的问题

使用示例：

```cpp
class X : public std::enable_shared_from_this<X> /*将自身作为基类模板的参数*/ {
public:
    std::shared_ptr<X> getSelf() {
        return shared_from_this();
    }
};
```

## nullptr

C++11 前，用 0 或 NULL 表示空指针，NULL 一般被定义成 0

0 既可以表示整数又可以表示空指针，在一些重载的场景中会有问题

nullptr 是专门用来表示空指针的，它的类型是一个独立的类型，用 nullptr 表示空指针更安全

## constexpr

constexpr 表示编译期求值，可以提高运行时效率，同时可以替代一些复杂的元编程魔法

const 只保证运行时不被修改，constexpr 是真正的编译期常量

## override

override 可以检查函数是否重写基类的虚函数

## final

final 可以防止一个类被继承或者防止一个虚函数被重写

## push和emplace

push 是先构造一个副本，然后拷贝或移动构造一个新元素

emplace 是直接构造一个新元素

## using

using 通常用来：

- 引入类型别名
- 引入命名空间
- 引入命名空间的成员

## 内联命名空间

内联命名空间会把成员导出到父命名空间

```cpp
inline namespace A{}
```

## 成员函数的引用限定

成员函数的引用限定会影响重载决议，& 限定的成员函数只能被左值调用，&& 限定的成员函数只能被右值调用

与 cv 限定不同，引用限定不会改变 this 的性质

# 模板编程

模板的语法可以理解成拼图

- 泛型编程：为一组类型编写一份代码
- 元编程：把类型当作一种数据

模板分为函数模板、类模板、变量模板（_v）、别名模板（_t）

## 模板实例化

模板必须实例化才会生成实际的代码（编译期）

- 显式实例化：显式声明或显式指定模板实参
- 隐式实例化：根据函数实参进行模板推导（模板推导不考虑隐式转换）

```cpp
template<typename T>
void f(T) {}

template void f<int>(int); //显式实例化 f<int>(int)

int main() {
    f(1);         //隐式实例化 f<int>(int)
    f<int>(1);    //显式实例化 f<int>(int)
}
```

## 模板参数

类型参数：参数是一个类型

- `template<typename T>`

非类型参数：参数是一个编译期常量

- `template<size_t N>`

## 模板特化

模板特化类似重载

- 全特化：给某个具体类型定制行为
- 偏特化：给一组类型定制行为

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

模板必须实例化才会生成实际的代码

如果声明和定义分别写在头文件和源文件，就没有隐式实例化了，链接会报错符号未定义

可以在模板定义的地方显式实例化，让编译器生成对应的代码，模板全特化有类似的作用

建议：模板直接定义在头文件，比如 boost 里的 .hpp

## 引用折叠

在模板或类型别名中，可能出现引用的引用，此时会触发引用折叠

- 右值引用的右值引用折叠成右值引用，其他情况都折叠成左值引用

## 万能引用

万能引用接收左值则为左值引用，接收右值则为右值引用

万能引用是通过模板推导和引用折叠实现的（模板推导会忽略实参的引用性，除了万能引用接收左值表达式的情况）

```cpp
//(1)
template<typename T>
void f(T&& x){}

//(2)
auto&& x = i;
```

## 完美转发

完美转发可以将接收到的实参原封不动地转发给另一个函数，保持实参的类型和值类别

```cpp
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

```cpp
template<typename T>
std::remove_reference_t<T>&& move(T&& param) {
    return static_cast<std::remove_reference_t<T>&&>(param);
}
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

SFINAE：当编译器进行模板推导时，替换失败不是错误，只是不选择这个重载

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

# 并发编程

## std::thread

std::thread 的构造函数默认是传值的，想传引用需要用 `std::ref()` 包装

```cpp
template<typename F, typename...Args>
explicit thread(F&& f/*可调用对象*/, Args&&...args);
```

在线程对象销毁之前，必须决定它的执行策略，否则 C++ 会自动调用 `std::terminate()` 终止程序

- `join()`：阻塞等待关联线程执行完毕
- `detach()`：不再管理关联线程，让其独立运行

`joinable()`：线程对象是否有关联线程

- `joinable()` 返回 true 才能 `join()` 或 `detach()`

## std::this_thread

`std::this_thread` 命名空间中有一些常用的函数：

- `get_id()`：获取当前线程的 id
- `yield()`：让出时间片
- `sleep_for()`：使当前线程停止一段时间
- `sleep_until()`：使当前线程停止到指定的时间点

## 数据竞争

一个线程正在写，另一线程读或写同一内存位置时，会发生数据竞争

- 数据竞争是未定义行为，程序的结果是不确定的
- 线程安全：多线程下，程序的执行结果是确定的

如何避免数据竞争

- 使用多线程同步设施：锁、条件变量、信号量
- 使用原子操作
- 使用线程局部存储
- 只读数据无需同步

检测工具：ThreadSanitizer（GCC/Clang）、Valgrind​（Linux/macOS）

## 互斥锁

互斥锁获取失败时，线程会阻塞切换

### mutex

`std::mutex`(C++11)

`std::recursive_mutex`(C++11)：递归锁，允许同一线程多次锁定

### shared_mutex

`std::shared_mutex`(C++17)：读写锁，允许独占锁定和共享锁定

### 管理类

`std::lock_guard`(C++11)：独占锁定，不能拷贝和移动

`std::scoped_lock`(C++17)：`std::lock_guard` 的上位替代，可以一次独占锁定多个锁

`std::unique_lock`(C++11)：独占锁定，可以移动

`std::shared_lock`(C++14)：共享锁定，可以移动

## 自旋锁

自旋锁获取失败时，线程不会阻塞切换，而是进行忙等待，直到获取到锁

自旋锁适合锁的持有时间很短的场景，可以减少线程切换的开销，如果锁的持有时间较长，会浪费 CPU 资源

## 读写锁

读写锁允许多个读线程同时持有锁，在读多于写的场景下，使用读写锁可以提高并发效率

读写锁分为读优先、写优先和完全公平，可以用互斥锁实现也可以用自旋锁实现

C++17 引入了 std::shared_mutex，支持完全公平的独占锁定和共享锁定（std::shared_lock），可以用来实现读写锁

pthread 库提供的读写锁是写优先的，要注意使用场景（如果写多于读，读线程可能饿死）

## 乐观锁

前面的互斥锁、自旋锁、读写锁都属于悲观锁，悲观锁认为多线程一定冲突，访问共享资源必须先加锁

乐观锁假设多线程没有冲突，等线程修改完共享资源，再验证这段时间有没有其他线程修改（通过版本号或时间戳），如没有则成功，否则回滚重试

乐观锁全程没有加锁，但是一旦发生冲突，重试的成本非常高，只有在冲突概率很低且加锁成本很高的场景，才考虑用乐观锁

使用乐观锁的例子有 Git、多人在线文档等等

## 锁的使用策略

1. 锁的粒度尽可能小（减少临界区大小、共享资源分桶等）
2. 根据场景使用合适的锁

## 条件变量

条件变量一般和互斥锁一起使用，允许线程在条件不满足时释放锁并等待，直到其他线程使条件满足，被唤醒的线程将重新获取锁

例子：实现一个阻塞队列（生产者消费者模型）

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
        cv_.wait(lock, [this]{return !q_.empty();}); // 等价 while(q_.empty()) cv_.wait(lock); 防止虚假唤醒
        auto res = q_.front();
        q_.pop();
        return res;
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

## std::async

std::async 是一个函数模板，可以启动一个异步任务，传参方式类似于 std::thread 的构造函数

任务的返回值保存在返回的 std::future 对象中，std::future 是一个类模板，提供一个 get 方法获取任务的返回值（阻塞等待）

## std::packaged_task

std::packaged_task 是一个类模板，与 std::function 类似，可以存储一个可调用对象

区别在于：std::packaged_task 只能移动、不能拷贝，并且可以通过关联的 std::future 对象获取任务的返回值

```cpp
auto f = []{};
std::function<void()> func = f;
std::packaged_task<void()> task = f;

std::jthread t1{f};    //ok
std::jthread t2{func}; //ok
std::jthread t3{task}; //error
std::jthread t4{std::move(task)}; //ok
std::jthread t4{std::ref(task)};  //also ok

auto g = []{ return 0; };
std::packaged_task<int()> task2 = g;
auto fu = task2.get_future();
std::jthread t{std::move(task2)};
std::cout << fu.get();
```

## std::promise

std::promise 是一个类模板，可以在某个线程中设置一个值，并在另一个线程中通过关联的 std::future 对象获取这个值

```cpp
std::promise<int> pm;
auto pm_val = pm.get_future();
auto ret_val = std::async([&pm]{
    pm.set_value(111);
    return "done";
});
std::cout << pm_val.get(); //111
std::cout << ret_val.get();//done
```

## std::atomic

std::atomic 可以提供原子操作和内存序控制

- 保证某个变量的操作是原子的（不可分割的）
- 保证线程间操作的可见性（CPU、缓存、编译器优化都可能导致指令乱序）

原子操作：

- 读/写：load、store
- 修改：exchange、fetch_add、fetch_sub、compare_exchange_weak、compare_exchange_strong
- CAS：`bool compare_exchange_weak(T& expected, T desired, std::memory_order success, std::memory_order failure)`
  - 如果变量的值等于 expected，则替换成 desired 并返回 true，否则将值写入 expected 并返回 false
  - weak 失败需循环重试，strong 保证成功（但会损失性能）

内存序：

- std::memory_order_relaxed：只保证原子，不保证可见性，一般用于独立的计数器
- std::memory_order_acquire：读操作，保证后面的读写不会重排到前面
- std::memory_order_release：写操作，保证前面的读写不会重排到后面
- std::memory_acq_rel：读-改-写操作，同时包含 acquire 和 release 语义
- std::memory_seq_cst：默认内存序，保证顺序一致，最安全但性能最低

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

# 常见手撕

## 只能在堆上创建对象的类

```cpp
class HeapOnly{
    HeapOnly(){}

public:
    static HeapOnly* create(){
        return new HeapOnly;
    }

    ~HeapOnly(){
        delete this;
    }

    HeapOnly(const HeapOnly&) = delete;
    HeapOnly& operator=(const HeapOnly&) = delete;
};
```

## 只能在栈上创建对象的类

```cpp
class StackOnly{
    StackOnly(){}

public:
    static StackOnly create(){
        return StackOnly{};
    }

    void* operator new(size_t) = delete;
    void* operator new[](size_t) = delete;
};
```

## 模拟实现strstr

```cpp
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

```cpp
void* my_memcpy(void* dst, const void* src, size_t n) {
    // 1. 考虑内存重叠
    // src <= dst && src + n - 1 >= dst 从后往前
    // 否则从前往后

    // 2. 不考虑重叠 直接从前往后即可
}
```

## 模拟实现split

```cpp
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

# 设计模式

设计模式是一些常见问题的经典解决方案，可以提高代码的可维护性、扩展性和复用性

## 基本原则

SOLID

- 单一职责：一个类只负责一个功能
- 开闭原则：对扩展开放，对修改关闭
- 里氏替换：子类必须能替换父类（is-a）
- 接口隔离：用户不应该被迫依赖它不需要的接口
- 依赖倒置：高层不应该依赖低层，二者都应依赖抽象

## 创建型

### 单例模式

单例模式可以保证一个类只有一个实例，并提供一个全局访问点来访问这个实例，一般用于一些全局管理类

```cpp
class Singleton {
    Singleton(){};

public:
    static Singleton& getInstance() {
        static Singleton instance; 
        return instance;
    }
    
    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;
};
```

### 工厂模式

工厂模式就是将对象创建封装在工厂中，与业务逻辑解耦

- 简单工厂模式：一个工厂创建多种对象，根据传入的参数判断对象类型
- 工厂方法模式：工厂父类提供一个公共接口，每个工厂子类只创建一种对象
- 抽象工厂模式：工厂父类提供多个接口，每个工厂子类可以创建多种对象

### 原型模式

原型模式就是通过克隆现有对象来创建新对象，当创建对象需要经过很多复杂计算时，使用原型模式可以快速生成同类对象，比如生成敌人

## 结构型

### 适配器模式

适配器模式可以将一个类的接口转换成另一个类的接口，使得不同接口的类可以协同工作

### 装饰器模式

装饰器模式可以在不修改原始类的情况下，动态添加新的行为，比如给角色加 buff

### 组件模式

组件模式就是将功能拆分成独立的组件，通过组合的方式挂载到对象上，比如道具的功能扩展

### 享元模式

享元模式就是将对象的内部状态设为共享，减少创建开销和内存占用，比如地形、树木的重复渲染

## 行为型

### 观察者模式

观察者模式可以定义一对多的依赖关系，当一个对象的状态改变，会自动通知所有依赖它的对象，一般用于事件系统

### 模板方法模式

模板方法模式就是在模板方法中定义逻辑骨架，在子类实现具体步骤（父类也可以提供默认实现）

```cpp
struct Base {
    // 模板方法
    void f() {
        step1(); // this->step1() 动态绑定
        step2();
        step3();
    }

    // 如果子类没有重写则调用默认实现
    virtual void step1() {
        std::cout << "Base::step1\n";
    }
    virtual void step2() {
        std::cout << "Base::step2\n";
    }
    virtual void step3() {
        std::cout << "Base::step3\n";
    }

    virtual ~Base(){} // 基类需要虚析构
};

struct A : Base {
    void step1() override {
        std::cout << "A::step1\n";
    }
    void step2() override {
        std::cout << "A::step2\n";
    }
    void step3() override {
        std::cout << "A::step3\n";
    }
};

struct B : Base {
    void step1() override {
        std::cout << "B::step1\n";
    }
    void step2() override {
        std::cout << "B::step2\n";
    }
    void step3() override {
        std::cout << "B::step3\n";
    }
};

int main() {
    Base* a = new A;
    a->f();
    Base* b = new B;
    b->f();
}
```

### 状态模式

状态模式就是给每个状态封装一个类，将状态实现与状态切换解耦

## MVC

MVC（Model-View-Controller）可以将数据、界面、逻辑解耦，一般用于 UI 系统

# C++14

返回类型推导、二进制字面量、变量模板、泛型 lambda、std::make_unique

# C++17

结构化绑定、if 初始化语句、编译期 if、折叠表达式、CTAD、std::filesystem、std::string_view、std::optional、std::variant、std::any

# C++20

概念、协程、模块、范围

## 概念

概念就是定义一套约束，用来修饰模板接口

和 SFINAE 一样，概念可以使模板报错更清晰，但比 SFINAE 更简单、强大

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

## 协程

C++ 的协程是可以暂停和恢复的函数，上下文保存在堆中

当函数中出现 `co_await`、`co_return`、`co_yield` 的任意一个，这个函数就是协程

co_await 一个对象，这个对象的类型需要实现三个函数

co_return 一个对象，这个对象的类型（协程的返回类型）需要包含一个内部类 promise_type，在 promise_type 中控制返回行为
