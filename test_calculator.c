#include "calculator.h"
#include <stdio.h>
#include <string.h>

// 测试结果结构
typedef struct {
    const char* name;
    int passed;
    const char* message;
    double actual_result;
    const char* actual_error_msg;
} TestResult;

// 测试用例结构
typedef struct {
    const char* expression;
    double expected_result;
    int expect_error;
    const char* expected_error_msg;
} TestCase;

// 用于比较浮点数是否相等
int isDoubleEqual(double a, double b) {
    if (isnan(a) && isnan(b)) return 1;
    if (isinf(a) && isinf(b)) return (a > 0) == (b > 0);
    
    // 对于非常小的数，使用绝对误差
    if (fabs(a) < EPSILON || fabs(b) < EPSILON) {
        return fabs(a - b) < EPSILON;
    }
    
    // 对于其他数，使用相对误差
    double relativeError = fabs((a - b) / b);
    return relativeError < EPSILON;
}

// 运行单个测试用例
TestResult runTest(const TestCase* test, AngleMode mode) {
    TestResult result = {test->expression, 0, NULL, 0, NULL};
    double actual_result;
    CalcError err = evaluateExpression(test->expression, mode, &actual_result);
    
    if (test->expect_error) {
        if (err.code != 0) {
            result.actual_error_msg = err.message;
            if (test->expected_error_msg != NULL && 
                strcmp(err.message, test->expected_error_msg) == 0) {
                result.passed = 1;
            } else {
                result.passed = 0;
                result.message = "错误消息不匹配";
            }
        } else {
            result.passed = 0;
            result.message = "期望出错但计算成功";
        }
    } else {
        if (err.code == 0) {
            if (isDoubleEqual(actual_result, test->expected_result)) {
                result.passed = 1;
            } else {
                result.passed = 0;
                result.message = "结果不匹配";
            }
        } else {
            result.passed = 0;
            result.message = "计算出错";
            result.actual_error_msg = err.message;
        }
    }
    result.actual_result = actual_result;
    return result;
}

// 打印测试结果
void printTestResult(const TestResult* result, const TestCase* test, double actual_result) {
    printf("测试用例: %s\n", result->name);
    if (result->passed) {
        printf("✓ 通过\n");
    } else {
        printf("✗ 失败: %s\n", result->message);
        if (test->expect_error) {
            printf("  期望的错误消息: '%s'\n", test->expected_error_msg ? test->expected_error_msg : "无");
            printf("  实际的错误消息: '%s'\n", result->actual_error_msg ? result->actual_error_msg : "无");
        } else {
            printf("  期望结果: %.*g\n", PRECISION, test->expected_result);
            printf("  实际结果: %.*g\n", PRECISION, actual_result);
        }
    }
    printf("\n");
}

