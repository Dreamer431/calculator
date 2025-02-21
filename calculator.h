#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <windows.h>
#include <float.h>

// 常量定义
#define MAX_EXPR 100
#define PRECISION 10
#define PI 3.14159265358979323846
#define INFINITY_THRESHOLD 1e15
#define EPSILON 1e-10

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
double evaluateExpression(const char* expr, AngleMode mode);
double getNumber(const char** expr);
int getPriority(char op);
FuncType getFunction(const char** expr);
double calculateFunction(FuncType func, double value, AngleMode mode);
double degreeToRadian(double degree);
double radianToDegree(double radian);
int isInfinite(double value);
int isUndefined(double value);

#endif // CALCULATOR_H 