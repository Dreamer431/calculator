#include "calculator.h"

int main() {
    // 设置控制台代码页
    SetConsoleOutputCP(65001);  // UTF-8
    SetConsoleCP(65001);       // UTF-8
    
    char expression[MAX_EXPR];
    
    while (1) {
        printf("\n请输入计算表达式 (如: (1+2)*3, 输入q退出): ");
        if (fgets(expression, sizeof(expression), stdin) == NULL) {
            break;
        }
        
        // 去除换行符
        expression[strcspn(expression, "\n")] = 0;
        
        // 检查是否退出
        if (expression[0] == 'q' || expression[0] == 'Q') {
            break;
        }
        
        // 计算结果
        double result = evaluateExpression(expression);
        printf("%s = %.*g\n", expression, PRECISION, result);
    }
    
    return 0;
} 