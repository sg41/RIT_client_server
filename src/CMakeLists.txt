# Определяем две исполняемые цели: клиент и сервер
add_executable(client client.cpp)
add_executable(server server.cpp)

# Указываем выходную директорию для исполняемых файлов
set_target_properties(client server PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

# Указываем заголовочные файлы, если они есть
# include_directories(.) 

# Добавляем библиотеки, если они необходимы
# target_link_libraries(client ...)
# target_link_libraries(server ...)