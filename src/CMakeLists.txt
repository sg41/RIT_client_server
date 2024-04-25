# Определяем две исполняемые цели: клиент и сервер
add_executable(client client/client_app.cpp)
add_executable(server server/server_app.cpp)

# Указываем выходную директорию для исполняемых файлов
set_target_properties(client server PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

# Указываем заголовочные файлы, если они есть
include_directories(. client server) 

# Добавляем библиотеки, если они необходимы
# target_link_libraries(client ...)
# target_link_libraries(server ...)