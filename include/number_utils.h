#ifndef NUMBER_UTILS_H
#define NUMBER_UTILS_H

#include <float.h>
#include "error_handling.h"

// 数值精度控制常量
#define ABSOLUTE_ZERO_THRESHOLD 1e-15  // 绝对零阈值，小于此值视为0
#define RELATIVE_EPSILON 1e-12        // 相对误差阈值
#define DISPLAY_FORMAT_THRESHOLD 1e7  // 超过此值使用科学计数法
#define DISPLAY_FORMAT_MIN 1e-4       // 小于此值使用科学计数法
#define EPSILON 1e-10                 // 一般精度阈值
#define INFINITY_THRESHOLD 1e15       // 无穷大阈值
#define PRECISION 10                  // 小数点后位数

// 数值处理函数声明
CalcError getNumberWithError(const char** expr, double* result);
double getNumber(const char** expr);
int isInfinite(double value);
int isUndefined(double value);
int isDoubleEqual(double a, double b);
int isCloseToInteger(double value, long* intValue);
char* formatNumber(double value, char* buffer, size_t bufferSize);

#endif // NUMBER_UTILS_H 