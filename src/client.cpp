/**
 * @file client.cpp
 * @author Sergey Gorbatenko (sg41@mail.ru)
 * @brief
 * @version 0.1
 * @date 2024-04-14
 *
 * @copyright Copyright (c) 2024
 *
 */
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include <chrono>
#include <iostream>
#include <string>
#include <thread>

class Client {
 public:
  Client(const std::string& name, const std::string& server_ip, int port,
         int period)
      : name(name), server_ip(server_ip), port(port), period(period) {}

  void run() {
    while (true) {
      int sockfd = socket(AF_INET, SOCK_STREAM, 0);
      if (sockfd == -1) {
        std::cerr << "Error creating socket" << std::endl;
        return;
      }

      sockaddr_in server_addr;
      server_addr.sin_family = AF_INET;
      server_addr.sin_port = htons(port);
      inet_pton(AF_INET, server_ip.c_str(), &server_addr.sin_addr);

      if (connect(sockfd, (sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        std::cerr << "Error connecting to server" << std::endl;
        close(sockfd);
        return;
      }

      auto now = std::chrono::system_clock::now();
      std::time_t time = std::chrono::system_clock::to_time_t(now);
      std::string time_str(std::ctime(&time));
      time_str.pop_back();
      std::string message = "[" + time_str + "] " + name;
      send(sockfd, message.c_str(), message.size(), 0);

      close(sockfd);
      std::this_thread::sleep_for(std::chrono::seconds(period));
    }
  }

 private:
  std::string name;
  std::string server_ip;
  int port;
  int period;
};

int main(int argc, char** argv) {
  if (argc != 5) {
    std::cerr << "Usage: client <name> <server_ip> <port> <period>"
              << std::endl;
    return 1;
  }

  std::string name(argv[1]);
  std::string server_ip(argv[2]);
  int port = std::stoi(argv[3]);
  int period = std::stoi(argv[4]);

  Client client(name, server_ip, port, period);
  client.run();

  return 0;
}
