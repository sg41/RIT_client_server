cmake_minimum_required(VERSION 3.10)
project(communications
    VERSION 1.0.0
    DESCRIPTION "Client Server socket communication tools"
    LANGUAGES CXX)
    
add_library(communications STATIC 
    client_connection.cpp
    server_connection.cpp
    connection.cpp
) 

target_include_directories(communications
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(communications PROPERTIES CXX_STANDARD 17)
