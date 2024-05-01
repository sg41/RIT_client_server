#TODO coverage
# Указываем переменные для удобства
BUILD_DIR = build
DOCS_DIR = docs
CMAKE_FLAGS = 
UNAME:=$(shell uname)
SOURCES = $(shell find . -name "*.cpp") $(shell find . -name "*.h")
SERVER_MODE = silent
ifeq ($(UNAME), Darwin)
	PYTEST = python3 -m pytest
	LEAKS = leaks --atExit --
else
	LEAKS = valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes
	PYTEST = pytest
endif

# Цель для создания директории сборки
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Цель для генерации файлов сборки CMake
cmake-configure: $(BUILD_DIR)
	cd $(BUILD_DIR) && cmake .. $(CMAKE_FLAGS)

# Цель для сборки проекта
build_all: cmake-configure
	cd $(BUILD_DIR) && make

# Цель для сборки проекта с отладочным режимом
debug: CMAKE_FLAGS += -DCMAKE_BUILD_TYPE=Debug
debug: cmake-configure
	cd $(BUILD_DIR) && make

# Цель для сборки проекта с отладочным режимом и запуска
debug_run: debug
	build/server 12345&
	build/client 127.0.0.1 12345
	sleep 1
	@ps -f | grep 'server 12345' | grep -v grep | awk '{print $$2}' | xargs kill

# Цели для запуска тестов
all_tests: test leaks coverage

tests: test

test: CMAKE_FLAGS += -DCMAKE_CXX_CPPCHECK="cppcheck;--enable=all;--suppress=missingIncludeSystem;--suppress=unusedFunction;"
test: debug
	$(BUILD_DIR)/parser_test
	-$(BUILD_DIR)/client  > /dev/null 2>&1
	-$(BUILD_DIR)/client 127.0.0.1 8080 > /dev/null 2>&1
	-$(BUILD_DIR)/server 8080 wrong_parameter
	$(BUILD_DIR)/server 8080 $(SERVER_MODE)&
	# sleep 1 second to allow server to start
	sleep 1 
	-$(BUILD_DIR)/client_test
	-$(PYTEST)
	# gracefull shutdown to collect coverage info
	echo "shutdown" | $(BUILD_DIR)/client 127.0.0.1 8080 > /dev/null 2>&1

leaks: debug
	$(LEAKS) $(BUILD_DIR)/server 8080 silent&
	# sleep 1 second to allow server to start
	sleep 1
	-$(LEAKS) $(BUILD_DIR)/client_test
	echo "shutdown" | $(BUILD_DIR)/client 127.0.0.1 8080 > /dev/null 2>&1

coverage: CMAKE_FLAGS += -DCOVERAGE=ON
coverage: test
	rm -rf $(BUILD_DIR)/report
	mkdir $(BUILD_DIR)/report
ifeq ($(UNAME), Darwin)
	gcovr -r . --html --html-details -o $(BUILD_DIR)/report/index.html
else
	lcov -t "gcov_report" -o $(BUILD_DIR)/gcov_report.info -c --d .
	lcov -r gcov_report.info '/usr/include/*' -o $(BUILD_DIR)/filtered.info
	genhtml -o $(BUILD_DIR)/report $(BUILD_DIR)/filtered.info --ignore-errors inconsistent ...
endif
	@find . -name "*.gcda" -exec rm -rf {} +

show_coverage: coverage
	open $(BUILD_DIR)/report/index.html

# Цель для сборки документации
docs: doxygen.conf $(SOURCES) README.md
	doxygen doxygen.conf

show_docs: docs
	open $(DOCS_DIR)/html/index.html

# Цель для очистки проекта от сборки
clean:
	rm -rf $(BUILD_DIR)
	find . -name "__pycache__" -exec rm -rf {} +
	find . -name ".pytest_cache"  -exec rm -rf {} +
	rm -rf $(DOCS_DIR)

# Цель по умолчанию - сборка проекта
.DEFAULT_GOAL := build_all
