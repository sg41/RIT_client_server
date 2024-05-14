/**
 * @file client.cpp
 * @author Sergey Gorbatenko (sg41@mail.ru)
 * @brief Implementation of Client class
 * @version 0.1
 * @date 2024-04-14
 *
 * @copyright Copyright (c) 2024
 *
 */
#include "client.h"

#include <arpa/inet.h>
#include <netinet/in.h>
#include <poll.h>
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
  bool connected = false;
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

  if (connect(sockfd_, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) >= 0) {
    connected = true;
  }

  if (log_)
    std::cout << "Connected to server: " << (connected ? "success" : "error")
              << std::endl;
  return connected;
}

bool Client::checkHaveMessage(int fd) {
  bool have_data = false;
  fd_set readfds;
  FD_ZERO(&readfds);
  FD_SET(fd, &readfds);
  struct timeval tv;
  tv.tv_sec = 0;
  tv.tv_usec = 0;
  int result = select(fd + 1, &readfds, NULL, NULL, &tv);
  if (result > 0 && FD_ISSET(fd, &readfds)) {
    have_data = true;
  }

  return have_data;
}

bool Client::sendMessage(const std::string& message) {
  int result = send(sockfd_, message.c_str(), message.length(), 0);
  if (result <= 0 && log_) {
    std::cerr << "Error sending message: " << strerror(errno) << std::endl;
  }
  return result > 0;
}

bool Client::receiveMessage(std::string& message) {
  char buffer[kBufferSize] = {0};
  int bytes_received = recv(sockfd_, buffer, kBufferSize, 0);
  if (bytes_received <= 0) {
    if (log_)
      std::cerr << "Error receiving message or connection closed: "
                << strerror(errno) << std::endl;
    message = "";
  } else {
    message = std::string(buffer, bytes_received);
  }
  return bytes_received > 0;
}

bool Client::reconnect() {
  bool connected = false;
  int attempt = 0;
  if (sockfd_ >= 0) close(sockfd_);
  sockfd_ = -1;
  while (attempt++ < kMaxRetries) {
    if (connectToServer()) {
      connected = true;
      break;
    }
    if (log_)
      std::cout << "Attempt to reconnect #" << attempt << " of " << kMaxRetries
                << " in " << kRetryTimeout << " seconds..." << std::endl;
    sleep(kRetryTimeout);
  }

  return connected;
}

Client::~Client() {
  if (sockfd_ >= 0) {
    close(sockfd_);
  }
}
