#include "calculator.h"

// 获取错误描述
const char* getErrorDescription(int errorCode) {
    switch (errorCode) {
        case ERR_SUCCESS:
            return "成功";
        case ERR_SYNTAX:
            return "语法错误";
        case ERR_DIV_BY_ZERO:
            return "除数不能为0";
        case ERR_INVALID_NUMBER:
            return "无效的数字";
        case ERR_OVERFLOW:
            return "数字溢出";
        case ERR_UNDEFINED:
            return "结果未定义";
        case ERR_INVALID_FUNCTION:
            return "无效的函数";
        case ERR_INVALID_ARGUMENT:
            return "无效的参数";
        case ERR_STACK_OVERFLOW:
            return "表达式过于复杂";
        case ERR_MISSING_PARENTHESIS:
            return "括号不匹配";
        case ERR_EMPTY_EXPRESSION:
            return "表达式不能为空";
        default:
            return "未知错误";
    }
}

// 检查栈溢出
CalcError checkStackOverflow(int stackSize, const char* stackName) {
    if (stackSize >= MAX_EXPR) {
        char errorMsg[100];
        snprintf(errorMsg, sizeof(errorMsg), "%s溢出，表达式过于复杂", stackName);
        return CALC_ERROR_CODE(ERR_STACK_OVERFLOW, errorMsg);
    }
    return CALC_SUCCESS;
}

// 检查括号匹配
CalcError checkBracketMatch(const char* expr) {
    int leftCount = 0;
    int rightCount = 0;
    int position = 0;
    
    while (*expr) {
        if (*expr == '(') {
            leftCount++;
        } else if (*expr == ')') {
            rightCount++;
            if (rightCount > leftCount) {
                return CALC_ERROR_POS("括号不匹配：右括号过多", position);
            }
        }
        expr++;
        position++;
    }
    
    if (leftCount > rightCount) {
        return CALC_ERROR_CODE(ERR_MISSING_PARENTHESIS, "括号不匹配：左括号过多");
    }
    
    return CALC_SUCCESS;
} 