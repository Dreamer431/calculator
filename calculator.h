#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <windows.h>

// 常量定义
#define MAX_EXPR 100
#define PRECISION 10

// 函数声明
double evaluateExpression(const char* expr);
double getNumber(const char** expr);
int getPriority(char op);

#endif // CALCULATOR_H 