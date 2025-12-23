#include "calculator.h"

/**
 * 浮点数比较函数，处理精度问题
 * 根据数值大小自动选择合适的比较方法
 */
int isDoubleEqual(double a, double b) {
    // 处理特殊情况
    // NaN 与任何值都不相等，包括自己（IEEE 754标准）
    if (isnan(a) || isnan(b)) return 0;
    if (isinf(a) && isinf(b)) return (a > 0) == (b > 0);
    
    // 检查绝对零
    if (fabs(a) < ABSOLUTE_ZERO_THRESHOLD && fabs(b) < ABSOLUTE_ZERO_THRESHOLD) {
        return 1;
    }
    
    // 对于非常小的数，使用绝对误差
    if (fabs(a) < EPSILON || fabs(b) < EPSILON) {
        return fabs(a - b) < EPSILON;
    }
    
    // 对于其他数，同时检查相对误差和绝对误差
    return fabs(a - b) < EPSILON || fabs((a - b) / ((fabs(a) > fabs(b)) ? a : b)) < RELATIVE_EPSILON;
}

/**
 * 检查数值是否接近整数
 * 如果接近，返回转换后的整数值
 */
int isCloseToInteger(double value, int64_t* intValue) {
    // 四舍五入到最接近的整数
    double rounded = round(value);
    
    // 检查是否在 int64_t 范围内，防止转换溢出
    if (rounded > (double)INT64_MAX || rounded < (double)INT64_MIN) {
        return 0;
    }
    
    if (isDoubleEqual(value, rounded)) {
        *intValue = (int64_t)rounded;
        return 1;
    }
    return 0;
}

/**
 * 判断是否无穷大
 * 包括处理浮点数的infinity和超大数值
 */
int isInfinite(double value) {
    return !isfinite(value) || fabs(value) > INFINITY_THRESHOLD;
}

/**
 * 判断是否未定义
 * 包括NaN和无穷大与负无穷大比较的情况
 */
int isUndefined(double value) {
    return isnan(value);
} 