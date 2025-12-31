# IP包分析器 Makefile
# 编译命令: make
# 清理命令: make clean

# 编译器
CXX = g++

# 编译选项
CXXFLAGS = -Wall -Wextra -std=c++11 -g

# 链接选项
LDFLAGS = -lpcap

# 目标文件
TARGET = ip_analyzer
SOURCES = ip_analyzer.cpp
OBJECTS = ip_analyzer.o

# 默认目标
all: $(TARGET)

# 编译可执行文件
$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(TARGET) $(LDFLAGS)
	@echo "编译成功！生成可执行文件: $(TARGET)"

# 编译对象文件
ip_analyzer.o: ip_analyzer.cpp
	$(CXX) $(CXXFLAGS) -c ip_analyzer.cpp -o ip_analyzer.o

# 清理生成的文件
clean:
	rm -f $(OBJECTS) $(TARGET)
	@echo "清理完成！"

# 运行程序
run: $(TARGET)
	@echo "运行程序..."
	@sudo ./$(TARGET)

# 安装依赖（Ubuntu/Debian）
install-deps:
	sudo apt-get update
	sudo apt-get install -y libpcap-dev

# 显示帮助信息
help:
	@echo "可用的命令:"
	@echo "  make          - 编译程序"
	@echo "  make clean    - 清理生成的文件"
	@echo "  make run      - 编译并运行程序"
	@echo "  make install-deps - 安装依赖库"
	@echo "  make help     - 显示此帮助信息"

.PHONY: all clean run install-deps help
