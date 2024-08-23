# 概述

为什么学 Lua：热更

环境搭建：安装 Lua，使用 VSCode + Lua 插件（Tencent）

# 变量

Lua 变量默认是全局的

使用 `local` 可以将变量声明为局部

- 防止命名冲突
- 访问速度更快（局部变量存储在栈中，全局变量需要通过 _G 进行查找）
- 出作用域自动回收

# 模块

一个 Lua 文件就是一个模块，模块可以有返回值

`require`：加载模块，返回模块的返回值（只会加载一次，除非删除缓存）

# 注释

```lua
--单行注释

--[[
多行注释
]]
```

# 分支语句

```lua
--单分支
if expr then

end

--双分支
if expr then

else

end

--多分支
if expr then

elseif expr1 then

elseif expr2 then

elseif expr3 then

else

end
```

# 循环语句

```lua
--while
while expr do

end

--repeat
repeat

until expr

--for
for i = left, right, stride--[[默认为1]] do

end
```

# 类型

Lua 是一门动态类型语言，变量没有类型，值才有类型

值类型：nil、number、boolean、string

引用类型：function、table、thread、userdata

# nil

nil 是一种值为 nil 的类型，表示无效值

Lua 变量无须初始化即可使用，默认值就是 nil

将 nil 赋给变量，相当于删除，Lua GC 会在合适的时候进行回收

# number

number 型用于表示数值

算术运算：

- `+` `-` `*` `%`
- `/` `^`（结果是浮点数）
- `//`（下取整，结果类型取决于操作数）
- string 类型的操作数会隐式转换为 number（浮点数）

比较运算/作为表的索引时，忽略整数和浮点数的差异（比如 1 和 1.0 这种）

# boolean

boolean 类型的值有两个，true 和 false

在布尔上下文中，被视为 false 的值有 false、nil，其他值将被视为 true

逻辑运算：`and` `or` `not`

三目运算符：`expr and expr1 or expr2`

# string

string 类型用于表示字符串

语法：

- 双引号：`"I am a str"`
- 单引号：`'I am a str'`
- 长字符串（忽略转义字符，保留换行和缩进）：`[[I am a doc str]]`

字符串操作：

- `..`（字符串拼接，number 类型的操作数会隐式转换为 string）
- `#`（返回字节数）

# function

Lua 函数是第一类值，函数定义其实是把一个 function 类型的值赋给一个变量

`:` 是一个语法糖，可以在方法调用中增加一个隐藏的 self 实参，或在方法定义中增加一个隐藏的 self 形参

## 闭包

函数内可以定义函数，内部函数可以访问外部函数的变量（也叫 upvalue 或者非局部变量），这里的内部函数就是闭包

闭包会保存 upvalue 的副本，每次调用外部函数都会生成一个新的闭包

闭包的常见用法：

- 实现迭代器

```lua
function iter(t)
    local idx = 0
    return function()
        idx = idx + 1
        return t[idx]
    end
end

for e in iter({1, 2, 3, 4, 5}) do
    print(e)
end
```

- 实现私有变量

```lua
function privateVar(val)
    return {
        set = function(x) val = x end,
        get = function() return val end
    }
end

var = privateVar(100)
print(var.get())
var.set(50)
print(var.get())
```

# table

Lua 表分为数组部分和哈希部分，数组索引从 1 开始

`.` 是一个语法糖，`a.x`，等价于 `a["x"]`

表的初始化：

- 数组式：`a = {1, 2}`
- 字典式（索引是字符串）：`a = {x = 1, y = 2}`
- 通用式：`a = {["x"] = 1, ["y"] = 2, [1] = 1, [2] = 2}`

表的遍历：

- `ipairs`：遍历数组部分，有序
- `pairs`：遍历所有元素，无序

数组操作：

- `#`：获取数组长度
- `table.insert`：任意位置插入元素，默认尾插
- `table.remove`：任意位置删除元素，默认尾删
- `table.sort`：对数组排序，默认升序
- `table.concat`：将数组元素拼接为字符串，默认无分隔符

判断表是不是数组：

```lua
function is_array(t)
    local len = #t
    local count = 0
    for k, _ in pairs(t) do
        count = count + 1
        if type(k) ~= "number" or k < 1 or k > len or t[k] == nil then
            return false
        end
    end
    return count == len
end
```

_G 是一个全局表，存储了所有全局变量的名字和值

## 元表

元表可以通过一些特定的字符串字段，来控制表的行为

- __index：控制表的查询，如果表中没有就会通过元表的__index来查询
- __newindex：控制表的插入
- __tostring：控制表到字符串的转换
- __call：控制表的调用
- __add __sub __mul __div __mod __pow
- __eq __lt __le

元表本身也是表

## 面向对象

Lua 元表可以用来模拟面向对象

```lua
--基类
local Class = {}
Class.__index = Class
function Class:new()
    local obj = {}
    setmetatable(obj, self)
    return obj
end
function Class:func()
    print("Hello! I am an object of Class")
end

--基类对象
local obj = Class:new()
obj:func()

--子类
local subClass = Class:new()
subClass.__index = subClass
function subClass:new()
    local obj = Class:new()
    setmetatable(obj, self)
    return obj
end
function subClass:func() --重写
    -- 调基类方法:
    -- local super = getmetatable(self)
    -- super:func()
    print("Hello! I am an object of subClass")
end

--子类对象
local obj = subClass:new()
obj:func()
```

## table深拷贝

```lua
local function deepcopy(t, vis--[[prevent loop reference]])
    if(type(t) ~= "table") then
        return t
    end
    
    vis = vis or {}
    if(vis[t]) then
        return vis[t]
    end
    
    local res = {}
    local mt = getmetatable(t)
    if mt then
        setmetatable(res, mt)
    end
    for k, v in pairs(t) do
        res[deepcopy(k, vis)] = deepcopy(v, vis)
    end
    return res
end
```

# thread

thread 是 Lua 协程的类型

公共方法：

- `coroutine.create(function)`：接受一个函数，返回一个协程对象
- `coroutine.resume(co, ...)`：恢复协程
- `coroutine.yield(...)`：挂起协程
- `coroutine.status(co)`：获取协程的状态

# userdata

# 热更新

热更新就是动态加载新代码而无需重启程序，一般用于不停机更新

Lua 可以很方便的实现热更新，因为 Lua 函数是第一类值，加载新代码不过是让原来的变量指向新的 function

## UnLua

