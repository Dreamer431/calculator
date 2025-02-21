#include "calculator.h"

double evaluateExpression(const char* expr) {
    double numbers[MAX_EXPR];  // 数字栈
    char operators[MAX_EXPR];  // 运算符栈
    int numTop = -1;          // 数字栈顶
    int opTop = -1;          // 运算符栈顶
    int lastWasNumber = 0;    // 标记上一个token是否为数字
    
    // 处理表达式
    while (*expr) {
        // 跳过空格
        if (*expr == ' ') {
            expr++;
            continue;
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
                    
                    switch (op) {
                        case '+': numbers[++numTop] = a + b; break;
                        case '-': numbers[++numTop] = a - b; break;
                        case '*': numbers[++numTop] = a * b; break;
                        case '/':
                            if (b == 0) {
                                printf("错误：除数不能为0！\n");
                                exit(1);
                            }
                            numbers[++numTop] = a / b;
                            break;
                    }
                }
                operators[++opTop] = '*';
            }
            numbers[++numTop] = getNumber(&expr);
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
                    
                    switch (op) {
                        case '+': numbers[++numTop] = a + b; break;
                        case '-': numbers[++numTop] = a - b; break;
                        case '*': numbers[++numTop] = a * b; break;
                        case '/':
                            if (b == 0) {
                                printf("错误：除数不能为0！\n");
                                exit(1);
                            }
                            numbers[++numTop] = a / b;
                            break;
                    }
                }
                operators[++opTop] = '*';
            }
            operators[++opTop] = *expr;
            expr++;
            lastWasNumber = 0;
            continue;
        }
        
        // 如果是右括号
        if (*expr == ')') {
            // 计算括号内的所有运算
            while (opTop >= 0 && operators[opTop] != '(') {
                double b = numbers[numTop--];
                double a = numbers[numTop--];
                char op = operators[opTop--];
                
                switch (op) {
                    case '+': numbers[++numTop] = a + b; break;
                    case '-': numbers[++numTop] = a - b; break;
                    case '*': numbers[++numTop] = a * b; break;
                    case '/':
                        if (b == 0) {
                            printf("错误：除数不能为0！\n");
                            exit(1);
                        }
                        numbers[++numTop] = a / b;
                        break;
                }
            }
            
            // 弹出左括号
            if (opTop >= 0 && operators[opTop] == '(') {
                opTop--;  // 移除左括号
            } else {
                printf("错误：括号不匹配！\n");
                exit(1);
            }
            expr++;
            lastWasNumber = 1;  // 括号计算完的结果视为一个数字
            continue;
        }
        
        // 如果是运算符
        if (*expr == '+' || *expr == '-' || *expr == '*' || *expr == '/') {
            while (opTop >= 0 && operators[opTop] != '(' && 
                   getPriority(operators[opTop]) >= getPriority(*expr)) {
                double b = numbers[numTop--];
                double a = numbers[numTop--];
                char op = operators[opTop--];
                
                switch (op) {
                    case '+': numbers[++numTop] = a + b; break;
                    case '-': numbers[++numTop] = a - b; break;
                    case '*': numbers[++numTop] = a * b; break;
                    case '/':
                        if (b == 0) {
                            printf("错误：除数不能为0！\n");
                            exit(1);
                        }
                        numbers[++numTop] = a / b;
                        break;
                }
            }
            operators[++opTop] = *expr;
            expr++;
            lastWasNumber = 0;
            continue;
        }
        
        // 无效字符
        if (*expr != ' ' && *expr != '\n') {
            printf("错误：无效的字符 '%c'\n", *expr);
            exit(1);
        }
        expr++;
    }
    
    // 处理剩余的运算符
    while (opTop >= 0) {
        if (operators[opTop] == '(') {
            printf("错误：括号不匹配！\n");
            exit(1);
        }
        
        double b = numbers[numTop--];
        double a = numbers[numTop--];
        char op = operators[opTop--];
        
        switch (op) {
            case '+': numbers[++numTop] = a + b; break;
            case '-': numbers[++numTop] = a - b; break;
            case '*': numbers[++numTop] = a * b; break;
            case '/':
                if (b == 0) {
                    printf("错误：除数不能为0！\n");
                    exit(1);
                }
                numbers[++numTop] = a / b;
                break;
        }
    }
    
    return numbers[0];
} 