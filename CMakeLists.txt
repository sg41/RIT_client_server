cmake_minimum_required(VERSION 3.10)
project(RIT_Client_Server
    VERSION 1.0.0
    DESCRIPTION "Simple Client Server communicating tool"
    LANGUAGES CXX)
add_compile_options(
    -Werror
    -Wall
    -Wextra
)
# Указываем директорию с исходными файлами
add_subdirectory(src)
