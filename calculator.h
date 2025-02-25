#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <windows.h>
#include <float.h>

// 错误代码枚举
typedef enum {
    ERR_SUCCESS = 0,          // 成功
    ERR_SYNTAX = 1,          // 语法错误
    ERR_DIV_BY_ZERO = 2,     // 除以零
    ERR_INVALID_NUMBER = 3,   // 无效数字
    ERR_OVERFLOW = 4,         // 数值溢出
    ERR_UNDEFINED = 5,        // 未定义结果
    ERR_INVALID_FUNCTION = 6, // 无效函数
    ERR_INVALID_ARGUMENT = 7, // 无效参数
    ERR_STACK_OVERFLOW = 8,   // 栈溢出
    ERR_MISSING_PARENTHESIS = 9, // 括号不匹配
    ERR_EMPTY_EXPRESSION = 10 // 空表达式
} ErrorCode;

// 错误处理结构
typedef struct {
    int code;  // 保持为int以兼容现有代码
    const char* message;
} CalcError;

// 错误处理宏 - 保持原始格式
#define CALC_SUCCESS ((CalcError){0, NULL})
#define CALC_ERROR(msg) ((CalcError){1, msg})

// 新增：带错误代码的错误处理宏（可选使用）
#define CALC_ERROR_CODE(code, msg) ((CalcError){code, msg})

// 常量定义
#define MAX_EXPR 100
#define PRECISION 10
#define PI 3.14159265358979323846
#define INFINITY_THRESHOLD 1e15
#define EPSILON 1e-10

// 运算符优先级
#define PRIORITY_ADD 1    // + -
#define PRIORITY_MUL 2    // * /
#define PRIORITY_POW 3    // ^
#define PRIORITY_PAR 0    // (

// 角度模式
typedef enum {
    MODE_DEG,  // 角度模式
    MODE_RAD   // 弧度模式
} AngleMode;

// 函数类型枚举
typedef enum {
    FUNC_NONE,
    FUNC_SIN,
    FUNC_COS,
    FUNC_TAN,
    FUNC_SQRT,
    FUNC_RAD,   // 转换为弧度
    FUNC_DEG    // 转换为角度
} FuncType;

// 函数声明
CalcError evaluateExpression(const char* expr, AngleMode mode, double* result);
CalcError getNumberWithError(const char** expr, double* result);
CalcError calculateFunctionWithError(FuncType func, double value, AngleMode mode, double* result);
double getNumber(const char** expr);
int getPriority(char op);
FuncType getFunction(const char** expr);
double calculateFunction(FuncType func, double value, AngleMode mode);
double degreeToRadian(double degree);
double radianToDegree(double radian);
int isInfinite(double value);
int isUndefined(double value);

// 新增：获取错误描述的函数声明
const char* getErrorDescription(int errorCode);

#endif // CALCULATOR_H 