#include "calculator.h"
#include "test_framework.h"
#include <stdio.h>
#include <string.h>

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

// 打印测试结果
void printTestResult(TestResult* result, TestCase* testCase, double actual) {
    if (result->success) {
        printf("测试通过: %s", testCase->expr);
        if (testCase->expectError) {
            printf(" => 正确捕获错误: %s\n", result->error_msg);
        } else {
            char expectedStr[50], actualStr[50];
            formatNumber(testCase->expected, expectedStr, sizeof(expectedStr));
            formatNumber(actual, actualStr, sizeof(actualStr));
            printf(" = %s (期望: %s)\n", actualStr, expectedStr);
        }
    } else {
        printf("测试失败: %s", testCase->expr);
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