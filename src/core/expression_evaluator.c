#include "calculator.h"

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
    
    // 用于跟踪当前位置
    const char* current_pos = expr;
    int position = 0;
    
    // 处理表达式
    while (*current_pos) {
        // 跳过空格
        if (*current_pos == ' ') {
            current_pos++;
            position++;
            continue;
        }
        
        // 检查是否是函数或常量
        if (isalpha(*current_pos)) {
            // 检查是否是 pi
            if (strncmp(current_pos, "pi", 2) == 0 && (!current_pos[2] || !isalpha(current_pos[2]))) {
                // 如果前一个是数字或右括号，插入乘号
                if (lastWasNumber) {
                    while (opTop >= 0 && operators[opTop] != '(' && 
                           getPriority(operators[opTop]) >= getPriority('*')) {
                        double b = numbers[numTop--];
                        double a = numbers[numTop--];
                        char op = operators[opTop--];
                        
                        double result;
                        CalcError err = performOperation(op, a, b, &result);
                        if (err.code != 0) return err;
                        
                        numbers[++numTop] = result;
                    }
                    err = checkStackOverflow(opTop + 1, "运算符栈");
                    if (err.code != 0) return err;
                    operators[++opTop] = '*';
                }
                err = checkStackOverflow(numTop + 1, "数字栈");
                if (err.code != 0) return err;
                numbers[++numTop] = PI;
                current_pos += 2;
                lastWasNumber = 1;
                continue;
            }
            
            // 检查是否是 e（自然对数的底）
            if (*current_pos == 'e' && (!current_pos[1] || !isalpha(current_pos[1]))) {
                // 如果前一个是数字或右括号，插入乘号
                if (lastWasNumber) {
                    while (opTop >= 0 && operators[opTop] != '(' && 
                           getPriority(operators[opTop]) >= getPriority('*')) {
                        double b = numbers[numTop--];
                        double a = numbers[numTop--];
                        char op = operators[opTop--];
                        
                        double result;
                        CalcError err = performOperation(op, a, b, &result);
                        if (err.code != 0) return err;
                        
                        numbers[++numTop] = result;
                    }
                    err = checkStackOverflow(opTop + 1, "运算符栈");
                    if (err.code != 0) return err;
                    operators[++opTop] = '*';
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
                    return CALC_ERROR_POS("函数后必须跟着括号！", position);
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
                    return CALC_ERROR_POS("括号不匹配！", position);
                }
                
                // 计算括号内的表达式
                size_t len = endExpr - current_pos - 1;  // 减1是为了不包含右括号
                char* subExpr = (char*)malloc(len + 1);  // +1 用于存储 \0
                if (subExpr == NULL) {
                    return CALC_ERROR_POS("内存分配失败", position);
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
                    funcErr.position = position; // 更新错误位置
                    return funcErr;
                }
                
                err = checkStackOverflow(numTop + 1, "数字栈");
                if (err.code != 0) return err;
                numbers[++numTop] = funcResult;
                
                current_pos = endExpr;
                lastWasNumber = 1;  // 函数计算结果视为一个数字
                continue;
            } else {
                return CALC_ERROR_POS("无效的字符", position);
            }
        }
        
        // 如果是数字或小数点
        if (isdigit(*current_pos) || *current_pos == '.') {
            // 如果前一个是数字或右括号，插入乘号
            if (lastWasNumber) {
                while (opTop >= 0 && operators[opTop] != '(' && 
                       getPriority(operators[opTop]) >= getPriority('*')) {
                    double b = numbers[numTop--];
                    double a = numbers[numTop--];
                    char op = operators[opTop--];
                    
                    double result;
                    CalcError err = performOperation(op, a, b, &result);
                    if (err.code != 0) return err;
                    
                    numbers[++numTop] = result;
                }
                err = checkStackOverflow(opTop + 1, "运算符栈");
                if (err.code != 0) return err;
                operators[++opTop] = '*';
            }
            
            // 获取数字
            double num;
            CalcError numErr = getNumberWithError(&current_pos, &num);
            if (numErr.code != 0) {
                numErr.position += position;  // 更新错误位置
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
                while (opTop >= 0 && operators[opTop] != '(' && 
                       getPriority(operators[opTop]) >= getPriority('*')) {
                    double b = numbers[numTop--];
                    double a = numbers[numTop--];
                    char op = operators[opTop--];
                    
                    double result;
                    CalcError err = performOperation(op, a, b, &result);
                    if (err.code != 0) return err;
                    
                    numbers[++numTop] = result;
                }
                err = checkStackOverflow(opTop + 1, "运算符栈");
                if (err.code != 0) return err;
                operators[++opTop] = '*';
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
                return CALC_ERROR_POS("括号内必须有表达式", position);
            }
            
            // 计算括号内的所有运算
            err = processOperators(numbers, &numTop, operators, &opTop, '(', 0);
            if (err.code != 0) return err;
            
            // 弹出左括号
            if (opTop >= 0 && operators[opTop] == '(') {
                opTop--;  // 移除左括号
            } else {
                return CALC_ERROR_POS("括号不匹配！", position);
            }
            current_pos++;
            lastWasNumber = 1;  // 括号计算完的结果视为一个数字
            continue;
        }
        
        // 如果是运算符
        if (*current_pos == '+' || *current_pos == '-' || *current_pos == '*' || *current_pos == '/' || *current_pos == '^') {
            // 检查连续运算符，但允许负号出现在表达式开头或左括号后
            if (!lastWasNumber && *current_pos == '-' && (isdigit(current_pos[1]) || current_pos[1] == '.' || 
                (current_pos[1] == 'p' && current_pos[2] == 'i') || current_pos[1] == 'e')) {
                // 处理负数
                current_pos++;  // 跳过负号
                if (current_pos[0] == 'p' && current_pos[1] == 'i') {
                    // 处理 -pi
                    err = checkStackOverflow(numTop + 1, "数字栈");
                    if (err.code != 0) return err;
                    numbers[++numTop] = -PI;
                    current_pos += 2;
                    lastWasNumber = 1;
                } else if (current_pos[0] == 'e' && (!current_pos[1] || !isalpha(current_pos[1]))) {
                    // 处理 -e
                    err = checkStackOverflow(numTop + 1, "数字栈");
                    if (err.code != 0) return err;
                    numbers[++numTop] = -E;
                    current_pos++;
                    lastWasNumber = 1;
                } else {
                    // 处理负数
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
            
            if (!lastWasNumber && *current_pos != '-') {
                return CALC_ERROR_POS("运算符使用不正确", position);
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
        return CALC_ERROR_POS("无效的字符", position);
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