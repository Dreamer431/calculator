#include "calculator.h"
#include "test_framework.h"
#include <stdio.h>

// 声明在test_cases.c中定义的测试用例数组
extern TestCase basicTests[];
extern TestCase scientificTests[];
extern TestCase errorTests[];
extern TestCase functionTests[];
extern TestCase radianTests[];
extern TestCase complexTests[];
extern TestCase boundaryTests[];
extern TestCase constantTests[];

// 获取数组大小的宏
#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))

int main() {
    // 设置控制台代码页
    SetConsoleOutputCP(65001);  // UTF-8
    SetConsoleCP(65001);       // UTF-8
    
    printf("开始运行测试...\n\n");

    // 运行基本运算测试
    printf("=== 基本运算测试 ===\n");
    for (size_t i = 0; i < 15; i++) {
        TestResult result = runTest(&basicTests[i], MODE_DEG);
        printTestResult(&result, &basicTests[i], result.actual_result);
    }

    // 运行科学计数法测试
    printf("\n=== 科学计数法测试 ===\n");
    for (size_t i = 0; i < 10; i++) {
        TestResult result = runTest(&scientificTests[i], MODE_DEG);
        printTestResult(&result, &scientificTests[i], result.actual_result);
    }

    // 运行错误处理测试
    printf("\n=== 错误处理测试 ===\n");
    for (size_t i = 0; i < 16; i++) {
        TestResult result = runTest(&errorTests[i], MODE_DEG);
        printTestResult(&result, &errorTests[i], result.actual_result);
    }

    // 运行函数测试（角度模式）
    printf("\n=== 函数测试（角度模式）===\n");
    for (size_t i = 0; i < 38; i++) {
        TestResult result = runTest(&functionTests[i], MODE_DEG);
        printTestResult(&result, &functionTests[i], result.actual_result);
    }

    // 运行弧度模式测试
    printf("\n=== 函数测试（弧度模式）===\n");
    for (size_t i = 0; i < 10; i++) {
        TestResult result = runTest(&radianTests[i], MODE_RAD);
        printTestResult(&result, &radianTests[i], result.actual_result);
    }

    // 运行复杂表达式测试
    printf("\n=== 复杂表达式测试 ===\n");
    for (size_t i = 0; i < 13; i++) {
        TestResult result = runTest(&complexTests[i], MODE_DEG);
        printTestResult(&result, &complexTests[i], result.actual_result);
    }

    // 运行边界值测试
    printf("\n=== 边界值测试 ===\n");
    for (size_t i = 0; i < 8; i++) {
        TestResult result = runTest(&boundaryTests[i], MODE_DEG);
        printTestResult(&result, &boundaryTests[i], result.actual_result);
    }

    // 运行新常量测试
    printf("\n=== 常量测试 ===\n");
    for (size_t i = 0; i < 6; i++) {
        TestResult result = runTest(&constantTests[i], MODE_DEG);
        printTestResult(&result, &constantTests[i], result.actual_result);
    }

    return 0;
}