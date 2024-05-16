/**
 * @file connection.cpp
 * @author Sergey Gorbatenko (sg41@mail.ru)
 * @brief Connection class implementation
 * @version 0.1
 * @date 2024-05-16
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#include "connection.h"

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cstring>
#include <stdexcept>

bool checkFDHaveData(int fd, int timeout) {
  if (fd < 0) {
    throw std::runtime_error("checkFDHaveData: Invalid file descriptor");
  }
  bool have_data = false;
  fd_set readfds;
  FD_ZERO(&readfds);
  FD_SET(fd, &readfds);
  struct timeval tv;
  tv.tv_sec = 0;
  tv.tv_usec = timeout;
  int result = select(fd + 1, &readfds, NULL, NULL, &tv);
  if (result > 0 && FD_ISSET(fd, &readfds)) {
    have_data = true;
  }

  return have_data;
}

Connection::Connection(int fd) : sockfd_(fd) {}

Connection::Connection(const std::string& ip, int port)
    : ip_{ip}, port_{port} {}

Connection::~Connection() { disconnect(); }

void Connection::disconnect() {
  if (sockfd_ >= 0) {
    close(sockfd_);
    sockfd_ = -1;
  }
}

bool Connection::checkHaveEvent() {
  return ::checkFDHaveData(sockfd_, event_timeout_);
}

void Connection::sendMessage(const std::string& message) {
  size_t result = send(sockfd_, message.c_str(), message.length(), 0);
  if (result != message.length()) {
    throw std::runtime_error("sendMessage: Error sending message");
  }
}

std::string Connection::receiveMessage() {
  char buffer[kBufferSize] = {0};
  int bytes_received = recv(sockfd_, buffer, kBufferSize, 0);
  if (bytes_received <= 0) {
    throw std::runtime_error(
        "receiveMessage: Error receiving message or connection closed");
  }
  return std::string(buffer, bytes_received);
}
