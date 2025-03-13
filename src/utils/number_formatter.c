#include "calculator.h"
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdint.h>  // 添加对int64_t的支持

/**
 * 统一格式化数值
 * 根据数值大小和特性自动选择合适的格式化策略
 * 
 * @param value 要格式化的数值
 * @param buffer 输出缓冲区
 * @param bufferSize 缓冲区大小
 * @return 格式化后的字符串（指向buffer的指针）
 */
char* formatNumber(double value, char* buffer, size_t bufferSize) {
    // 处理特殊情况：NaN
    if (isnan(value)) {
        snprintf(buffer, bufferSize, "未定义");
        return buffer;
    }
    
    // 处理特殊情况：Infinity
    if (isinf(value)) {
        snprintf(buffer, bufferSize, "%s无穷大", value > 0 ? "" : "-");
        return buffer;
    }
    
    // 检查是否接近整数
    int64_t intValue;
    if (isCloseToInteger(value, &intValue)) {
        // 处理大整数：超过10^15使用科学计数法
        if (fabs(value) >= 1e15) {
            snprintf(buffer, bufferSize, "%.1e", value);
            return buffer;
        }
        
        // 普通整数：直接显示
        snprintf(buffer, bufferSize, "%lld", (long long)intValue);
        return buffer;
    }
    
    // 处理非整数
    double absValue = fabs(value);
    
    // 根据数值范围选择格式
    if (absValue >= DISPLAY_FORMAT_THRESHOLD || (absValue > 0 && absValue < DISPLAY_FORMAT_MIN)) {
        // 超出范围：使用科学计数法，保留6位有效数字
        snprintf(buffer, bufferSize, "%.6e", value);
        return buffer;
    }
    
    // 标准范围：使用定点表示法，但要移除尾部多余的零
    snprintf(buffer, bufferSize, "%.*f", PRECISION, value);
    
    // 清理尾部的零和可能的小数点
    char* decimal = strchr(buffer, '.');
    if (decimal) {
        char* end = buffer + strlen(buffer) - 1;
        
        // 从末尾开始，移除多余的零
        while (end > decimal && *end == '0') {
            *end-- = '\0';
        }
        
        // 如果小数点成为末尾字符，也移除它
        if (end == decimal) {
            *end = '\0';
        }
    }
    
    return buffer;
} 