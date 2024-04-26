# Определяем две исполняемые цели: клиент и сервер
add_executable(client client/client_app.cpp client/client.cpp)
add_executable(server server/server_app.cpp server/client_handler.cpp server/server.cpp server/parser.cpp)

# Указываем выходную директорию для исполняемых файлов
set_target_properties(client server PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

# Указываем стандарт языка
set_target_properties(client server PROPERTIES CXX_STANDARD 17)

# Указываем заголовочные файлы, если они есть
include_directories(. client server) 
