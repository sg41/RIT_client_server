find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

add_executable(client_test client_test.cpp client.cpp client_app.cpp ${GTEST_LIBRARIES})
target_link_libraries(client_test ${GTEST_LIBRARIES})

add_executable(server_test server_test.cpp server.cpp client_handler.cpp ${GTEST_LIBRARIES})
target_link_libraries(server_test ${GTEST_LIBRARIES} parser)