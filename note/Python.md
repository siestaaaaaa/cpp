# 概论

## 命令行启动

Windows：`python`

Mac 或 Linux：`python3`

退出：`exit()`

## 解释器

CPython：官方的 Python 解释器，由 C 语言开发

IPython：基于 CPython 的一个解释器，在交互方式上有所增强

PyPy：和 CPython 有所不同，PyPy 采用了 JIT 技术对 Python 代码进行动态编译（而非解释），可以显著提高 Python 代码的运行速度

Jython：运行在 Java 平台的 Python 解释器，可以把 Python 代码编译成 Java 字节码

IronPython：运行在 .Net 平台的 Python 解释器，可以把 Python 代码编译成 .Net 字节码

# 基础知识

## 代码块

Python 使用缩进来组织代码块

建议使用 4 个空格的缩进，可以在文本编辑器设置把 Tab 自动转换为 4 个空格，确保不混用 Tab 和空格

## 变量和对象

Python 的变量是对象的引用，对象是存储数据的实体

变量没有类型，对象才有类型

对变量赋值，其实是改变了变量的指向，而不是修改对象本身，除非对象是可变的（比如 list、dict、set 等）

没有变量引用的对象会被自动回收

## 数值类型

int：整数类型，无大小限制

float：浮点数类型，双精度浮点数

下取整除法：`//`

浮点数除法：`/`

## 布尔类型

bool：表示逻辑值，int 的子类，True 等价于 1，False 等价于 0

逻辑运算：`and`、`or`、`not`

在布尔上下文中，被视为 False 的对象有 False、None、零、空容器，其他对象将被视为 True

## 字符串类型

字符串：`'I am a str'` 或 `"I am a str"`

多行字符串：`'''I am a doc str'''` 或 `"""I am a doc str"""`

原始字符串（不转义）：`r'I am a raw str'` 或 `r"I am a raw str"`

C-Style 格式化字符串：`'%d%f%s' % (0, 0.0, 'I am a format str')`

## 编码

ASCII：美国的编码标准

Unicode：把所有语言都统一到一套编码

UTF-8：可变长编码，解决 Unicode 的空间浪费问题，兼容 ASCII

目前主流的编码方式：内存中统一使用 Unicode，保存或传输时转换为 UTF-8

## 循环

Python 有两种循环

`for Elem in Iterable:`

`while Condition:`

# 函数

## 返回值

默认返回 `None`

返回多个值，其实是返回一个 tuple

## 参数

必选参数、默认参数、可变参数

## 匿名函数

匿名函数是一个没有名字的函数对象

语法：`lambda` 后面跟 `参数`、`:` 和 `一个表达式`，表达式的结果就是匿名函数的返回值

## 闭包

## 装饰器


