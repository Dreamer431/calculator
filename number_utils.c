#include "calculator.h"

// 修改getNumber函数签名以支持错误处理
CalcError getNumberWithError(const char** expr, double* result) {
    double number = 0;
    double decimal = 0.1;
    int hasDecimal = 0;
    int decimalCount = 0;
    int digitCount = 0;
    int hasExponent = 0;
    int exponent = 0;
    int exponentSign = 1;
    
    // 跳过空格
    while (**expr == ' ') (*expr)++;
    
    // 确保至少有一个数字
    if (!isdigit(**expr) && **expr != '.') {
        return CALC_ERROR("无效的数字格式");
    }
    
    // 处理整数和小数部分
    while (isdigit(**expr) || **expr == '.' || tolower(**expr) == 'e') {
        if (**expr == '.') {
            if (hasDecimal || hasExponent) {
                return CALC_ERROR("数字格式不正确，多个小数点！");
            }
            hasDecimal = 1;
            (*expr)++;
        } else if (tolower(**expr) == 'e') {
            if (hasExponent) {
                return CALC_ERROR("数字格式不正确，多个指数符号！");
            }
            hasExponent = 1;
            (*expr)++;
            
            // 处理指数的符号
            if (**expr == '+' || **expr == '-') {
                exponentSign = (**expr == '+') ? 1 : -1;
                (*expr)++;
            }
            
            // 读取指数值
            if (!isdigit(**expr)) {
                return CALC_ERROR("指数部分必须是整数！");
            }
            while (isdigit(**expr)) {
                exponent = exponent * 10 + (**expr - '0');
                if (exponent > 308) { // 防止指数过大
                    return CALC_ERROR("数字太大！");
                }
                (*expr)++;
            }
            
            // 检查是否还有指数符号
            if (tolower(**expr) == 'e') {
                return CALC_ERROR("数字格式不正确，多个指数符号！");
            }
            
            // 检查指数后是否有小数点
            if (**expr == '.') {
                return CALC_ERROR("指数部分必须是整数！");
            }
            break;  // 指数部分结束后退出循环
        } else {
            if (hasDecimal) {
                if (decimalCount < PRECISION) {
                    number = number + (**expr - '0') * decimal;
                    decimal *= 0.1;
                    decimalCount++;
                }
            } else {
                digitCount++;
                if (digitCount > 15) {  // 防止整数部分过长
                    return CALC_ERROR("数字太大！");
                }
                // 检查整数部分溢出
                if (number > DBL_MAX / 10) {
                    return CALC_ERROR("数字太大！");
                }
                number = number * 10 + (**expr - '0');
            }
            (*expr)++;
        }
    }
    
    // 应用指数
    if (hasExponent) {
        double scale = pow(10.0, exponentSign * exponent);
        if (isinf(scale) || isinf(number * scale)) {
            return CALC_ERROR("数字太大！");
        }
        number *= scale;
    }
    
    *result = number;
    return CALC_SUCCESS;
}

// 为了保持兼容性，保留原有的getNumber函数
double getNumber(const char** expr) {
    double result;
    CalcError err = getNumberWithError(expr, &result);
    if (err.code != 0) {
        return NAN;
    }
    return result;
}

int getPriority(char op) {
    switch (op) {
        case '+':
        case '-':
            return PRIORITY_ADD;
        case '*':
        case '/':
            return PRIORITY_MUL;
        case '^':
            return PRIORITY_POW;
        case '(':
            return PRIORITY_PAR;
        default:
            return -1;
    }
}

FuncType getFunction(const char** expr) {
    char func[10] = {0};
    int i = 0;
    
    while (isalpha(**expr) && i < 9) {
        func[i++] = tolower(**expr);
        (*expr)++;
    }
    func[i] = '\0';
    
    if (strcmp(func, "sin") == 0) return FUNC_SIN;
    if (strcmp(func, "cos") == 0) return FUNC_COS;
    if (strcmp(func, "tan") == 0) return FUNC_TAN;
    if (strcmp(func, "sqrt") == 0) return FUNC_SQRT;
    if (strcmp(func, "rad") == 0) return FUNC_RAD;
    if (strcmp(func, "deg") == 0) return FUNC_DEG;
    
    return FUNC_NONE;
}

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
    switch (func) {
        case FUNC_SIN:
            angle = (mode == MODE_DEG) ? degreeToRadian(value) : value;
            *result = sin(angle);
            // 处理精度问题
            if (fabs(*result) < EPSILON) *result = 0;
            if (fabs(fabs(*result) - 1) < EPSILON) *result = (*result > 0) ? 1 : -1;
            break;
            
        case FUNC_COS:
            angle = (mode == MODE_DEG) ? degreeToRadian(value) : value;
            *result = cos(angle);
            // 处理精度问题
            if (fabs(*result) < EPSILON) *result = 0;
            if (fabs(fabs(*result) - 1) < EPSILON) *result = (*result > 0) ? 1 : -1;
            break;
            
        case FUNC_TAN:
            angle = (mode == MODE_DEG) ? degreeToRadian(value) : value;
            // 检查是否是90度的倍数
            if (mode == MODE_DEG) {
                double mod90 = fmod(fabs(value), 180);
                if (fabs(mod90 - 90) < EPSILON) {
                    return CALC_ERROR("tan函数在该点处无定义");
                }
            } else {
                double modPiHalf = fmod(fabs(value), PI);
                if (fabs(modPiHalf - PI/2) < EPSILON) {
                    return CALC_ERROR("tan函数在该点处无定义");
                }
            }
            *result = tan(angle);
            // 处理精度问题
            if (fabs(*result) < EPSILON) *result = 0;
            break;
            
        case FUNC_SQRT:
            if (value < 0) {
                return CALC_ERROR("负数不能开平方根！");
            }
            *result = sqrt(value);
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

double degreeToRadian(double degree) {
    return degree * PI / 180.0;
}

double radianToDegree(double radian) {
    return radian * 180.0 / PI;
}

int isInfinite(double value) {
    return !isfinite(value) || fabs(value) > INFINITY_THRESHOLD;
}

int isUndefined(double value) {
    return isnan(value) || (value == INFINITY && value == -INFINITY);
} 