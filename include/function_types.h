#ifndef FUNCTION_TYPES_H
#define FUNCTION_TYPES_H

#include <math.h>
#include "error_handling.h"

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
    FUNC_ASIN,   // 反正弦
    FUNC_ACOS,   // 反余弦
    FUNC_ATAN,   // 反正切
    FUNC_SQRT,
    FUNC_LOG,    // 常用对数(以10为底)
    FUNC_LN,     // 自然对数
    FUNC_ABS,    // 绝对值
    FUNC_RAD,    // 转换为弧度
    FUNC_DEG     // 转换为角度
} FuncType;

// 常量定义
#define PI 3.14159265358979323846
#define E 2.71828182845904523536  // 自然对数的底

// 运算符优先级
#define PRIORITY_ADD 1    // + -
#define PRIORITY_MUL 2    // * /
#define PRIORITY_POW 3    // ^
#define PRIORITY_PAR 0    // (

// 函数声明
FuncType getFunction(const char** expr);
int getPriority(char op);
double calculateFunction(FuncType func, double value, AngleMode mode);
CalcError calculateFunctionWithError(FuncType func, double value, AngleMode mode, double* result);
double degreeToRadian(double degree);
double radianToDegree(double radian);

#endif // FUNCTION_TYPES_H 