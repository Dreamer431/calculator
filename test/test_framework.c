#include "calculator.h"
#include "test_framework.h"
#include <stdio.h>
#include <string.h>

// 全局测试统计
TestStats globalStats = {0, 0, 0};

// 重置测试统计
void resetTestStats(void) {
    globalStats.total = 0;
    globalStats.passed = 0;
    globalStats.failed = 0;
}

// 运行单个测试用例
TestResult runTest(TestCase* testCase, AngleMode mode) {
    TestResult result;
    result.success = 0;
    
    double actualResult;
    CalcError err = evaluateExpression(testCase->expr, mode, &actualResult);
    
    result.actual_result = actualResult;
    result.err_code = err.code;
    result.error_msg = err.message;
    
    if (testCase->expectError) {
        // 期望出现错误
        if (err.code != 0) {
            // 如果设置了期望的错误消息，检查错误消息是否匹配
            if (testCase->errorMsg && strcmp(testCase->errorMsg, err.message) != 0) {
                result.success = 0;
            } else {
                result.success = 1;
            }
        } else {
            result.success = 0;
        }
    } else {
        // 期望计算成功
        if (err.code == 0) {
            if (isDoubleEqual(actualResult, testCase->expected)) {
                result.success = 1;
            }
        }
    }
    
    return result;
}

// 打印测试结果并更新统计
void printTestResult(TestResult* result, TestCase* testCase, double actual) {
    globalStats.total++;
    
    if (result->success) {
        globalStats.passed++;
        printf("  [PASS] %s", testCase->expr);
        if (testCase->expectError) {
            printf(" => 正确捕获错误: %s\n", result->error_msg);
        } else {
            char expectedStr[50], actualStr[50];
            formatNumber(testCase->expected, expectedStr, sizeof(expectedStr));
            formatNumber(actual, actualStr, sizeof(actualStr));
            printf(" = %s (期望: %s)\n", actualStr, expectedStr);
        }
    } else {
        globalStats.failed++;
        printf("  [FAIL] %s", testCase->expr);
        if (testCase->expectError) {
            if (result->err_code == 0) {
                printf(" => 期望出现错误，但计算成功: %g\n", actual);
            } else {
                printf(" => 错误消息不符合预期，得到: %s, 期望: %s\n", result->error_msg, testCase->errorMsg);
            }
        } else {
            if (result->err_code != 0) {
                printf(" => 计算失败: %s\n", result->error_msg);
            } else {
                char expectedStr[50], actualStr[50];
                formatNumber(testCase->expected, expectedStr, sizeof(expectedStr));
                formatNumber(actual, actualStr, sizeof(actualStr));
                printf(" => 结果不符合预期, 得到: %s, 期望: %s\n", actualStr, expectedStr);
            }
        }
    }
}

// 打印测试摘要
void printTestSummary(void) {
    printf("\n");
    printf("========================================\n");
    printf("            测试摘要\n");
    printf("========================================\n");
    printf("  总测试数:   %d\n", globalStats.total);
    printf("  通过:       %d\n", globalStats.passed);
    printf("  失败:       %d\n", globalStats.failed);
    printf("  通过率:     %.1f%%\n", globalStats.total > 0 ? 
           (100.0 * globalStats.passed / globalStats.total) : 0.0);
    printf("========================================\n");
    
    if (globalStats.failed == 0) {
        printf("  ✓ 所有测试通过！\n");
    } else {
        printf("  ✗ 有 %d 个测试失败\n", globalStats.failed);
    }
    printf("========================================\n");
}