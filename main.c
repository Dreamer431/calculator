#include "calculator.h"

int main() {
    // 设置控制台代码页
    SetConsoleOutputCP(65001);  // UTF-8
    SetConsoleCP(65001);       // UTF-8
    
    char expression[MAX_EXPR];
    char history[5][MAX_EXPR] = {"", "", "", "", ""};  // 保存最近5条历史记录
    int historyCount = 0;
    AngleMode mode = MODE_DEG;  // 默认使用角度模式
    
    printf("计算器启动 (默认使用角度模式)\n");
    printf("特殊命令：\n");
    printf("  mode     - 切换角度/弧度模式\n");
    printf("  history  - 显示历史记录\n");
    printf("  help     - 显示帮助信息\n");
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
        
        if (strcmp(expression, "help") == 0) {
            printf("计算器使用帮助：\n");
            printf("1. 支持的运算：+, -, *, /, ^ (幂运算)\n");
            printf("2. 支持的函数：sin, cos, tan, sqrt, rad, deg\n");
            printf("3. 支持的常量：pi (3.14159...)\n");
            printf("4. 角度模式下，三角函数的参数单位为角度\n");
            printf("5. 弧度模式下，三角函数的参数单位为弧度\n");
            printf("6. 使用括号可以改变计算优先级\n");
            printf("7. 例子：\n");
            printf("   - 1 + 2 * 3 = 7\n");
            printf("   - (1 + 2) * 3 = 9\n");
            printf("   - sin(30) = 0.5 (角度模式)\n");
            printf("   - sin(pi/6) = 0.5 (弧度模式)\n");
            continue;
        }
        
        if (strcmp(expression, "history") == 0) {
            printf("历史记录：\n");
            for (int i = 0; i < historyCount; i++) {
                printf("%d: %s\n", i + 1, history[i]);
            }
            if (historyCount == 0) {
                printf("暂无历史记录\n");
            }
            continue;
        }
        
        // 检查表达式是否为空
        if (strlen(expression) == 0) {
            continue;
        }
        
        // 计算结果
        double result;
        CalcError err = evaluateExpression(expression, mode, &result);
        
        // 显示结果
        if (err.code != 0) {
            printf("错误: %s\n", err.message);
            
            // 添加额外的错误提示信息
            if (strstr(err.message, "语法") || strstr(err.message, "无效的字符")) {
                printf("提示：请检查表达式语法是否正确\n");
            } else if (strstr(err.message, "数字格式") || strstr(err.message, "数字太大")) {
                printf("提示：请检查数字格式是否正确\n");
            } else if (strstr(err.message, "括号")) {
                printf("提示：请检查括号是否匹配\n");
            }
        } else if (isUndefined(result)) {
            printf("%s = 未定义\n", expression);
            
            // 添加到历史记录
            if (historyCount < 5) {
                sprintf(history[historyCount++], "%s = 未定义", expression);
            } else {
                // 移动历史记录
                for (int i = 0; i < 4; i++) {
                    strcpy(history[i], history[i + 1]);
                }
                sprintf(history[4], "%s = 未定义", expression);
            }
        } else if (isInfinite(result)) {
            printf("%s = %s无穷大\n", expression, result > 0 ? "" : "-");
            
            // 添加到历史记录
            if (historyCount < 5) {
                sprintf(history[historyCount++], "%s = %s无穷大", expression, result > 0 ? "" : "-");
            } else {
                // 移动历史记录
                for (int i = 0; i < 4; i++) {
                    strcpy(history[i], history[i + 1]);
                }
                sprintf(history[4], "%s = %s无穷大", expression, result > 0 ? "" : "-");
            }
        } else {
            printf("%s = %.*g\n", expression, PRECISION, result);
            
            // 添加到历史记录
            if (historyCount < 5) {
                sprintf(history[historyCount++], "%s = %.*g", expression, PRECISION, result);
            } else {
                // 移动历史记录
                for (int i = 0; i < 4; i++) {
                    strcpy(history[i], history[i + 1]);
                }
                sprintf(history[4], "%s = %.*g", expression, PRECISION, result);
            }
        }
    }
    
    return 0;
} 