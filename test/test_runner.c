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
extern TestCase implicitMultiplyTests[];
extern TestCase powerTests[];
extern TestCase unitConversionTests[];
extern TestCase whitespaceTests[];

// 辅助函数：运行测试数组直到遇到 NULL 终止符
static int runTestSuite(const char* suiteName, TestCase tests[], AngleMode mode) {
    int suiteTotal = 0, suitePassed = 0;
    printf("\n=== %s ===\n", suiteName);
    
    for (size_t i = 0; tests[i].expr != NULL; i++) {
        TestResult result = runTest(&tests[i], mode);
        printTestResult(&result, &tests[i], result.actual_result);
        suiteTotal++;
        if (result.success) suitePassed++;
    }
    
    printf("  --- 小计: %d/%d 通过 ---\n", suitePassed, suiteTotal);
    return suiteTotal - suitePassed;  // 返回失败数
}

int main() {
    // 设置控制台代码页（仅Windows）
#ifdef _WIN32
    SetConsoleOutputCP(65001);  // UTF-8
    SetConsoleCP(65001);       // UTF-8
#endif
    
    printf("========================================\n");
    printf("       Calculator 单元测试\n");
    printf("========================================\n");
    
    // 重置统计
    resetTestStats();
    
    // 运行所有测试套件
    runTestSuite("基本运算测试", basicTests, MODE_DEG);
    runTestSuite("幂运算与结合性测试", powerTests, MODE_DEG);
    runTestSuite("隐式乘法测试", implicitMultiplyTests, MODE_DEG);
    runTestSuite("科学计数法测试", scientificTests, MODE_DEG);
    runTestSuite("错误处理测试", errorTests, MODE_DEG);
    runTestSuite("函数测试（角度模式）", functionTests, MODE_DEG);
    runTestSuite("函数测试（弧度模式）", radianTests, MODE_RAD);
    runTestSuite("单位转换函数测试", unitConversionTests, MODE_DEG);
    runTestSuite("复杂表达式测试", complexTests, MODE_DEG);
    runTestSuite("边界值测试", boundaryTests, MODE_DEG);
    runTestSuite("常量测试", constantTests, MODE_DEG);
    runTestSuite("空格处理测试", whitespaceTests, MODE_DEG);
    
    // 打印测试摘要
    printTestSummary();
    
    // 返回失败数作为退出码
    return globalStats.failed;
}