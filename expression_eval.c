#include "calculator.h"

// 检查栈是否会溢出
static CalcError checkStackOverflow(int top, const char* stackName) {
    if (top >= MAX_EXPR - 1) {
        char errorMsg[100];
        snprintf(errorMsg, sizeof(errorMsg), "%s溢出，表达式过于复杂", stackName);
        return CALC_ERROR(errorMsg);
    }
    return CALC_SUCCESS;
}

// 抽取运算符计算逻辑
static CalcError performOperation(char op, double a, double b, double* result) {
    switch (op) {
        case '+': *result = a + b; break;
        case '-': *result = a - b; break;
        case '*': *result = a * b; break;
        case '/':
            if (b == 0) {
                return CALC_ERROR("除数不能为0");
            }
            *result = a / b;
            break;
        case '^':
            *result = pow(a, b);
            break;
        default:
            return CALC_ERROR("未知的运算符");
    }
    return CALC_SUCCESS;
}

// 处理运算符栈的计算
static CalcError processOperators(double* numbers, int* numTop, 
                                char* operators, int* opTop, 
                                char stopAt, int minPriority) {
    while (*opTop >= 0 && operators[*opTop] != stopAt && 
           getPriority(operators[*opTop]) >= minPriority) {
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

// 检查括号是否匹配
static CalcError checkBracketMatch(const char* expr) {
    int count = 0;
    int maxDepth = 0;
    const char* p = expr;
    
    while (*p) {
        if (*p == '(') {
            count++;
            if (count > maxDepth) maxDepth = count;
            if (maxDepth > 10) {  // 限制括号嵌套深度
                return CALC_ERROR("表达式过于复杂");
            }
        } else if (*p == ')') {
            count--;
            if (count < 0) {
                return CALC_ERROR("括号不匹配：右括号过多");
            }
        }
        p++;
    }
    
    if (count > 0) {
        return CALC_ERROR("括号不匹配：左括号过多");
    }
    return CALC_SUCCESS;
}

// 获取错误描述
const char* getErrorDescription(int errorCode) {
    switch (errorCode) {
        case 0:  // ERR_SUCCESS
            return "操作成功";
        case 1:  // ERR_SYNTAX
            return "语法错误";
        case ERR_DIV_BY_ZERO:
            return "除数不能为零";
        case ERR_INVALID_NUMBER:
            return "无效的数字格式";
        case ERR_OVERFLOW:
            return "数值溢出";
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

// 主函数修改为返回错误信息
CalcError evaluateExpression(const char* expr, AngleMode mode, double* result) {
    if (!expr || !*expr) {
        return CALC_ERROR("表达式不能为空");
    }

    // 首先检查括号匹配
    CalcError bracketErr = checkBracketMatch(expr);
    if (bracketErr.code != 0) {
        return bracketErr;
    }

    // 检查表达式结尾
    const char* lastChar = expr + strlen(expr) - 1;
    while (lastChar > expr && *lastChar == ' ') lastChar--;
    if (*lastChar == '+' || *lastChar == '-' || *lastChar == '*' || *lastChar == '/') {
        return CALC_ERROR("表达式不能以运算符结尾");
    }

    double numbers[MAX_EXPR];  // 数字栈
    char operators[MAX_EXPR];  // 运算符栈
    int numTop = -1;          // 数字栈顶
    int opTop = -1;          // 运算符栈顶
    int lastWasNumber = 0;    // 标记上一个token是否为数字
    CalcError err = CALC_SUCCESS;
    
    // 处理表达式
    while (*expr) {
        // 跳过空格
        if (*expr == ' ') {
            expr++;
            continue;
        }
        
        // 检查是否是函数或常量
        if (isalpha(*expr)) {
            // 检查是否是 pi
            if (strncmp(expr, "pi", 2) == 0 && (!expr[2] || !isalpha(expr[2]))) {
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
                expr += 2;
                lastWasNumber = 1;
                continue;
            }
            
            // 检查是否是函数
            FuncType func = getFunction(&expr);
            if (func != FUNC_NONE) {
                // 跳过空格
                while (*expr == ' ') expr++;
                
                // 必须跟着左括号
                if (*expr != '(') {
                    return CALC_ERROR("函数后必须跟着括号！");
                }
                
                // 处理括号内的表达式
                expr++; // 跳过左括号
                const char* endExpr = expr;
                int bracketCount = 1;
                
                // 找到对应的右括号
                while (bracketCount > 0 && *endExpr) {
                    if (*endExpr == '(') bracketCount++;
                    if (*endExpr == ')') bracketCount--;
                    endExpr++;
                }
                
                if (bracketCount > 0) {
                    return CALC_ERROR("括号不匹配！");
                }
                
                // 计算括号内的表达式
                size_t len = endExpr - expr - 1;  // 减1是为了不包含右括号
                char* subExpr = (char*)malloc(len + 1);  // +1 用于存储 \0
                if (subExpr == NULL) {
                    return CALC_ERROR("内存分配失败");
                }
                strncpy(subExpr, expr, len);
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
                    return funcErr;
                }
                
                err = checkStackOverflow(numTop + 1, "数字栈");
                if (err.code != 0) return err;
                numbers[++numTop] = funcResult;
                expr = endExpr;  // 移动到右括号后
                lastWasNumber = 1;
                continue;
            }
            
            // 如果不是函数也不是pi，则是无效字符
            return CALC_ERROR("无效的字符");
        }
        
        // 如果是数字
        if (isdigit(*expr) || *expr == '.') {
            // 如果前一个是右括号，插入乘号
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
            double num;
            CalcError numErr = getNumberWithError(&expr, &num);
            if (numErr.code != 0) {
                return numErr;
            }
            err = checkStackOverflow(numTop + 1, "数字栈");
            if (err.code != 0) return err;
            numbers[++numTop] = num;
            lastWasNumber = 1;
            continue;
        }
        
        // 如果是左括号
        if (*expr == '(') {
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
            operators[++opTop] = *expr;
            expr++;
            lastWasNumber = 0;
            continue;
        }
        
        // 如果是右括号
        if (*expr == ')') {
            // 检查空括号
            if (opTop >= 0 && operators[opTop] == '(' && 
                (numTop < 0 || (expr[-1] == '(' && !lastWasNumber))) {
                return CALC_ERROR("括号内必须有表达式");
            }
            
            // 计算括号内的所有运算
            err = processOperators(numbers, &numTop, operators, &opTop, '(', 0);
            if (err.code != 0) return err;
            
            // 弹出左括号
            if (opTop >= 0 && operators[opTop] == '(') {
                opTop--;  // 移除左括号
            } else {
                return CALC_ERROR("括号不匹配！");
            }
            expr++;
            lastWasNumber = 1;  // 括号计算完的结果视为一个数字
            continue;
        }
        
        // 如果是运算符
        if (*expr == '+' || *expr == '-' || *expr == '*' || *expr == '/' || *expr == '^') {
            // 检查连续运算符，但允许负号出现在表达式开头或左括号后
            if (!lastWasNumber && *expr == '-' && (isdigit(expr[1]) || expr[1] == '.' || 
                (expr[1] == 'p' && expr[2] == 'i'))) {
                // 处理负数
                expr++;  // 跳过负号
                if (expr[0] == 'p' && expr[1] == 'i') {
                    // 处理 -pi
                    err = checkStackOverflow(numTop + 1, "数字栈");
                    if (err.code != 0) return err;
                    numbers[++numTop] = -PI;
                    expr += 2;
                    lastWasNumber = 1;
                } else {
                    // 处理负数
                    double num;
                    CalcError numErr = getNumberWithError(&expr, &num);
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
            
            if (!lastWasNumber && *expr != '-') {
                return CALC_ERROR("运算符使用不正确");
            }
            
            char currentOp = *expr;
            int currentPriority = getPriority(currentOp);
            
            // 处理栈中的运算符
            while (opTop >= 0 && operators[opTop] != '(') {
                // 幂运算是右结合的，其他运算符是左结合的
                if (currentOp == '^' && operators[opTop] == '^') {
                    break;  // 遇到连续的幂运算，保持在栈中
                }
                if (getPriority(operators[opTop]) >= currentPriority) {
                    double b = numbers[numTop--];
                    double a = numbers[numTop--];
                    char op = operators[opTop--];
                    
                    double result;
                    CalcError err = performOperation(op, a, b, &result);
                    if (err.code != 0) return err;
                    
                    numbers[++numTop] = result;
                } else {
                    break;
                }
            }
            
            err = checkStackOverflow(opTop + 1, "运算符栈");
            if (err.code != 0) return err;
            operators[++opTop] = currentOp;
            expr++;
            lastWasNumber = 0;
            continue;
        }
        
        // 无效字符
        if (*expr != ' ' && *expr != '\n') {
            return CALC_ERROR("无效的字符");
        }
        expr++;
    }
    
    // 处理剩余的运算符
    err = processOperators(numbers, &numTop, operators, &opTop, '(', 0);
    if (err.code != 0) return err;
    
    if (numTop != 0) {
        return CALC_ERROR("表达式格式错误");
    }
    
    *result = numbers[0];
    return CALC_SUCCESS;
} 