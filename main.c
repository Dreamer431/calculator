#include "calculator.h"

int main() {
    // 设置控制台代码页
    SetConsoleOutputCP(65001);  // UTF-8
    SetConsoleCP(65001);       // UTF-8
    
    char expression[MAX_EXPR];
    AngleMode mode = MODE_DEG;  // 默认使用角度模式
    
    printf("计算器启动 (默认使用角度模式)\n");
    printf("特殊命令：\n");
    printf("  mode     - 切换角度/弧度模式\n");
    printf("  q        - 退出程序\n");
    printf("函数支持：\n");
    printf("  sin(x)   - 正弦函数\n");
    printf("  cos(x)   - 余弦函数\n");
    printf("  tan(x)   - 正切函数\n");
    printf("  sqrt(x)  - 平方根\n");
    printf("  rad(x)   - 角度转弧度\n");
    printf("  deg(x)   - 弧度转角度\n");
    printf("常量支持：\n");
    printf("  pi       - 圆周率\n\n");
    
    while (1) {
        printf("\n请输入计算表达式 [%s]: ", mode == MODE_DEG ? "角度" : "弧度");
        if (fgets(expression, sizeof(expression), stdin) == NULL) {
            break;
        }
        
        // 去除换行符
        expression[strcspn(expression, "\n")] = 0;
        
        // 检查特殊命令
        if (strcmp(expression, "q") == 0 || strcmp(expression, "Q") == 0) {
            break;
        }
        
        if (strcmp(expression, "mode") == 0) {
            mode = (mode == MODE_DEG) ? MODE_RAD : MODE_DEG;
            printf("切换到%s模式\n", mode == MODE_DEG ? "角度" : "弧度");
            continue;
        }
        
        // 计算结果
        double result = evaluateExpression(expression, mode);
        
        // 显示结果
        if (isUndefined(result)) {
            printf("%s = 未定义\n", expression);
        } else if (isInfinite(result)) {
            printf("%s = %s无穷大\n", expression, result > 0 ? "" : "-");
        } else {
            printf("%s = %.*g\n", expression, PRECISION, result);
        }
    }
    
    return 0;
} 