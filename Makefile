#TODO coverage
# Указываем переменные для удобства
BUILD_DIR = build
DOCS_DIR = docs
CMAKE_FLAGS = 
UNAME:=$(shell uname)
SOURCES = $(shell find . -name "*.cpp") $(shell find . -name "*.h")
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
	ps -f | grep 'server 12345' | grep -v grep | awk '{print $$2}' | xargs kill

# Цель для запуска тестов
test: CMAKE_FLAGS += -DCMAKE_CXX_CPPCHECK="cppcheck;--enable=all;--suppress=missingIncludeSystem;--suppress=unusedFunction;"
test: debug
	build/parser_test
	build/server 8080 silent&
	# sleep 1 second to allow server to start
	sleep 1 
	-build/client_test
	-$(PYTEST)
	@ps -f | grep 'server 8080' | grep -v grep | awk '{print $$2}' | xargs kill

leaks: debug
	$(LEAKS) build/server 8080 silent&
	# sleep 1 second to allow server to start
	sleep 1
	-$(LEAKS) build/client_test
	echo "shutdown" | build/client 127.0.0.1 8080 > /dev/null 2>&1

coverage: debug
	build/server 8080 silent&
	cd $(BUILD_DIR) && make coverage
	@ps -f | grep 'server 8080' | grep -v grep | awk '{print $$2}' | xargs kill
	open build/src/coverage/index.html

docs: doxygen.conf $(SOURCES) README.md
	doxygen doxygen.conf

# Цель для очистки проекта (удаление директории build)
clean:
	rm -rf $(BUILD_DIR)
	find . -name "__pycache__" -exec rm -rf {} +
	find . -name ".pytest_cache"  -exec rm -rf {} +
	rm -rf $(DOCS_DIR)

# Цель по умолчанию - сборка проекта
.DEFAULT_GOAL := build_all