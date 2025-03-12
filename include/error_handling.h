#ifndef ERROR_HANDLING_H
#define ERROR_HANDLING_H

// 错误代码枚举
typedef enum {
    ERR_SUCCESS = 0,          // 成功
    ERR_SYNTAX = 1,          // 语法错误
    ERR_DIV_BY_ZERO = 2,     // 除以零
    ERR_INVALID_NUMBER = 3,   // 无效数字
    ERR_OVERFLOW = 4,         // 数值溢出
    ERR_UNDEFINED = 5,        // 未定义结果
    ERR_INVALID_FUNCTION = 6, // 无效函数
    ERR_INVALID_ARGUMENT = 7, // 无效参数
    ERR_STACK_OVERFLOW = 8,   // 栈溢出
    ERR_MISSING_PARENTHESIS = 9, // 括号不匹配
    ERR_EMPTY_EXPRESSION = 10 // 空表达式
} ErrorCode;

// 错误处理结构
typedef struct {
    int code;  // 保持为int以兼容现有代码
    const char* message;
    int position; // 添加错误位置信息，表示错误发生在表达式中的位置
} CalcError;

// 错误处理宏 - 修改以包含位置信息
#define CALC_SUCCESS ((CalcError){0, NULL, -1})
#define CALC_ERROR(msg) ((CalcError){1, msg, -1})

// 新增：带错误代码和位置的错误处理宏
#define CALC_ERROR_CODE_POS(code, msg, pos) ((CalcError){code, msg, pos})
#define CALC_ERROR_POS(msg, pos) ((CalcError){1, msg, pos})
#define CALC_ERROR_CODE(code, msg) ((CalcError){code, msg, -1})

// 错误处理函数声明
const char* getErrorDescription(int errorCode);

#endif // ERROR_HANDLING_H 