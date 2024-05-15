cmake_minimum_required(VERSION 3.10)
project(parser
    VERSION 1.0.0
    DESCRIPTION "Client Server command parse tools"
    LANGUAGES CXX)
    
add_library(parser STATIC parser.cpp) 

target_include_directories(parser
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(parser PROPERTIES CXX_STANDARD 17)
