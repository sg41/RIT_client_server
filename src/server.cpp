/**
 * @file server.cpp
 * @author Sergey Gorbatenko (sg41@mail.ru)
 * @brief
 * @version 0.1
 * @date 2024-04-14
 *
 * @copyright Copyright (c) 2024
 *
 */
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include <fstream>
#include <iostream>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

std::mutex mtx;

void handle_client(int client_sockfd) {
  char buffer[1024];
  int bytes_received = recv(client_sockfd, buffer, sizeof(buffer), 0);
  if (bytes_received <= 0) {
    return;
  }
  buffer[bytes_received] = '\0';

  std::lock_guard<std::mutex> lock(mtx);
  std::ofstream log_file("log.txt", std::ios::app);
  log_file << buffer << std::endl;
  close(client_sockfd);
}

class Server {
 public:
  Server(int port) : port(port) {}

  void run() {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
      std::cerr << "Error creating socket" << std::endl;
      return;
    }

    sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(sockfd, (sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
      std::cerr << "Error binding socket" << std::endl;
      return;
    }

    if (listen(sockfd, 5) == -1) {
      std::cerr << "Error listening on socket" << std::endl;
      return;
    }

    while (true) {
      sockaddr_in client_addr;
      socklen_t client_addr_size = sizeof(client_addr);
      int client_sockfd =
          accept(sockfd, (sockaddr*)&client_addr, &client_addr_size);
      if (client_sockfd == -1) {
        std::cerr << "Error accepting connection" << std::endl;
        continue;
      }

      std::thread client_thread(handle_client, client_sockfd);
      client_thread.detach();
    }
  }

 private:
  int port;
};

int main(int argc, char** argv) {
  if (argc != 2) {
    std::cerr << "Usage: server <port>" << std::endl;
    return 1;
  }

  int port = std::stoi(argv[1]);

  Server server(port);
  server.run();

  return 0;
}