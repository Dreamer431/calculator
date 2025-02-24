# Calculator

一个支持科学计算的计算器程序，提供基础运算、三角函数、常量计算等功能。

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
  - 支持角度模式和弧度模式

- 其他函数
  - sqrt(x)：平方根函数

- 常量支持
  - pi：圆周率（3.14159...）

- 错误处理
  - 提供详细的错误提示
  - 检查括号匹配
  - 检查数值范围
  - 验证表达式格式

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

## 使用方法
1. 在命令行中运行程序：
   ```bash
   ./calculator
   ```
2. 程序启动后：
   - 输入数学表达式进行计算
   - 输入 `mode` 切换角度/弧度模式
   - 输入 `q` 退出程序

## 表达式规则
- 支持的运算符：+、-、*、/、^（幂运算）
- 支持的函数：sin、cos、tan、sqrt
- 支持的常量：pi
- 数字格式：
  - 整数：123
  - 小数：123.456
  - 科学计数法：1.23e-4
- 括号使用：
  - 支持多层嵌套，如 (1+(2*(3+4)))
  - 支持隐式乘法，如 2(3+4)
- 负数处理：
  - 直接使用负号，如 -1
  - 表达式中的负数需要括号，如 2*(-3)

## 注意事项
- 输入表达式时，请确保表达式符合四则运算的规则
- 三角函数默认使用角度模式，可通过 `mode` 命令切换
- 表达式长度不要超过100个字符
- 括号嵌套不要超过10层
- 数值范围限制：
  - 最大值：1e308
  - 最小值：1e-308

## 待完善事项

根据测试结果，以下几个方面还需要改进：

1. 科学计数法错误处理
   - 多个指数符号的处理有误，例如 `1ee2` 应返回"数字格式不正确，多个指数符号！"，但当前返回"指数部分必须是整数！"

2. 边界值处理
   - 大数显示格式不统一，例如 `9999999999` 的显示格式
     - 期望结果：1e+10
     - 实际结果：9999999999
   - 接近零的值的处理不够精确，例如 `cos(89.999999)`
     - 期望结果：0
     - 实际结果：1.745329243e-08
