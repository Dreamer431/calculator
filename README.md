# Calculator

一个科学计算器程序，使用C语言开发，支持基础运算、三角函数、对数函数、常量计算等丰富功能。

[![Language](https://img.shields.io/badge/language-C-blue.svg)](https://en.wikipedia.org/wiki/C_(programming_language))
[![Platform](https://img.shields.io/badge/platform-Windows-lightgrey.svg)](https://www.microsoft.com/windows)

---

## 目录
- [功能特点](#功能特点)
- [项目结构](#项目结构)
- [编译要求](#编译要求)
- [编译方法](#编译方法)
- [使用方法](#使用方法)
- [表达式规则](#表达式规则)
- [注意事项](#注意事项)
- [错误代码说明](#错误代码说明)
- [最近更新](#最近更新)
- [待完善事项](#待完善事项)

## 功能特点
- 基础运算
  - 支持加减乘除四则运算
  - 支持幂运算（使用^符号）
  - 支持括号嵌套
  - 支持小数计算
  - 支持数字与括号之间的隐式乘法（如 2(3+4)）
  - 支持科学计数法（如 1.23e-4）

- 三角函数
  - sin(x)：正弦函数
  - cos(x)：余弦函数
  - tan(x)：正切函数
  - asin(x)：反正弦函数
  - acos(x)：反余弦函数
  - atan(x)：反正切函数
  - 支持角度模式和弧度模式

- 其他数学函数
  - sqrt(x)：平方根函数
  - log(x)：常用对数（以10为底）
  - ln(x)：自然对数
  - abs(x)：绝对值
  - rad(x)：角度转弧度
  - deg(x)：弧度转角度

- 常量支持
  - pi：圆周率（3.14159...）
  - e：自然对数的底（2.71828...）

- 错误处理
  - 提供详细的错误提示
  - 检查括号匹配
  - 检查数值范围
  - 验证表达式格式
  - 错误代码分类

- 用户体验
  - 命令历史记录功能（最近5条）
  - 详细的帮助信息
  - 针对不同错误类型的提示

## 项目结构
```text
calculator/
├── include/            # 头文件目录
│   ├── calculator.h    # 主头文件
│   ├── error_handling.h # 错误处理头文件
│   ├── function_types.h # 函数类型定义
│   └── number_utils.h  # 数值处理工具
│
├── src/                # 源代码目录
│   ├── core/           # 核心计算功能
│   │   ├── expression_evaluator.c # 表达式求值
│   │   ├── error_handling.c       # 错误处理
│   │   ├── operator_handling.c    # 运算符处理
│   │   └── main.c                 # 主程序入口
│   │
│   └── utils/          # 工具函数
│       ├── math_functions.c       # 数学函数实现
│       ├── number_parser.c        # 数字解析
│       ├── number_formatter.c     # 数字格式化
│       └── precision_handling.c   # 精度处理
│
├── test/               # 测试相关
│   ├── test_runner.c   # 测试运行器
│   ├── test_framework.c # 测试框架
│   ├── test_framework.h # 测试框架头文件
│   └── test_cases.c    # 测试用例
│
├── build/              # 编译产物目录
├── Makefile            # 项目构建配置
├── calculator.exe      # 可执行文件
├── test_runner.exe     # 测试可执行文件
├── README.md           # 项目说明文档
└── .gitignore          # Git忽略配置
```

## 编译要求
- 操作系统：Windows 10及以上
- 编译器：TDM-GCC-64
- 构建工具：make

## 编译方法
1. 确保已安装TDM-GCC-64编译器
2. 在项目根目录打开命令行
3. 运行以下命令进行编译：
   ```bash
   make
   ```
4. 运行测试（可选）：
   ```bash
   make test
   ```
5. 清理编译产物（可选）：
   ```bash
   make clean
   ```

详细的构建配置请参考 [Makefile](Makefile)。

## 使用方法
1. 在命令行中运行程序：
   ```bash
   ./calculator
   ```
2. 程序启动后：
   - 输入数学表达式进行计算
   - 输入 `mode` 切换角度/弧度模式
   - 输入 `history` 查看历史记录
   - 输入 `help` 查看帮助信息
   - 输入 `q` 退出程序

更多详细的使用规则请参考下面的[表达式规则](#表达式规则)部分。

## 表达式规则
- 支持的运算符：+、-、*、/、^（幂运算）
- 支持的函数：
  - 三角函数：sin、cos、tan、asin、acos、atan
  - 对数函数：log（常用对数）、ln（自然对数）
  - 其他函数：sqrt、abs、rad、deg
- 支持的常量：pi、e
- 数字格式：
  - 整数：`123`
  - 小数：`123.456`
  - 科学计数法：`1.23e-4`
- 括号使用：
  - 支持多层嵌套，如 `(1+(2*(3+4)))`
  - 支持隐式乘法，如 `2(3+4)`
- 负数处理：
  - 直接使用负号，如 `-1`
  - 表达式中的负数需要括号，如 `2*(-3)`

## 注意事项
- 输入表达式时，请确保表达式符合四则运算的规则
- 三角函数默认使用角度模式，可通过 `mode` 命令切换
- 表达式长度不要超过100个字符
- 括号嵌套不要超过10层
- 数值范围限制：
  - 最大值：1e308
  - 最小值：1e-308
  - 整数支持：使用int64_t类型，支持约±9.2×10^18范围的整数

## 错误代码说明
系统定义了以下错误代码类型：

| 错误代码 | 错误类型 | 说明 |
|---------|---------|------|
| 0 | 操作成功 | 计算正常完成 |
| 1 | 语法错误 | 表达式格式不正确 |
| 2 | 除以零 | 除数为零的情况 |
| 3 | 无效数字 | 数字格式错误 |
| 4 | 数值溢出 | 计算结果超出范围 |
| 5 | 未定义结果 | 数学上未定义的运算 |
| 6 | 无效函数 | 不支持的函数名 |
| 7 | 无效参数 | 函数参数不合法 |
| 8 | 栈溢出 | 表达式过于复杂 |
| 9 | 括号不匹配 | 括号配对错误 |
| 10 | 空表达式 | 输入为空 |

## 最近更新
- 重构了项目目录结构，使代码组织更加清晰
- 添加了更多数学函数支持：asin、acos、atan、log、ln、abs
- 添加了新常量：e（自然对数的底）
- 更新大整数处理：从 long 改为 int64_t，提供更大范围的整数支持
- 改进整数格式化：大于 10^15 的整数会使用科学计数法显示
- 修复了大数显示和精度问题
- 优化了代码质量和可维护性
- 改进了边界测试和错误处理

## 待完善事项

根据测试结果，以下几个方面还需要改进：

### 1. 边界值处理
- 接近零的值的处理不够精确，例如 `cos(89.999999)`
  - 期望结果：0
  - 实际结果：1.745329243e-08
   
### 2. 用户界面
- 优化命令行界面，提供更友好的用户体验
- 考虑添加图形界面