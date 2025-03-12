#include "calculator.h"

// 浮点数比较函数，处理精度问题
int isDoubleEqual(double a, double b) {
    // 处理特殊情况
    if (isnan(a) && isnan(b)) return 1;
    if (isinf(a) && isinf(b)) return (a > 0) == (b > 0);
    
    // 检查绝对零
    if (fabs(a) < ABSOLUTE_ZERO_THRESHOLD && fabs(b) < ABSOLUTE_ZERO_THRESHOLD) {
        return 1;
    }
    
    // 对于非常小的数，使用绝对误差
    if (fabs(a) < EPSILON || fabs(b) < EPSILON) {
        return fabs(a - b) < EPSILON;
    }
    
    // 对于其他数，使用相对误差
    double relativeError = fabs((a - b) / b);
    double absoluteError = fabs(a - b);
    
    // 同时满足相对误差和绝对误差条件之一
    return relativeError < RELATIVE_EPSILON || absoluteError < EPSILON;
}

// 检查数值是否接近整数
int isCloseToInteger(double value, long* intValue) {
    // 四舍五入到最接近的整数
    double rounded = round(value);
    if (isDoubleEqual(value, rounded)) {
        *intValue = (long)rounded;
        return 1;
    }
    return 0;
}

// 判断是否无穷大
int isInfinite(double value) {
    return !isfinite(value) || fabs(value) > INFINITY_THRESHOLD;
}

// 判断是否未定义
int isUndefined(double value) {
    return isnan(value) || (value == INFINITY && value == -INFINITY);
} 