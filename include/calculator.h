#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <windows.h>
#include <float.h>

// 包含分解的头文件
#include "error_handling.h"
#include "function_types.h"
#include "number_utils.h"

// 常量定义
#define MAX_EXPR 100

// 主要接口函数声明 - 核心计算功能
CalcError evaluateExpression(const char* expr, AngleMode mode, double* result);

// 括号处理函数
CalcError checkBracketMatch(const char* expr);

// 运算符处理函数
CalcError processOperators(double* numbers, int* numTop, char* operators, int* opTop, char stopAt, int processEqual);
CalcError performOperation(char op, double a, double b, double* result);

// 安全检查函数
CalcError checkStackOverflow(int stackSize, const char* stackName);

#endif // CALCULATOR_H 