#include "calculator.h"
#include <math.h>
#include <stdint.h>  // 添加对int64_t的支持

// 角度转弧度
double degreeToRadian(double degree) {
    return degree * PI / 180.0;
}

// 弧度转角度
double radianToDegree(double radian) {
    return radian * 180.0 / PI;
}

/**
 * 检查三角函数的特殊角度并返回标准值
 * @param angle 角度值
 * @param mode 角度模式
 * @param funcType 函数类型（sin, cos, tan）
 * @return 0: 非特殊值; 1: 应为0; 2: 应为1; 3: 应为-1; 4: 无定义
 */
static int checkTrigSpecialAngle(double angle, AngleMode mode, FuncType funcType) {
    double epsilon = (mode == MODE_DEG) ? ANGLE_EPSILON_DEG : ANGLE_EPSILON_RAD;
    double fullCircle = (mode == MODE_DEG) ? 360.0 : 2 * PI;
    double right_angle = (mode == MODE_DEG) ? 90.0 : PI/2;
    double half_circle = (mode == MODE_DEG) ? 180.0 : PI;
    
    // 规范化角度到[0, fullCircle)
    angle = fmod(angle, fullCircle);
    if (angle < 0) angle += fullCircle;
    
    // 检查是否接近特殊角
    switch (funcType) {
        case FUNC_SIN:
            // sin在0°和180°处为0
            if (isNearStandardAngle(angle, 0, epsilon, mode == MODE_DEG) || 
                isNearStandardAngle(angle, half_circle, epsilon, mode == MODE_DEG)) {
                return 1; // 值应为0
            }
            // sin在90°处为1，270°处为-1
            else if (isNearStandardAngle(angle, right_angle, epsilon, mode == MODE_DEG)) {
                return 2; // 值应为1
            }
            else if (isNearStandardAngle(angle, right_angle + half_circle, epsilon, mode == MODE_DEG)) {
                return 3; // 值应为-1
            }
            break;
            
        case FUNC_COS:
            // cos在90°和270°处为0
            if (isNearStandardAngle(angle, right_angle, epsilon, mode == MODE_DEG) || 
                isNearStandardAngle(angle, right_angle + half_circle, epsilon, mode == MODE_DEG)) {
                return 1; // 值应为0
            }
            // cos在0°处为1，180°处为-1
            else if (isNearStandardAngle(angle, 0, epsilon, mode == MODE_DEG)) {
                return 2; // 值应为1
            }
            else if (isNearStandardAngle(angle, half_circle, epsilon, mode == MODE_DEG)) {
                return 3; // 值应为-1
            }
            break;
            
        case FUNC_TAN:
            // tan在0°和180°处为0
            if (isNearStandardAngle(angle, 0, epsilon, mode == MODE_DEG) || 
                isNearStandardAngle(angle, half_circle, epsilon, mode == MODE_DEG)) {
                return 1; // 值应为0
            }
            // tan在90°和270°处无定义
            else if (isNearStandardAngle(angle, right_angle, epsilon, mode == MODE_DEG) || 
                     isNearStandardAngle(angle, right_angle + half_circle, epsilon, mode == MODE_DEG)) {
                return 4; // 无定义
            }
            break;
            
        default:
            // 其他函数不需要特殊处理
            break;
    }
    
    return 0; // 非特殊角
}

/**
 * 计算数学函数，带错误处理
 */
