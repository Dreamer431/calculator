#include "calculator.h"

// 添加历史记录管理函数
void addToHistory(char history[][MAX_EXPR], int* historyCount, const char* entry) {
    if (*historyCount < 5) {
        // 还有空间，直接添加
        strcpy(history[*historyCount], entry);
        (*historyCount)++;
    } else {
        // 移动历史记录，删除最旧的
        for (int i = 0; i < 4; i++) {
            strcpy(history[i], history[i + 1]);
        }
        // 添加新记录
        strcpy(history[4], entry);
    }
}

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
    printf("基本函数：\n");
    printf("  sin(x)   - 正弦函数\n");
    printf("  cos(x)   - 余弦函数\n");
    printf("  tan(x)   - 正切函数\n");
    printf("  sqrt(x)  - 平方根\n");
    printf("  abs(x)   - 绝对值\n");
    printf("反三角函数：\n");
    printf("  asin(x)  - 反正弦函数\n");
    printf("  acos(x)  - 反余弦函数\n");
    printf("  atan(x)  - 反正切函数\n");
    printf("对数函数：\n");
    printf("  log(x)   - 常用对数（以10为底）\n");
    printf("  ln(x)    - 自然对数\n");
    printf("单位转换：\n");
    printf("  rad(x)   - 角度转弧度\n");
    printf("  deg(x)   - 弧度转角度\n");
    printf("常量支持：\n");
    printf("  pi       - 圆周率 (3.14159...)\n");
    printf("  e        - 自然对数的底 (2.71828...)\n\n");
    
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
            printf("2. 支持的函数：\n");
            printf("   - 三角函数：sin, cos, tan\n");
            printf("   - 反三角函数：asin, acos, atan\n");
            printf("   - 对数函数：log(常用对数), ln(自然对数)\n");
            printf("   - 其他函数：sqrt(平方根), abs(绝对值), rad(角度转弧度), deg(弧度转角度)\n");
            printf("3. 支持的常量：\n");
            printf("   - pi：圆周率 (3.14159...)\n");
            printf("   - e：自然对数的底 (2.71828...)\n");
            printf("4. 角度模式下，三角函数的参数单位为角度\n");
            printf("5. 弧度模式下，三角函数的参数单位为弧度\n");
            printf("6. 使用括号可以改变计算优先级\n");
            printf("7. 例子：\n");
            printf("   - 1 + 2 * 3 = 7\n");
            printf("   - (1 + 2) * 3 = 9\n");
            printf("   - sin(30) = 0.5 (角度模式)\n");
            printf("   - sin(pi/6) = 0.5 (弧度模式)\n");
            printf("   - log(100) = 2\n");
            printf("   - ln(e^2) ≈ 2 (e为自然对数的底，约等于2.71828)\n");
            printf("   - abs(-3) = 3\n");
            printf("   - asin(0.5) = 30 (角度模式)\n");
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
            
            // 如果有错误位置信息，显示错误位置
            if (err.position >= 0) {
                printf("%s\n", expression);
                // 打印指向错误位置的箭头
                for (int i = 0; i < err.position; i++) {
                    printf(" ");
                }
                printf("^\n");
            }
            
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
            char historyEntry[MAX_EXPR];
            sprintf(historyEntry, "%s = 未定义", expression);
            addToHistory(history, &historyCount, historyEntry);
        } else if (isInfinite(result)) {
            printf("%s = %s无穷大\n", expression, result > 0 ? "" : "-");
            
            // 添加到历史记录
            char historyEntry[MAX_EXPR];
            sprintf(historyEntry, "%s = %s无穷大", expression, result > 0 ? "" : "-");
            addToHistory(history, &historyCount, historyEntry);
        } else {
            // 使用新的格式化函数
            char resultStr[50];
            formatNumber(result, resultStr, sizeof(resultStr));
            printf("%s = %s\n", expression, resultStr);
            
            // 添加到历史记录
            char historyEntry[MAX_EXPR];
            sprintf(historyEntry, "%s = %s", expression, resultStr);
            addToHistory(history, &historyCount, historyEntry);
        }
    }
    
    return 0;
} 