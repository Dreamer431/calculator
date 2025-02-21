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