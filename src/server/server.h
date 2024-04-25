#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include <chrono>
#include <cstring>
#include <iostream>
#include <string>
#include <thread>

#include "client_handler.h"

class Server {
 private:
  int server_socket_ = -1;
  int port_;
  bool log_ = false;

 public:
  Server(int port, bool log = false)
      : port_(port), server_socket_(-1), log_(log) {}

  bool startServer() {
    server_socket_ = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket_ < 0) {
      if (log_)
        std::cerr << "Error creating socket: " << strerror(errno) << std::endl;
      return false;
    }

    int option = 1;
    setsockopt(server_socket_, SOL_SOCKET, SO_REUSEADDR, &option,
               sizeof(option));

    sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port_);

    if (bind(server_socket_, (struct sockaddr*)&address, sizeof(address)) < 0) {
      if (log_)
        std::cerr << "Error binding socket: " << strerror(errno) << std::endl;
      return false;
    }

    if (listen(server_socket_, 5) < 0) {
      if (log_)
        std::cerr << "Error listening on socket: " << strerror(errno)
                  << std::endl;
      return false;
    }

    if (log_) std::cout << "Server listening on port " << port_ << std::endl;
    return true;
  }

  void acceptConnections() {
    while (true) {
      sockaddr_in client_address;
      socklen_t client_addr_len = sizeof(client_address);
      int client_socket = accept(
          server_socket_, (struct sockaddr*)&client_address, &client_addr_len);
      if (client_socket < 0) {
        if (log_)
          std::cerr << "Error accepting connection: " << strerror(errno)
                    << std::endl;
        continue;
      }

      if (log_) std::cout << "New client connected!" << std::endl;
      ClientHandler client_handler(client_socket);
      std::thread client_thread(&ClientHandler::handleClient, &client_handler);
      client_thread.detach();
    }
  }

  ~Server() {
    if (server_socket_ >= 0) {
      close(server_socket_);
    }
  }
};