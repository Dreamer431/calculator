#include "calculator.h"

// 智能格式化数值
char* formatNumber(double value, char* buffer, size_t bufferSize) {
    // 处理特殊情况
    if (isnan(value)) {
        snprintf(buffer, bufferSize, "未定义");
        return buffer;
    }
    if (isinf(value)) {
        snprintf(buffer, bufferSize, "%s无穷大", value > 0 ? "" : "-");
        return buffer;
    }
    
    // 处理特大整数
    if (value >= 1e10 && fmod(value, 1.0) == 0) {
        snprintf(buffer, bufferSize, "%.0e", value);
        return buffer;
    }
    
    // 检查是否接近整数
    long intValue;
    if (isCloseToInteger(value, &intValue)) {
        // 如果是整数，直接显示整数
        snprintf(buffer, bufferSize, "%ld", intValue);
        return buffer;
    }
    
    // 根据数值大小选择合适的显示格式
    if (fabs(value) >= DISPLAY_FORMAT_THRESHOLD || (fabs(value) > 0 && fabs(value) < DISPLAY_FORMAT_MIN)) {
        // 使用科学计数法
        snprintf(buffer, bufferSize, "%.6e", value);
    } else {
        // 使用普通小数格式
        snprintf(buffer, bufferSize, "%.10f", value);
        
        // 移除末尾多余的零
        char* p = buffer + strlen(buffer) - 1;
        while (p > buffer && *p == '0') {
            *p-- = '\0';
        }
        
        // 如果小数点后面没有数字，移除小数点
        if (*p == '.') {
            *p = '\0';
        }
    }
    
    return buffer;
} 