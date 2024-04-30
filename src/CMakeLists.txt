cmake_minimum_required(VERSION 3.10)
project(RIT_Client_Server
    VERSION 1.0.0
    DESCRIPTION "Simple Client Server communicating tool"
    LANGUAGES CXX
)

# Определяем две исполняемые цели: клиент и сервер
add_subdirectory(tools)
add_executable(client 
    client/client_app.cpp 
    client/client.cpp
    client/console_client.cpp
    )
add_executable(server 
    server/server_app.cpp 
    server/client_handler.cpp 
    server/server.cpp 
    server/command.cpp
    )

# Указываем выходную директорию для исполняемых файлов
set_target_properties(client server PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

# Указываем стандарт языка
set_target_properties(client server PROPERTIES CXX_STANDARD 17)

# Указываем заголовочные файлы, если они есть
include_directories(${CMAKE_CURRENT_SOURCE_DIR} client server tools) 

target_link_libraries(server PUBLIC parser)
target_link_libraries(client PUBLIC parser)

# Тесты =====================================================================================
find_package(GTest REQUIRED)
find_package(Python REQUIRED)

include_directories(${GTEST_INCLUDE_DIRS})

add_executable(parser_test
    tests/parser_test.cpp
    )
add_executable(client_test
    tests/client_test.cpp
    client/client_app.cpp 
    client/client.cpp
    server/server.cpp
    server/client_handler.cpp
    )

target_link_libraries(client_test ${GTEST_MAIN_LIBRARIES} parser)
target_link_libraries(parser_test ${GTEST_MAIN_LIBRARIES} parser)

# Указываем выходную директорию для исполняемых файлов
set_target_properties(client_test PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set_target_properties(parser_test PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

# Указываем стандарт языка
set_target_properties(parser_test PROPERTIES CXX_STANDARD 17)
set_target_properties(client_test PROPERTIES CXX_STANDARD 17)

