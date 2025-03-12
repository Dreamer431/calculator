#ifndef TEST_FRAMEWORK_H
#define TEST_FRAMEWORK_H

#include "calculator.h"

// 测试用例结构
typedef struct {
    const char* expr;       // 测试表达式
    double expected;        // 期望结果
    int expectError;        // 是否期望出错
    const char* errorMsg;   // 期望的错误消息
} TestCase;

// 测试结果结构
typedef struct {
    int success;            // 测试是否成功
    double actual_result;   // 实际计算结果
    int err_code;           // 错误代码
    const char* error_msg;  // 错误消息
} TestResult;

// 运行单个测试用例
TestResult runTest(TestCase* testCase, AngleMode mode);

// 打印测试结果
void printTestResult(TestResult* result, TestCase* testCase, double actual);

#endif // TEST_FRAMEWORK_H 