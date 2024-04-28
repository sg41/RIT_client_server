cmake_minimum_required(VERSION 3.10)
project(command_parser
    VERSION 1.0.0
    DESCRIPTION "Client Server command parse tools"
    LANGUAGES CXX)
    
add_library(command_parser STATIC command.cpp parser.cpp) 

target_include_directories(command_parser
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)