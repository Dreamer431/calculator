#include "calculator.h"
#include <stdint.h>  // 添加对int64_t的支持

/**
 * 浮点数比较函数，处理精度问题
 * 根据数值大小自动选择合适的比较方法
 */
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

/**
 * 检查角度是否接近特定的标准角
 * @param angle 角度值
 * @param standardAngle 标准角度值
 * @param epsilon 允许的误差
 * @param isDegreesMode 是否是角度模式
 * @return 如果接近标准角，返回1，否则返回0
 */
int isNearStandardAngle(double angle, double standardAngle, double epsilon, int isDegreesMode) {
    double modValue;
    double fullCircle;
    
    if (isDegreesMode) {
        fullCircle = 360.0;
    } else {
        fullCircle = 2 * PI;
    }
    
    // 将角度转换到[0, fullCircle)范围
    angle = fmod(angle, fullCircle);
    if (angle < 0) angle += fullCircle;
    
    // 将标准角也转换到[0, fullCircle)范围
    standardAngle = fmod(standardAngle, fullCircle);
    if (standardAngle < 0) standardAngle += fullCircle;
    
    // 计算最短角度距离
    modValue = fabs(angle - standardAngle);
    if (modValue > fullCircle / 2) {
        modValue = fullCircle - modValue;
    }
    
    return modValue < epsilon;
}

/**
 * 特殊的三角函数值处理
 * 修正非常接近0、1、-1的三角函数计算结果
 * @param value 计算结果
 * @param isStandardValue 0=非标准值，1=应当为0，2=应当为1，3=应当为-1
 * @return 修正后的值
 */
double correctTrigValue(double value, int isStandardValue) {
    // 非标准值，直接返回
    if (isStandardValue == 0) {
        return value;
    }
    
    // 如果值已经非常接近标准值，则直接返回标准值
    if (isStandardValue == 1 && fabs(value) < EPSILON) {
        return 0.0;
    } else if (isStandardValue == 2 && fabs(value - 1.0) < EPSILON) {
        return 1.0;
    } else if (isStandardValue == 3 && fabs(value + 1.0) < EPSILON) {
        return -1.0;
    }
    
    return value;
} 