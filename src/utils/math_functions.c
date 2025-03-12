#include "calculator.h"

// 计算三角函数和其他数学函数
CalcError calculateFunctionWithError(FuncType func, double value, AngleMode mode, double* result) {
    // 检查输入是否有效
    if (isUndefined(value)) {
        *result = value;
        return CALC_SUCCESS;
    }

    // 如果是pi的倍数，先标准化到[-2π, 2π]范围
    if (mode == MODE_RAD) {
        value = fmod(value, 2 * PI);
        if (value > PI) value -= 2 * PI;
        else if (value < -PI) value += 2 * PI;
    }

    double angle;
    long intValue = 0; // 初始化intValue
    switch (func) {
        case FUNC_SIN:
            angle = (mode == MODE_DEG) ? degreeToRadian(value) : value;
            *result = sin(angle);
            // 处理精度问题，使用新的精度控制函数
            if (fabs(*result) < ABSOLUTE_ZERO_THRESHOLD) *result = 0;
            // 特殊角度的处理
            if (mode == MODE_DEG) {
                // 90度的整数倍特殊处理
                double mod90 = fmod(fabs(value), 180);
                if (fabs(mod90 - 90) < EPSILON) {
                    *result = (value > 0) ? 1 : -1;
                    if (fmod(fabs(value), 360) > 180) *result = -*result;
                } else if (fabs(mod90) < EPSILON || fabs(mod90 - 180) < EPSILON) {
                    *result = 0;
                }
            } else {
                // 特殊弧度值处理
                double modPi = fmod(fabs(value), 2 * PI);
                if (fabs(modPi - PI/2) < EPSILON) {
                    *result = (value > 0) ? 1 : -1;
                    if (fmod(fabs(value), 2 * PI) > PI) *result = -*result;
                } else if (fabs(modPi) < EPSILON || fabs(modPi - PI) < EPSILON) {
                    *result = 0;
                }
            }
            // 再次检查是否接近整数
            if (isCloseToInteger(*result, &intValue)) {
                *result = intValue;
            }
            break;
            
        case FUNC_COS:
            angle = (mode == MODE_DEG) ? degreeToRadian(value) : value;
            *result = cos(angle);
            // 处理精度问题，使用新的精度控制函数
            if (fabs(*result) < ABSOLUTE_ZERO_THRESHOLD) *result = 0;
            // 特殊角度的处理
            if (mode == MODE_DEG) {
                // 90度的整数倍特殊处理
                double mod90 = fmod(fabs(value), 180);
                if (fabs(mod90) < EPSILON || fabs(mod90 - 180) < EPSILON) {
                    *result = (fabs(mod90) < EPSILON) ? 1 : -1;
                    // 根据360度周期调整符号
                    if (fmod(fabs(value), 360) > 180) *result = -*result;
                } else if (fabs(mod90 - 90) < EPSILON) {
                    *result = 0;
                } else if (fabs(mod90 - 90) < 0.001) {
                    // 接近90度的特殊处理
                    *result = 0;
                }
            } else {
                // 特殊弧度值处理
                double modPi = fmod(fabs(value), 2 * PI);
                if (fabs(modPi) < EPSILON || fabs(modPi - 2*PI) < EPSILON) {
                    *result = 1;
                } else if (fabs(modPi - PI) < EPSILON) {
                    *result = -1;
                } else if (fabs(modPi - PI/2) < EPSILON || fabs(modPi - 3*PI/2) < EPSILON) {
                    *result = 0;
                } else if (fabs(modPi - PI/2) < 0.0001 || fabs(modPi - 3*PI/2) < 0.0001) {
                    // 接近π/2的特殊处理
                    *result = 0;
                }
            }
            // 再次检查是否接近整数
            if (isCloseToInteger(*result, &intValue)) {
                *result = intValue;
            }
            break;
            
        case FUNC_TAN:
            angle = (mode == MODE_DEG) ? degreeToRadian(value) : value;
            // 检查是否是90度的倍数
            if (mode == MODE_DEG) {
                double mod90 = fmod(fabs(value), 180);
                if (fabs(mod90 - 90) < EPSILON) {
                    return CALC_ERROR_CODE(ERR_UNDEFINED, "tan函数在该点处无定义");
                }
            } else {
                double modPiHalf = fmod(fabs(value), PI);
                if (fabs(modPiHalf - PI/2) < EPSILON) {
                    return CALC_ERROR_CODE(ERR_UNDEFINED, "tan函数在该点处无定义");
                }
            }
            *result = tan(angle);
            // 处理精度问题，使用新的精度控制函数
            if (fabs(*result) < ABSOLUTE_ZERO_THRESHOLD) *result = 0;
            if (isCloseToInteger(*result, &intValue)) {
                *result = intValue; // 如果接近整数，直接使用整数值
            }
            break;
            
        case FUNC_ASIN: // 反正弦函数
            if (value < -1 || value > 1) {
                return CALC_ERROR_CODE(ERR_INVALID_ARGUMENT, "asin的参数必须在[-1,1]范围内");
            }
            *result = asin(value);
            if (mode == MODE_DEG) {
                *result = radianToDegree(*result);
            }
            // 处理精度问题
            if (fabs(*result) < ABSOLUTE_ZERO_THRESHOLD) *result = 0;
            if (isCloseToInteger(*result, &intValue)) {
                *result = intValue;
            }
            break;
            
        case FUNC_ACOS: // 反余弦函数
            if (value < -1 || value > 1) {
                return CALC_ERROR_CODE(ERR_INVALID_ARGUMENT, "acos的参数必须在[-1,1]范围内");
            }
            *result = acos(value);
            if (mode == MODE_DEG) {
                *result = radianToDegree(*result);
            }
            // 处理精度问题
            if (fabs(*result) < ABSOLUTE_ZERO_THRESHOLD) *result = 0;
            if (isCloseToInteger(*result, &intValue)) {
                *result = intValue;
            }
            break;
            
        case FUNC_ATAN: // 反正切函数
            *result = atan(value);
            if (mode == MODE_DEG) {
                *result = radianToDegree(*result);
            }
            // 处理精度问题
            if (fabs(*result) < ABSOLUTE_ZERO_THRESHOLD) *result = 0;
            if (isCloseToInteger(*result, &intValue)) {
                *result = intValue;
            }
            break;
            
        case FUNC_SQRT:
            if (value < 0) {
                return CALC_ERROR_CODE(ERR_INVALID_ARGUMENT, "负数不能开平方根！");
            }
            *result = sqrt(value);
            // 检查结果是否接近整数
            if (isCloseToInteger(*result, &intValue)) {
                *result = intValue; // 如果接近整数，直接使用整数值
            }
            break;
            
        case FUNC_LOG: // 常用对数（以10为底）
            if (value <= 0) {
                return CALC_ERROR_CODE(ERR_INVALID_ARGUMENT, "log函数的参数必须大于0");
            }
            *result = log10(value);
            // 处理精度问题
            if (fabs(*result) < ABSOLUTE_ZERO_THRESHOLD) *result = 0;
            if (isCloseToInteger(*result, &intValue)) {
                *result = intValue;
            }
            break;
            
        case FUNC_LN: // 自然对数
            if (value <= 0) {
                return CALC_ERROR_CODE(ERR_INVALID_ARGUMENT, "ln函数的参数必须大于0");
            }
            *result = log(value);
            // 处理精度问题
            if (fabs(*result) < ABSOLUTE_ZERO_THRESHOLD) *result = 0;
            if (isCloseToInteger(*result, &intValue)) {
                *result = intValue;
            }
            break;
            
        case FUNC_ABS: // 绝对值函数
            *result = fabs(value);
            if (isCloseToInteger(*result, &intValue)) {
                *result = intValue;
            }
            break;
            
        case FUNC_RAD:
            *result = degreeToRadian(value);
            break;
            
        case FUNC_DEG:
            *result = radianToDegree(value);
            break;
            
        default:
            *result = value;
    }
    return CALC_SUCCESS;
}

// 为了保持兼容性，保留原有的calculateFunction函数
double calculateFunction(FuncType func, double value, AngleMode mode) {
    double result;
    CalcError err = calculateFunctionWithError(func, value, mode, &result);
    if (err.code != 0) {
        return NAN;
    }
    return result;
}

// 角度转弧度
double degreeToRadian(double degree) {
    return degree * PI / 180.0;
}

// 弧度转角度
double radianToDegree(double radian) {
    return radian * 180.0 / PI;
} 