int main() {
    SetConsoleOutputCP(65001);  // UTF-8
    
    // 基本运算测试用例
    TestCase basicTests[] = {
        {"1+1", 2, 0, NULL},
        {"2-1", 1, 0, NULL},
        {"2*3", 6, 0, NULL},
        {"6/2", 3, 0, NULL},
        {"2+2*3", 8, 0, NULL},
        {"(2+2)*3", 12, 0, NULL},
        {"1.5+2.5", 4, 0, NULL},
        {"-1+2", 1, 0, NULL},        // 测试负数
        {"2*-3", -6, 0, NULL},       // 测试负数
        {"0.1+0.2", 0.3, 0, NULL},   // 测试小数精度
        {"1e-10", 1e-10, 0, NULL},   // 测试极小数
        {"2^3", 8, 0, NULL},         // 测试幂运算
        {"2^-2", 0.25, 0, NULL},     // 测试负幂
        {"3^0.5", sqrt(3), 0, NULL}, // 测试小数幂
        {"2^0", 1, 0, NULL},         // 测试0次幂
    };

    // 科学计数法测试用例
    TestCase scientificTests[] = {
        {"1e5", 1e5, 0, NULL},           // 正指数
        {"1e-5", 1e-5, 0, NULL},         // 负指数
        {"1.23e2", 123, 0, NULL},        // 带小数点
        {"1.23e-2", 0.0123, 0, NULL},    // 带小数点和负指数
        {"1E5", 1e5, 0, NULL},           // 大写E
        {"1.23E-2", 0.0123, 0, NULL},    // 大写E带小数点
        {"1e0", 1, 0, NULL},             // 零指数
        {"1.23e+2", 123, 0, NULL},       // 显式正号
        {"1.23e308", 1.23e308, 0, NULL}, // 接近最大值
        {"1.23e-308", 1.23e-308, 0, NULL}, // 接近最小值
    };

    // 错误处理测试用例
    TestCase errorTests[] = {
        {"1/0", 0, 1, "除数不能为0"},
        {"(1+2", 0, 1, "括号不匹配：左括号过多"},
        {"1+2)", 0, 1, "括号不匹配：右括号过多"},
        {"()", 0, 1, "括号内必须有表达式"},
        {"1++2", 0, 1, "运算符使用不正确"},
        {"1+", 0, 1, "表达式不能以运算符结尾"},
        {"1.2.3", 0, 1, "数字格式不正确，多个小数点！"},
        {"1..2", 0, 1, "数字格式不正确，多个小数点！"},
        {"", 0, 1, "表达式不能为空"},           // 测试空表达式
        {"abc", 0, 1, "无效的字符"},           // 测试无效字符
        {"999999999999999999", 0, 1, "数字太大！"},  // 测试数字溢出
        {"1e309", 0, 1, "数字太大！"},         // 测试指数溢出
        {"1e-309", 0, 1, "数字太大！"},        // 测试指数下溢
        {"1ee2", 0, 1, "数字格式不正确，多个指数符号！"}, // 测试多个指数符号
        {"1e2.3", 0, 1, "指数部分必须是整数！"}, // 测试小数指数
        {"(((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((1)))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))", 0, 1, "表达式过于复杂"},  // 测试栈溢出
    };

    // 函数测试用例
    TestCase functionTests[] = {
        {"sin(0)", 0, 0, NULL},
        {"cos(0)", 1, 0, NULL},
        {"tan(0)", 0, 0, NULL},
        {"sqrt(4)", 2, 0, NULL},
        {"sqrt(-1)", 0, 1, "负数不能开平方根！"},
        {"sin(90)", 1, 0, NULL},
        {"cos(90)", 0, 0, NULL},
        {"tan(90)", 0, 1, "tan函数在该点处无定义"},  // 测试tan在90度的情况
        // {"sin(pi)", 0, 0, NULL},                    // 测试pi常量
        // {"cos(pi)", -1, 0, NULL},                   // 测试pi常量
        {"sin(-90)", -1, 0, NULL},                  // 测试负角度
        {"cos(-90)", 0, 0, NULL},                   // 测试负角度
        // {"sin(pi/2)", 1, 0, NULL},                  // 测试pi的运算
    };

    // 弧度模式测试用例
    TestCase radianTests[] = {
        {"sin(pi/2)", 1, 0, NULL},          // 测试pi/2弧度
        {"cos(pi/2)", 0, 0, NULL},          // 测试pi/2弧度
        {"sin(pi)", 0, 0, NULL},            // 测试pi弧度
        {"cos(pi)", -1, 0, NULL},           // 测试pi弧度
        {"sin(pi/6)", 0.5, 0, NULL},        // 测试pi/6弧度
        {"cos(pi/3)", 0.5, 0, NULL},        // 测试pi/3弧度
        {"tan(pi/4)", 1, 0, NULL},          // 测试pi/4弧度
        {"sin(-pi/2)", -1, 0, NULL},        // 测试负弧度
        {"cos(-pi)", -1, 0, NULL},          // 测试负弧度
        {"tan(pi)", 0, 0, NULL},            // 测试pi弧度
    };

    // 复杂表达式测试用例
    TestCase complexTests[] = {
        {"1+2*(3+4)", 15, 0, NULL},
        {"2(3+4)", 14, 0, NULL},
        {"sin(30)*2", 1, 0, NULL},
        {"cos(60)/2", 0.25, 0, NULL},
        {"sin(cos(0))", 0.01745240644, 0, NULL},        // 测试函数嵌套
        {"(1+2)*(3+4)", 21, 0, NULL},             // 测试多重括号
        {"2*pi", 2*PI, 0, NULL},                  // 测试常量运算
        {"sin(45)*cos(45)", 0.5, 0, NULL},        // 测试多重函数
        {"sqrt(sin(90)^2 + cos(90)^2)", 1, 0, NULL},  // 测试复杂表达式
        {"2^3^2", 512, 0, NULL},                  // 测试幂运算结合性
        {"1e-10*1e10", 1, 0, NULL},               // 测试科学计数法运算
        {"sqrt(3^2 + 4^2)", 5, 0, NULL},          // 测试勾股定理
        {"(1+2+3+4+5)*2/3", 10, 0, NULL},         // 测试长表达式
    };

    // 边界值测试用例
    TestCase boundaryTests[] = {
        {"0.000000001", 1e-9, 0, NULL},           // 测试小数精度
        {"9999999999", 1e10, 0, NULL},            // 测试大数
        {"1/1000000000", 1e-9, 0, NULL},          // 测试除法精度
        {"0.1^10", 1e-10, 0, NULL},               // 测试幂运算精度
        {"sin(89.999999)", 1, 0, NULL},           // 测试接近90度
        {"cos(89.999999)", 0, 0, NULL},           // 测试接近90度
        {"pi*1e-10", PI*1e-10, 0, NULL},          // 测试常量与小数相乘
        {"(1+1e-10)^2", 1 + 2e-10, 0, NULL},      // 测试精确计算
    };

    printf("开始运行测试...\n\n");

    // 运行基本运算测试
    printf("=== 基本运算测试 ===\n");
    for (size_t i = 0; i < sizeof(basicTests)/sizeof(basicTests[0]); i++) {
        TestResult result = runTest(&basicTests[i], MODE_DEG);
        printTestResult(&result, &basicTests[i], result.actual_result);
    }

    // 运行科学计数法测试
    printf("=== 科学计数法测试 ===\n");
    for (size_t i = 0; i < sizeof(scientificTests)/sizeof(scientificTests[0]); i++) {
        TestResult result = runTest(&scientificTests[i], MODE_DEG);
        printTestResult(&result, &scientificTests[i], result.actual_result);
    }

    // 运行错误处理测试
    printf("=== 错误处理测试 ===\n");
    for (size_t i = 0; i < sizeof(errorTests)/sizeof(errorTests[0]); i++) {
        TestResult result = runTest(&errorTests[i], MODE_DEG);
        printTestResult(&result, &errorTests[i], result.actual_result);
    }

    // 运行函数测试（角度模式）
    printf("=== 函数测试（角度模式）===\n");
    for (size_t i = 0; i < sizeof(functionTests)/sizeof(functionTests[0]); i++) {
        TestResult result = runTest(&functionTests[i], MODE_DEG);
        printTestResult(&result, &functionTests[i], result.actual_result);
    }

    // 运行弧度模式测试
    printf("=== 函数测试（弧度模式）===\n");
    for (size_t i = 0; i < sizeof(radianTests)/sizeof(radianTests[0]); i++) {
        TestResult result = runTest(&radianTests[i], MODE_RAD);
        printTestResult(&result, &radianTests[i], result.actual_result);
    }

    // 运行复杂表达式测试
    printf("=== 复杂表达式测试 ===\n");
    for (size_t i = 0; i < sizeof(complexTests)/sizeof(complexTests[0]); i++) {
        TestResult result = runTest(&complexTests[i], MODE_DEG);
        printTestResult(&result, &complexTests[i], result.actual_result);
    }

    // 运行边界值测试
    printf("=== 边界值测试 ===\n");
    for (size_t i = 0; i < sizeof(boundaryTests)/sizeof(boundaryTests[0]); i++) {
        TestResult result = runTest(&boundaryTests[i], MODE_DEG);
        printTestResult(&result, &boundaryTests[i], result.actual_result);
    }

    return 0;
} 