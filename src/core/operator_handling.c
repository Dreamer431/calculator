#include "calculator.h"
#include <stdint.h>  // 添加对int64_t的支持

// 执行基本运算
CalcError performOperation(char op, double a, double b, double* result) {
    switch (op) {
        case '+':
            *result = a + b;
            break;
        case '-':
            *result = a - b;
            break;
        case '*':
            *result = a * b;
            break;
        case '/':
            if (fabs(b) < ABSOLUTE_ZERO_THRESHOLD) {
                return CALC_ERROR_CODE(ERR_DIV_BY_ZERO, "除数不能为0");
            }
            *result = a / b;
            break;
        case '^':
            // 处理特殊情况：0的负数次幂
            if (fabs(a) < ABSOLUTE_ZERO_THRESHOLD && b < 0) {
                return CALC_ERROR_CODE(ERR_UNDEFINED, "0的负数次幂未定义");
            }
            // 处理特殊情况：负数的小数次幂
            if (a < 0 && fabs(b - (int64_t)b) > EPSILON) {
                return CALC_ERROR_CODE(ERR_UNDEFINED, "负数不能开非整数次方根");
            }
            *result = pow(a, b);
            break;
        default:
            return CALC_ERROR_CODE(ERR_SYNTAX, "无效的运算符");
    }
    
    // 处理溢出和无穷大
    if (isInfinite(*result)) {
        return CALC_ERROR_CODE(ERR_OVERFLOW, "计算结果太大！");
    }
    
    // 处理接近整数的浮点数
    int64_t intValue;
    if (isCloseToInteger(*result, &intValue)) {
        *result = intValue;
    }
    
    return CALC_SUCCESS;
}

// 处理运算符栈
CalcError processOperators(double* numbers, int* numTop, char* operators, int* opTop, char stopAt, int processEqual) {
    // 处理运算符栈中的运算符
    // 注意：幂运算是右结合的，其他运算符是左结合的
    // stopAt 用于指示遇到什么字符停止（如左括号），或者用于优先级比较
    
    while (*opTop >= 0) {
        char stackOp = operators[*opTop];
        
        // 如果栈顶是左括号，停止处理（用于右括号匹配）
        if (stackOp == '(') {
            break;
        }
        
        int stackPriority = getPriority(stackOp);
        int stopPriority = getPriority(stopAt);
        
        // 决定是否处理栈顶运算符
        int shouldProcess;
        if (stackOp == '^') {
            // 幂运算是右结合的：只有栈顶优先级严格大于当前运算符时才处理
            shouldProcess = stackPriority > stopPriority;
        } else {
            // 其他运算符是左结合的：栈顶优先级大于等于当前运算符时处理
            shouldProcess = stackPriority >= stopPriority;
        }
        
        // processEqual 为真时表示处理剩余所有运算符（表达式结尾）
        if (processEqual && stopPriority < 0) {
            shouldProcess = 1;
        }
        
        if (!shouldProcess) break;
        
        if (*numTop < 1) {
            return CALC_ERROR_CODE(ERR_SYNTAX, "运算符使用不正确");
        }
        
        double b = numbers[(*numTop)--];
        double a = numbers[(*numTop)--];
        char op = operators[(*opTop)--];
        
        double result;
        CalcError err = performOperation(op, a, b, &result);
        if (err.code != 0) return err;
        
        numbers[++(*numTop)] = result;
    }
    
    return CALC_SUCCESS;
} 