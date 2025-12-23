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

// 测试统计结构
typedef struct {
    int total;              // 总测试数
    int passed;             // 通过数
    int failed;             // 失败数
} TestStats;

// 全局测试统计
extern TestStats globalStats;

// 运行单个测试用例
TestResult runTest(TestCase* testCase, AngleMode mode);

// 打印测试结果并更新统计
void printTestResult(TestResult* result, TestCase* testCase, double actual);

// 重置测试统计
void resetTestStats(void);

// 打印测试摘要
void printTestSummary(void);

#endif // TEST_FRAMEWORK_H