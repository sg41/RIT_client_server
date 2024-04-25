# Указываем переменные для удобства
BUILD_DIR = build

# Цель для создания директории сборки
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Цель для генерации файлов сборки CMake
cmake-configure: $(BUILD_DIR)
	cd $(BUILD_DIR) && cmake ..

# Цель для сборки проекта
build_all: cmake-configure
	cd $(BUILD_DIR) && make

# Цель для очистки проекта (удаление директории build)
clean:
	rm -rf $(BUILD_DIR)

# Цель по умолчанию - сборка проекта
.DEFAULT_GOAL := build_all