/**
 * @file server_app.cpp
 * @author Sergey Gorbatenko (sg41@mail.ru)
 * @brief Simple server application using Server class
 * @version 0.1
 * @date 2024-05-01
 *
 * @copyright Copyright (c) 2024
 *
 */
#include <iostream>

#include "server.h"

int main(int argc, char** argv) {
  int port = 8080;  // Default port
  bool log = true;

  if (argc > 1) {
    port = atoi(argv[1]);
  }

  if (argc > 2) {
    if (std::string(argv[2]) == "silent") {
      log = false;
    } else {
      std::cout << "Usage: " << argv[0] << " <port> [silent]" << std::endl;
      return 1;
    }
  }

  Server server(port, log);
  if (server.startServer()) {
    server.acceptConnections();
  }

  return 0;
  // exit(0);
}