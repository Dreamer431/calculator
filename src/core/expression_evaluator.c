#include "calculator.h"

/**
 * 处理隐式乘法（如 2pi, 2(3+4), (2)(3) 等情况）
 * 当上一个 token 是数字或右括号，下一个是数字、常量或左括号时插入乘号
 * 
 * @param numbers   数字栈
 * @param numTop    数字栈顶指针
 * @param operators 运算符栈
 * @param opTop     运算符栈顶指针
 * @return 成功返回 CALC_SUCCESS，否则返回错误
 */
static CalcError handleImplicitMultiply(double* numbers, int* numTop, 
                                         char* operators, int* opTop) {
    // 先处理优先级不低于乘法的运算符
    while (*opTop >= 0 && operators[*opTop] != '(' && 
           getPriority(operators[*opTop]) >= getPriority('*')) {
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
    
    // 检查栈溢出并添加乘号
    CalcError err = checkStackOverflow(*opTop + 1, "运算符栈");
    if (err.code != 0) return err;
    operators[++(*opTop)] = '*';
    
    return CALC_SUCCESS;
}

// 主函数修改为返回错误信息
CalcError evaluateExpression(const char* expr, AngleMode mode, double* result) {
    if (!expr || !*expr) {
        return CALC_ERROR_CODE(ERR_EMPTY_EXPRESSION, "表达式不能为空");
    }

    // 首先检查括号匹配
    CalcError bracketErr = checkBracketMatch(expr);
    if (bracketErr.code != 0) {
        return bracketErr;
    }

    // 检查表达式结尾
    const char* lastChar = expr + strlen(expr) - 1;
    int lastCharPos = strlen(expr) - 1;
    while (lastChar > expr && *lastChar == ' ') {
        lastChar--;
        lastCharPos--;
    }
    if (*lastChar == '+' || *lastChar == '-' || *lastChar == '*' || *lastChar == '/') {
        return CALC_ERROR_POS("表达式不能以运算符结尾", lastCharPos);
    }

    double numbers[MAX_EXPR];  // 数字栈
    char operators[MAX_EXPR];  // 运算符栈
    int numTop = -1;          // 数字栈顶
    int opTop = -1;          // 运算符栈顶
    int lastWasNumber = 0;    // 标记上一个token是否为数字
    CalcError err = CALC_SUCCESS;
    
    // 用于跟踪当前位置（使用指针差值计算位置，更准确）
    const char* current_pos = expr;
    // 宏用于计算当前位置
    #define CURRENT_POS ((int)(current_pos - expr))
    
    // 处理表达式
    while (*current_pos) {
        // 跳过空格
        if (*current_pos == ' ') {
            current_pos++;
            continue;
        }
        
        // 检查是否是函数或常量
        if (isalpha(*current_pos)) {
            // 检查是否是 pi（大小写不敏感）
            if ((tolower(current_pos[0]) == 'p' && tolower(current_pos[1]) == 'i') && 
                (!current_pos[2] || !isalpha(current_pos[2]))) {
                // 如果前一个是数字或右括号，插入乘号
                if (lastWasNumber) {
                    err = handleImplicitMultiply(numbers, &numTop, operators, &opTop);
                    if (err.code != 0) return err;
                }
                err = checkStackOverflow(numTop + 1, "数字栈");
                if (err.code != 0) return err;
                numbers[++numTop] = PI;
                current_pos += 2;
                lastWasNumber = 1;
                continue;
            }
            
            // 检查是否是 e（自然对数的底，大小写不敏感）
            if (tolower(*current_pos) == 'e' && (!current_pos[1] || !isalpha(current_pos[1]))) {
                // 如果前一个是数字或右括号，插入乘号
                if (lastWasNumber) {
                    err = handleImplicitMultiply(numbers, &numTop, operators, &opTop);
                    if (err.code != 0) return err;
                }
                err = checkStackOverflow(numTop + 1, "数字栈");
                if (err.code != 0) return err;
                numbers[++numTop] = E;
                current_pos++;
                lastWasNumber = 1;
                continue;
            }
            
            // 检查是否是函数
            FuncType func = getFunction(&current_pos);
            if (func != FUNC_NONE) {
                // 跳过空格
                while (*current_pos == ' ') current_pos++;
                
                // 必须跟着左括号
                if (*current_pos != '(') {
                    return CALC_ERROR_POS("函数后必须跟着括号！", CURRENT_POS);
                }
                
                // 处理括号内的表达式
                current_pos++; // 跳过左括号
                const char* endExpr = current_pos;
                int bracketCount = 1;
                
                // 找到对应的右括号
                while (bracketCount > 0 && *endExpr) {
                    if (*endExpr == '(') bracketCount++;
                    if (*endExpr == ')') bracketCount--;
                    endExpr++;
                }
                
                if (bracketCount > 0) {
                    return CALC_ERROR_POS("括号不匹配！", CURRENT_POS);
                }
                
                // 计算括号内的表达式
                size_t len = endExpr - current_pos - 1;  // 减1是为了不包含右括号
                char* subExpr = (char*)malloc(len + 1);  // +1 用于存储 \0
                if (subExpr == NULL) {
                    return CALC_ERROR_POS("内存分配失败", CURRENT_POS);
                }
                strncpy(subExpr, current_pos, len);
                subExpr[len] = '\0';
                
                double value;
                CalcError subExprErr = evaluateExpression(subExpr, mode, &value);
                free(subExpr);
                
                if (subExprErr.code != 0) {
                    return subExprErr;
                }
                
                // 计算函数值
                double funcResult;
                CalcError funcErr = calculateFunctionWithError(func, value, mode, &funcResult);
                if (funcErr.code != 0) {
                    funcErr.position = CURRENT_POS; // 更新错误位置
                    return funcErr;
                }
                
                err = checkStackOverflow(numTop + 1, "数字栈");
                if (err.code != 0) return err;
                numbers[++numTop] = funcResult;
                
                current_pos = endExpr;
                lastWasNumber = 1;  // 函数计算结果视为一个数字
                continue;
            } else {
                return CALC_ERROR_POS("无效的字符", CURRENT_POS);
            }
        }
        
        // 如果是数字或小数点
        if (isdigit(*current_pos) || *current_pos == '.') {
            // 如果前一个是数字或右括号，插入乘号
            if (lastWasNumber) {
                err = handleImplicitMultiply(numbers, &numTop, operators, &opTop);
                if (err.code != 0) return err;
            }
            
            // 获取数字
            double num;
            CalcError numErr = getNumberWithError(&current_pos, &num);
            if (numErr.code != 0) {
                numErr.position += CURRENT_POS;  // 更新错误位置
                return numErr;
            }
            
            // 添加到数字栈
            err = checkStackOverflow(numTop + 1, "数字栈");
            if (err.code != 0) return err;
            numbers[++numTop] = num;
            lastWasNumber = 1;
            continue;
        }
        
        // 如果是左括号
        if (*current_pos == '(') {
            // 如果前一个是数字或右括号，插入乘号
            if (lastWasNumber) {
                err = handleImplicitMultiply(numbers, &numTop, operators, &opTop);
                if (err.code != 0) return err;
            }
            
            err = checkStackOverflow(opTop + 1, "运算符栈");
            if (err.code != 0) return err;
            operators[++opTop] = *current_pos;
            current_pos++;
            lastWasNumber = 0;
            continue;
        }
        
        // 如果是右括号
        if (*current_pos == ')') {
            // 检查空括号
            if (opTop >= 0 && operators[opTop] == '(' && 
                (numTop < 0 || (current_pos[-1] == '(' && !lastWasNumber))) {
                return CALC_ERROR_POS("括号内必须有表达式", CURRENT_POS);
            }
            
            // 计算括号内的所有运算
            err = processOperators(numbers, &numTop, operators, &opTop, '(', 0);
            if (err.code != 0) return err;
            
            // 弹出左括号
            if (opTop >= 0 && operators[opTop] == '(') {
                opTop--;  // 移除左括号
            } else {
                return CALC_ERROR_POS("括号不匹配！", CURRENT_POS);
            }
            current_pos++;
            lastWasNumber = 1;  // 括号计算完的结果视为一个数字
            continue;
        }
        
        // 如果是运算符
        if (*current_pos == '+' || *current_pos == '-' || *current_pos == '*' || *current_pos == '/' || *current_pos == '^') {
            // 检查连续运算符，但允许负号出现在表达式开头或左括号后
            // 支持: -3, -.5, -pi, -PI, -e, -E, -sin(30), -cos(60) 等
            if (!lastWasNumber && *current_pos == '-') {
                char nextChar = current_pos[1];
                // 检查下一个字符是否可以跟在负号后面
                if (isdigit(nextChar) || nextChar == '.' || isalpha(nextChar)) {
                    // 处理负数或负值表达式
                    current_pos++;  // 跳过负号
                    
                    // 检查是否是常量 pi（大小写不敏感）
                    if ((tolower(current_pos[0]) == 'p' && tolower(current_pos[1]) == 'i') &&
                        (!current_pos[2] || !isalpha(current_pos[2]))) {
                        err = checkStackOverflow(numTop + 1, "数字栈");
                        if (err.code != 0) return err;
                        numbers[++numTop] = -PI;
                        current_pos += 2;
                        lastWasNumber = 1;
                    }
                    // 检查是否是常量 e（大小写不敏感）
                    else if (tolower(current_pos[0]) == 'e' && (!current_pos[1] || !isalpha(current_pos[1]))) {
                        err = checkStackOverflow(numTop + 1, "数字栈");
                        if (err.code != 0) return err;
                        numbers[++numTop] = -E;
                        current_pos++;
                        lastWasNumber = 1;
                    }
                    // 检查是否是函数（如 -sin(30)）
                    else if (isalpha(current_pos[0])) {
                        FuncType func = getFunction(&current_pos);
                        if (func != FUNC_NONE) {
                            // 跳过空格
                            while (*current_pos == ' ') current_pos++;
                            
                            if (*current_pos != '(') {
                                return CALC_ERROR_POS("函数后必须跟着括号！", CURRENT_POS);
                            }
                            
                            // 处理括号内的表达式
                            current_pos++; // 跳过左括号
                            const char* endExpr = current_pos;
                            int bracketCount = 1;
                            
                            while (bracketCount > 0 && *endExpr) {
                                if (*endExpr == '(') bracketCount++;
                                if (*endExpr == ')') bracketCount--;
                                endExpr++;
                            }
                            
                            if (bracketCount > 0) {
                                return CALC_ERROR_POS("括号不匹配！", CURRENT_POS);
                            }
                            
                            size_t len = endExpr - current_pos - 1;
                            char* subExpr = (char*)malloc(len + 1);
                            if (subExpr == NULL) {
                                return CALC_ERROR_POS("内存分配失败", CURRENT_POS);
                            }
                            strncpy(subExpr, current_pos, len);
                            subExpr[len] = '\0';
                            
                            double value;
                            CalcError subExprErr = evaluateExpression(subExpr, mode, &value);
                            free(subExpr);
                            
                            if (subExprErr.code != 0) {
                                return subExprErr;
                            }
                            
                            double funcResult;
                            CalcError funcErr = calculateFunctionWithError(func, value, mode, &funcResult);
                            if (funcErr.code != 0) {
                                funcErr.position = CURRENT_POS;
                                return funcErr;
                            }
                            
                            err = checkStackOverflow(numTop + 1, "数字栈");
                            if (err.code != 0) return err;
                            numbers[++numTop] = -funcResult;  // 取负值
                            
                            current_pos = endExpr;
                            lastWasNumber = 1;
                        } else {
                            return CALC_ERROR_POS("无效的字符", CURRENT_POS);
                        }
                    }
                    // 处理普通负数
                    else {
                        double num;
                        CalcError numErr = getNumberWithError(&current_pos, &num);
                        if (numErr.code != 0) {
                            return numErr;
                        }
                        err = checkStackOverflow(numTop + 1, "数字栈");
                        if (err.code != 0) return err;
                        numbers[++numTop] = -num;
                        lastWasNumber = 1;
                    }
                    continue;
                }
            }
            
            if (!lastWasNumber && *current_pos != '-') {
                return CALC_ERROR_POS("运算符使用不正确", CURRENT_POS);
            }
            
            char currentOp = *current_pos;
            
            // 检查是否有足够的数字用于运算
            err = processOperators(numbers, &numTop, operators, &opTop, currentOp, 0);
            if (err.code != 0) return err;
            
            // 将当前运算符入栈
            err = checkStackOverflow(opTop + 1, "运算符栈");
            if (err.code != 0) return err;
            operators[++opTop] = currentOp;
            
            current_pos++;
            lastWasNumber = 0;
            continue;
        }
        
        // 无效字符
        return CALC_ERROR_POS("无效的字符", CURRENT_POS);
    }
    
    // 处理剩余的运算符
    err = processOperators(numbers, &numTop, operators, &opTop, '\0', 1);
    if (err.code != 0) return err;
    
    // 检查结果
    if (numTop != 0 || opTop != -1) {
        return CALC_ERROR_CODE(ERR_SYNTAX, "表达式不完整");
    }
    
    *result = numbers[0];
    return CALC_SUCCESS;
}