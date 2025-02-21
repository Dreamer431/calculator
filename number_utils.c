#include "calculator.h"

double getNumber(const char** expr) {
    double number = 0;
    double decimal = 0.1;
    int hasDecimal = 0;
    int decimalCount = 0;
    
    // 跳过空格
    while (**expr == ' ') (*expr)++;
    
    while (isdigit(**expr) || **expr == '.') {
        if (**expr == '.') {
            if (hasDecimal) {
                printf("错误：数字格式不正确，多个小数点！\n");
                exit(1);
            }
            hasDecimal = 1;
        } else if (hasDecimal) {
            if (decimalCount < PRECISION) {
                number = number + (**expr - '0') * decimal;
                decimal *= 0.1;
                decimalCount++;
            }
        } else {
            number = number * 10 + (**expr - '0');
        }
        (*expr)++;
    }
    
    return number;
}

int getPriority(char op) {
    switch (op) {
        case '+':
        case '-':
            return 1;
        case '*':
        case '/':
            return 2;
        case '(':
            return 0;
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

double calculateFunction(FuncType func, double value, AngleMode mode) {
    // 检查输入是否有效
    if (isUndefined(value)) {
        return value;
    }

    double angle;
    switch (func) {
        case FUNC_SIN:
            angle = (mode == MODE_DEG) ? degreeToRadian(value) : value;
            return sin(angle);
            
        case FUNC_COS:
            angle = (mode == MODE_DEG) ? degreeToRadian(value) : value;
            return cos(angle);
            
        case FUNC_TAN:
            angle = (mode == MODE_DEG) ? degreeToRadian(value) : value;
            // 检查是否在无穷大点
            if (fabs(cos(angle)) < EPSILON) {
                printf("警告：tan在%g%s处趋近于无穷大\n", 
                       value, (mode == MODE_DEG) ? "°" : " rad");
                return INFINITY;
            }
            return tan(angle);
            
        case FUNC_SQRT:
            if (value < 0) {
                printf("错误：负数不能开平方根！\n");
                return NAN;
            }
            return sqrt(value);
            
        case FUNC_RAD:
            return degreeToRadian(value);
            
        case FUNC_DEG:
            return radianToDegree(value);
            
        default:
            return value;
    }
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
    return isnan(value);
} 