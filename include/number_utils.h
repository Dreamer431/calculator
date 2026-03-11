#ifndef NUMBER_UTILS_H
#define NUMBER_UTILS_H

#include <float.h>
#include <stdint.h>  // 添加对int64_t的支持
#include "error_handling.h"

// ─── 精度控制常量 ────────────────────────────────────────────────────────────
//
// 精度层级（从严到宽）：
//   ABSOLUTE_ZERO_THRESHOLD (1e-15): 浮点运算中视为绝对零的阈值，
//       用于消除接近零的微小误差（如 sin(180°) 应为0而非1e-16）。
//   RELATIVE_EPSILON (1e-12): 相对误差比较阈值，用于两数之差的
//       相对大小判断（isDoubleEqual），比绝对阈值更宽松。
//   EPSILON (1e-10): 通用精度阈值，用于判断浮点数是否接近整数
//       （isCloseToInteger），容忍累积误差。
//   ANGLE_EPSILON_DEG/RAD: 三角函数特殊角判断阈值，容忍角度/弧度
//       输入时的舍入误差（如89.9999°应按90°处理）。
//
// 显示格式阈值：
//   DISPLAY_FORMAT_THRESHOLD / DISPLAY_FORMAT_MIN: 决定何时切换
//       定点与科学计数法显示。
// ─────────────────────────────────────────────────────────────────────────────

#define ABSOLUTE_ZERO_THRESHOLD 1e-15  // 绝对零阈值：消除浮点微小误差（最严格）
#define RELATIVE_EPSILON        1e-12  // 相对误差阈值：用于两数相等性比较
#define EPSILON                 1e-10  // 通用精度阈值：用于接近整数的判断（最宽松）
#define ANGLE_EPSILON_DEG       0.001  // 角度模式特殊角判断容差（单位：度）
#define ANGLE_EPSILON_RAD       0.0001 // 弧度模式特殊角判断容差（单位：弧度）

#define DISPLAY_FORMAT_THRESHOLD 1e7   // 超过此值切换为科学计数法显示
#define DISPLAY_FORMAT_MIN       1e-4  // 小于此值切换为科学计数法显示
#define PRECISION                10    // 结果显示的小数点后最大位数
#define INFINITY_THRESHOLD       DBL_MAX // 无穷大判断阈值（与 DBL_MAX 对齐）

// 整数处理常量
#define MAX_INTEGER_DIGITS 15        // 整数部分最大位数（防止溢出）
#define LARGE_INTEGER_THRESHOLD 1e15 // 大整数显示阈值（超过使用科学计数法）

// 历史记录大小
#define HISTORY_SIZE 5

// 数值处理函数声明
CalcError getNumberWithError(const char** expr, double* result);
int isInfinite(double value);
int isUndefined(double value);
int isDoubleEqual(double a, double b);
int isCloseToInteger(double value, int64_t* intValue);  // 将long改为int64_t
char* formatNumber(double value, char* buffer, size_t bufferSize);

#endif // NUMBER_UTILS_H
