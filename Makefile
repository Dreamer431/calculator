CC = gcc
CFLAGS = -Wall -Wextra -O2 -Iinclude -Itest
TARGET = calculator
TEST_TARGET = test_runner

# 源文件
CORE_SRCS = src/core/expression_evaluator.c src/core/operator_handling.c src/core/error_handling.c
UTILS_SRCS = src/utils/number_parser.c src/utils/math_functions.c src/utils/precision_handling.c src/utils/number_formatter.c
MAIN_SRCS = src/core/main.c
TEST_SRCS = test/test_runner.c test/test_framework.c test/test_cases.c

# 所有源文件
SRCS = $(MAIN_SRCS) $(CORE_SRCS) $(UTILS_SRCS)
TEST_ALL_SRCS = $(TEST_SRCS) $(CORE_SRCS) $(UTILS_SRCS)

# 对象文件
OBJS = $(SRCS:.c=.o)
TEST_OBJS = $(TEST_ALL_SRCS:.c=.o)
OBJ_DIR = build

# 将对象文件放在 build 目录下
OBJ_FILES = $(addprefix $(OBJ_DIR)/, $(notdir $(OBJS)))
TEST_OBJ_FILES = $(addprefix $(OBJ_DIR)/, $(notdir $(TEST_OBJS)))

# 设置vpath以查找源文件
vpath %.c src/core src/utils test

# 跨平台命令适配
ifeq ($(OS),Windows_NT)
    RM_DIR = rd /s /q
    RM_FILE = del /f /q
    MKDIR = if not exist $@ mkdir $@
    EXE_EXT = .exe
else
    RM_DIR = rm -rf
    RM_FILE = rm -f
    MKDIR = mkdir -p $@
    EXE_EXT =
endif

# 默认目标
all: $(TARGET)

# 测试目标
test: $(TEST_TARGET)
	./$(TEST_TARGET)$(EXE_EXT)

# 创建 build 目录
$(OBJ_DIR):
	$(MKDIR)

# 生成可执行文件
$(TARGET): $(OBJ_FILES)
	$(CC) $(OBJ_FILES) -o $(TARGET)$(EXE_EXT) -lm

# 生成测试可执行文件
$(TEST_TARGET): $(TEST_OBJ_FILES)
	$(CC) $(TEST_OBJ_FILES) -o $(TEST_TARGET)$(EXE_EXT) -lm

# 编译源文件到 build 目录
$(OBJ_DIR)/%.o: %.c include/calculator.h | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# 清理命令（跨平台兼容）
clean:
ifeq ($(OS),Windows_NT)
	-$(RM_DIR) $(OBJ_DIR) 2>nul
	-$(RM_DIR) obj 2>nul
	-$(RM_FILE) *.o 2>nul
	-$(RM_FILE) $(TARGET)$(EXE_EXT) 2>nul
	-$(RM_FILE) $(TEST_TARGET)$(EXE_EXT) 2>nul
else
	$(RM_DIR) $(OBJ_DIR)
	$(RM_FILE) $(TARGET) $(TEST_TARGET)
endif

.PHONY: clean all test