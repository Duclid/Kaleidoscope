# 使用LLVM实现一个简单的编译器

## 一、目标
基于LLVM官网教程实现一个基于 Kaleidoscope 语言的编译器

Kaleidoscope 语言
- 浮点数，仅支持64位浮点数
- 变量定义
- 函数声明
- 函数调用
- 条件分支

Kaleidoscope 支持的四种运算符
> '<' < '+' = '-' < '*'

一个 Kaleidoscope 程序
```
# 斐波那契数列函数定义
def fib(x)
    if x < 3 then
        1
    else
        fib(x - 1) + fib(x - 2)

fib(40)

# 函数声明
extern sin(arg)
extern cos(arg)
extern atan2(arg1 arg2)

# 声明后的函数可调用
atan2(sin(.4), cos(42))
```

## 二、实现
###  1. Lexer

实现一个词法分析工具类，读入字符输出记号流(token)。

### 2. Parser

实现一个语法分析器，根据 Kaleidoscope 文法解析词法分析器输出的 token，根据文法规则生成抽象语法树。

#### AST 节点类型

- 数字表达式
  - val
- 变量表达式
  - name
- 二元操作表达式
  - op
  - lhs
  - rhs
- 函数
  - 函数声明
    - name 函数名称
    - args 函数参数
  - 函数定义
    - 函数声明
    - 函数体
  - 函数调用
    - callee 调用函数
    - args  函数参数
- 条件分支

#### AST 节点解析器

## 三、测试

## 四、改进
