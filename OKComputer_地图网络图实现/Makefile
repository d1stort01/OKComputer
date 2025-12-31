# Makefile - 地图网络图分析系统编译脚本

# 编译器设置
CXX = g++
CXXFLAGS = -std=c++14 -Wall -Wextra -O2

# 源文件
SOURCES = main.cpp City.cpp Graph.cpp UserManager.cpp FileManager.cpp MapNetwork.cpp MenuSystem.cpp
TEST_SOURCES = TestProgram.cpp City.cpp Graph.cpp UserManager.cpp FileManager.cpp MapNetwork.cpp

# 目标文件
OBJECTS = $(SOURCES:.cpp=.o)
TEST_OBJECTS = $(TEST_SOURCES:.cpp=.o)

# 可执行文件名
TARGET = MapNetworkSystem
TEST_TARGET = TestProgram

# 默认目标
all: $(TARGET)

# 编译主程序
$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJECTS)
	@echo "编译完成: $(TARGET)"

# 编译测试程序
$(TEST_TARGET): $(TEST_OBJECTS)
	$(CXX) $(CXXFLAGS) -o $(TEST_TARGET) $(TEST_OBJECTS)
	@echo "编译完成: $(TEST_TARGET)"

# 编译对象文件
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# 清理生成的文件
clean:
	rm -f *.o
	rm -f $(TARGET)
	rm -f $(TEST_TARGET)
	rm -f *.txt
	@echo "清理完成"

# 运行主程序
run: $(TARGET)
	./$(TARGET)

# 运行测试程序
test: $(TEST_TARGET)
	./$(TEST_TARGET)

# 创建备份
backup:
	@mkdir -p backup
	@cp *.h backup/
	@cp *.cpp backup/
	@cp Makefile backup/
	@echo "备份完成到 backup/ 目录"

# 从备份恢复
restore:
	@cp backup/*.h .
	@cp backup/*.cpp .
	@cp backup/Makefile .
	@echo "从备份恢复完成"

# 帮助
help:
	@echo "可用的目标:"
	@echo "  all      - 编译主程序（默认）"
	@echo "  test     - 编译并运行测试程序"
	@echo "  run      - 运行主程序"
	@echo "  clean    - 清理生成的文件"
	@echo "  backup   - 备份源文件"
	@echo "  restore  - 从备份恢复"
	@echo "  help     - 显示此帮助信息"

# 声明伪目标
.PHONY: all test run clean backup restore help
