#TODO memory leak check
#TODO coverage
# Указываем переменные для удобства
BUILD_DIR = build
CMAKE_FLAGS = 
UNAME:=$(shell uname)
ifeq ($(UNAME), Darwin)
	PYTEST = python3 -m pytest
else
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
	build/server 8080 &
	# sleep 1 second to allow server to start
	sleep 1 
	-build/client_test
	-$(PYTEST)
	echo "shutdown" | build/client 127.0.0.1 8080
	# ps -f | grep 'server 8080' | grep -v grep | awk '{print $$2}' | xargs kill

coverage: debug
	cd $(BUILD_DIR) && make coverage

leaks: debug
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes build/server 8080&
	# sleep 1 second to allow server to start
	sleep 1
	-valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes build/client_test
	echo "shutdown" | build/client 127.0.0.1 8080


# Цель для очистки проекта (удаление директории build)
clean:
	rm -rf $(BUILD_DIR)
	find . -name "__pycache__" -exec rm -rf {} +
	find . -name ".pytest_cache"  -exec rm -rf {} +

# Цель по умолчанию - сборка проекта
.DEFAULT_GOAL := build_all