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
    const char* start = *expr; // 记录起始位置用于计算错误位置
    
    // 跳过空格
    while (**expr == ' ') (*expr)++;
    
    // 确保至少有一个数字
    if (!isdigit(**expr) && **expr != '.') {
        return CALC_ERROR_POS("无效的数字格式", *expr - start);
    }
    
    // 处理整数和小数部分
    while (isdigit(**expr) || **expr == '.' || tolower(**expr) == 'e') {
        if (**expr == '.') {
            if (hasDecimal || hasExponent) {
                return CALC_ERROR_POS("数字格式不正确，多个小数点！", *expr - start);
            }
            hasDecimal = 1;
            (*expr)++;
        } else if (tolower(**expr) == 'e') {
            if (hasExponent) {
                return CALC_ERROR_POS("数字格式不正确，多个指数符号！", *expr - start);
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
                if (tolower(**expr) == 'e') {
                    return CALC_ERROR_POS("数字格式不正确，多个指数符号！", *expr - start);
                }
                return CALC_ERROR_POS("指数部分必须是整数！", *expr - start);
            }
            while (isdigit(**expr)) {
                exponent = exponent * 10 + (**expr - '0');
                if (exponent > 308) { // 防止指数过大
                    return CALC_ERROR_POS("数字太大！", *expr - start);
                }
                (*expr)++;
            }
            
            // 检查是否还有指数符号
            if (tolower(**expr) == 'e') {
                return CALC_ERROR_POS("数字格式不正确，多个指数符号！", *expr - start);
            }
            
            // 检查指数后是否有小数点
            if (**expr == '.') {
                return CALC_ERROR_POS("指数部分必须是整数！", *expr - start);
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
                    return CALC_ERROR_POS("数字太大！", *expr - start);
                }
                // 检查整数部分溢出
                if (number > DBL_MAX / 10) {
                    return CALC_ERROR_POS("数字太大！", *expr - start);
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
            return CALC_ERROR_POS("数字太大！", *expr - start);
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

// 获取函数类型
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
    if (strcmp(func, "asin") == 0) return FUNC_ASIN;
    if (strcmp(func, "acos") == 0) return FUNC_ACOS;
    if (strcmp(func, "atan") == 0) return FUNC_ATAN;
    if (strcmp(func, "sqrt") == 0) return FUNC_SQRT;
    if (strcmp(func, "log") == 0) return FUNC_LOG;
    if (strcmp(func, "ln") == 0) return FUNC_LN;
    if (strcmp(func, "abs") == 0) return FUNC_ABS;
    if (strcmp(func, "rad") == 0) return FUNC_RAD;
    if (strcmp(func, "deg") == 0) return FUNC_DEG;
    
    return FUNC_NONE;
}

// 获取运算符优先级
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