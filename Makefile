CC = C:/TDM-GCC-64/bin/gcc.exe
CFLAGS = -Wall -Wextra -O2
TARGET = calculator
SRCS = main.c number_utils.c expression_eval.c
OBJS = $(SRCS:.c=.o)
OBJ_DIR = build

# 将对象文件放在 build 目录下
OBJ_FILES = $(addprefix $(OBJ_DIR)/, $(OBJS))

# 默认目标
all: $(TARGET)

# 创建 build 目录
$(OBJ_DIR):
	if not exist $(OBJ_DIR) mkdir $(OBJ_DIR)

# 生成可执行文件
$(TARGET): $(OBJ_FILES)
	$(CC) $(OBJ_FILES) -o $(TARGET)

# 编译源文件到 build 目录
$(OBJ_DIR)/%.o: %.c calculator.h | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# 清理命令（包括旧文件）
clean:
	if exist $(OBJ_DIR) rd /s /q $(OBJ_DIR)
	if exist obj rd /s /q obj
	if exist *.o del *.o
	if exist $(TARGET).exe del $(TARGET).exe

.PHONY: clean all 