CalcError calculateFunctionWithError(FuncType func, double value, AngleMode mode, double* result) {
    // 检查输入是否有效
    if (isUndefined(value)) {
        *result = value;
        return CALC_SUCCESS;
    }
    
    double angle;
    int specialValue;
    
    switch (func) {
        case FUNC_SIN:
            // 检查特殊角度
            specialValue = checkTrigSpecialAngle(value, mode, FUNC_SIN);
            if (specialValue == 1) {
                *result = 0.0;
                return CALC_SUCCESS;
            } else if (specialValue == 2) {
                *result = 1.0;
                return CALC_SUCCESS;
            } else if (specialValue == 3) {
                *result = -1.0;
                return CALC_SUCCESS;
            }
            
            // 常规计算
            angle = (mode == MODE_DEG) ? degreeToRadian(value) : value;
            *result = sin(angle);
            
            // 处理接近零的值
            if (fabs(*result) < EPSILON) {
                *result = 0.0;
            }
            break;
            
        case FUNC_COS:
            // 检查特殊角度
            specialValue = checkTrigSpecialAngle(value, mode, FUNC_COS);
            if (specialValue == 1) {
                *result = 0.0;
                return CALC_SUCCESS;
            } else if (specialValue == 2) {
                *result = 1.0;
                return CALC_SUCCESS;
            } else if (specialValue == 3) {
                *result = -1.0;
                return CALC_SUCCESS;
            }
            
            // 常规计算
            angle = (mode == MODE_DEG) ? degreeToRadian(value) : value;
            *result = cos(angle);
            
            // 处理接近零的值
            if (fabs(*result) < EPSILON) {
                *result = 0.0;
            }
            break;
            
        case FUNC_TAN:
            // 检查特殊角度
            specialValue = checkTrigSpecialAngle(value, mode, FUNC_TAN);
            if (specialValue == 1) {
                *result = 0.0;
                return CALC_SUCCESS;
            } else if (specialValue == 4) {
                return CALC_ERROR_CODE(ERR_UNDEFINED, "tan函数在该点处无定义");
            }
            
            // 常规计算
            angle = (mode == MODE_DEG) ? degreeToRadian(value) : value;
            *result = tan(angle);
            break;
            
        case FUNC_ASIN:
            // 检查参数范围
            if (fabs(value) > 1.0) {
                return CALC_ERROR_CODE(ERR_INVALID_ARGUMENT, "asin的参数必须在[-1,1]范围内");
            }
            *result = asin(value);
            if (mode == MODE_DEG) {
                *result = radianToDegree(*result);
            }
            break;
            
        case FUNC_ACOS:
            // 检查参数范围
            if (fabs(value) > 1.0) {
                return CALC_ERROR_CODE(ERR_INVALID_ARGUMENT, "acos的参数必须在[-1,1]范围内");
            }
            *result = acos(value);
            if (mode == MODE_DEG) {
                *result = radianToDegree(*result);
            }
            break;
            
        case FUNC_ATAN:
            *result = atan(value);
            if (mode == MODE_DEG) {
                *result = radianToDegree(*result);
            }
            break;
            
        case FUNC_SQRT:
            // 检查参数范围
            if (value < 0) {
                return CALC_ERROR_CODE(ERR_INVALID_ARGUMENT, "负数不能开平方根！");
            }
            *result = sqrt(value);
            break;
            
        case FUNC_LOG:
            // 检查参数范围
            if (value <= 0) {
                return CALC_ERROR_CODE(ERR_INVALID_ARGUMENT, "log函数的参数必须大于0");
            }
            *result = log10(value);
            break;
            
        case FUNC_LN:
            // 检查参数范围
            if (value <= 0) {
                return CALC_ERROR_CODE(ERR_INVALID_ARGUMENT, "ln函数的参数必须大于0");
            }
            *result = log(value);
            break;
            
        case FUNC_ABS:
            *result = fabs(value);
            break;
            
        case FUNC_RAD:
            *result = degreeToRadian(value);
            break;
            
        case FUNC_DEG:
            *result = radianToDegree(value);
            break;
            
        default:
            return CALC_ERROR_CODE(ERR_INVALID_FUNCTION, "无效的函数");
    }
    
    // 检查结果是否接近整数
    int64_t intValue;
    if (isCloseToInteger(*result, &intValue)) {
        *result = intValue;
    }
    
    return CALC_SUCCESS;
}

/**
 * 计算函数值，简化接口版本
 */
double calculateFunction(FuncType func, double value, AngleMode mode) {
    double result;
    CalcError err = calculateFunctionWithError(func, value, mode, &result);
    if (err.code != 0) {
        return NAN; // 出错时返回NaN
    }
    return result;
} 