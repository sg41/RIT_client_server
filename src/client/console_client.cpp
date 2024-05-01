/**
 * @file console_client.cpp
 * @author Sergey Gorbatenko (sg41@mail.ru)
 * @brief Simple console client application
 * @version 0.1
 * @date 2024-05-01
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#include <iostream>

#include "client_app.h"

int main(int argc, char** argv) {
  if (argc < 3) {
    std::cerr << "Usage: " << argv[0] << " <server_ip> <server_port>"
              << std::endl;
    return 1;
  }

  std::string server_ip = argv[1];
  int server_port = atoi(argv[2]);

  ClientApp client(server_ip, server_port, true);
  if (!client.connectToServer()) {
    std::cerr << "Failed to connect to server." << std::endl;
    return 1;
  }

  client.run();

  std::cout << "\nBye!" << std::endl;
  return 0;
}