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
#include "client.h"

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include <chrono>
#include <cstring>
#include <iostream>
#include <string>
#include <thread>

Client::Client(const std::string& ip, int port, bool log)
    : server_ip_(ip), server_port_(port), sockfd_(-1), log_(log) {}

bool Client::connectToServer() {
  sockfd_ = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd_ < 0) {
    if (log_)
      std::cerr << "Error creating socket: " << strerror(errno) << std::endl;
    return false;
  }

  sockaddr_in serv_addr;
  memset(&serv_addr, 0, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(server_port_);
  inet_pton(AF_INET, server_ip_.c_str(), &serv_addr.sin_addr);

  int attempt = 0;
  while (connect(sockfd_, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) <
             0 &&
         attempt++ < kMaxRetries) {
    if (log_)
      std::cerr << "Error connecting to server: " << strerror(errno) << "\n"
                << " Retrying #" << attempt << " of " << kMaxRetries << " in "
                << kRetryTimeout << " seconds..." << std::endl;
    sleep(kRetryTimeout);
    continue;
  }
  if (log_) std::cout << "Connected to server!" << std::endl;
  return true;
}

bool Client::sendMessage(const std::string& message) {
  int result = send(sockfd_, message.c_str(), message.length(), 0);
  if (result <= 0 && log_) {
    std::cerr << "Error sending message: " << strerror(errno) << std::endl;
  }
  return result >= 0;
}

bool Client::receiveMessage(std::string& message) {
  char buffer[kBufferSize] = {0};
  int bytes_received = recv(sockfd_, buffer, kBufferSize, 0);
  if (bytes_received < 0) {
    if (log_)
      std::cerr << "Error receiving message or connection closed: "
                << strerror(errno) << std::endl;
    message = "";
  } else {
    message = std::string(buffer, bytes_received);
  }
  return bytes_received >= 0;
}

void Client::reconnect() {
  int attempt = 0;
  if (sockfd_ >= 0) close(sockfd_);
  sockfd_ = -1;
  while (!connectToServer() && attempt++ < kMaxRetries) {
    if (log_)
      std::cout << "Attempt to reconnect #" << attempt << " of " << kMaxRetries
                << " in " << kRetryTimeout << " seconds..." << std::endl;
    sleep(kRetryTimeout);
  }
}

Client::~Client() {
  if (sockfd_ >= 0) {
    close(sockfd_);
  }
}
