cmake_minimum_required(VERSION 3.10)
project(RIT_Client_Server)
add_compile_options(
    -Werror
    -Wall
    -Wextra
)
# Указываем директорию с исходными файлами
add_subdirectory(src)