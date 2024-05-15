#include "communicator.h"

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cstring>
#include <stdexcept>

bool checkFDHaveData(int fd, int timeout) {
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

Connection::Connection(std::string ip, int port) : ip_{ip}, port_{port} {}

Connection::~Connection() {
  if (sockfd_ >= 0) {
    close(sockfd_);
  }
}

void ServerConnection::establishConnection() {
  sockfd_ = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd_ < 0) {
    throw std::runtime_error("Error creating socket");
  }

  int option = 1;
  setsockopt(sockfd_, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));

  sockaddr_in address;
  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons(port_);

  if (bind(sockfd_, reinterpret_cast<const struct sockaddr*>(&address),
           sizeof(address)) < 0) {
    throw std::runtime_error("Error binding socket");
  }

  if (listen(sockfd_, 5) < 0) {
    throw std::runtime_error("Error listening for incoming connections");
  }
}

void ClientConnection::establishConnection() {
  sockfd_ = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd_ < 0) {
    throw std::runtime_error("Error creating socket");
  }

  sockaddr_in serv_addr;
  memset(&serv_addr, 0, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(port_);
  inet_pton(AF_INET, ip_.c_str(), &serv_addr.sin_addr);

  if (connect(sockfd_, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
    throw std::runtime_error("Error connecting to server");
  }
}

bool Connection::checkHaveEvent() {
  return ::checkFDHaveData(sockfd_, event_timeout_);
}

void Connection::sendMessage(const std::string& message) {
  size_t result = send(sockfd_, message.c_str(), message.length(), 0);
  if (result != message.length()) {
    throw std::runtime_error("Error sending message");
  }
}

std::string Connection::receiveMessage() {
  char buffer[kBufferSize] = {0};
  int bytes_received = recv(sockfd_, buffer, kBufferSize, 0);
  if (bytes_received <= 0) {
    throw std::runtime_error("Error receiving message or connection closed");
  }
  return std::string(buffer, bytes_received);
}

void ClientConnection::sendMessage(const std::string& message) {
  try {
    Connection::sendMessage(message);
  } catch (std::runtime_error& e) {
    if (retry_on_error_ && !reconnect()) {
      throw std::runtime_error("SendMessage: Error reconnecting to server");
    }
  }
}

std::string ClientConnection::receiveMessage() {
  try {
    return Connection::receiveMessage();
  } catch (std::runtime_error& e) {
    if (retry_on_error_ && !reconnect()) {
      throw std::runtime_error("ReceiveMessage: Error reconnecting to server");
    }
  }
  return "";
}
bool ClientConnection::reconnect() {
  bool connected = false;
  int attempt = 0;
  if (sockfd_ >= 0) close(sockfd_);
  sockfd_ = -1;
  while (attempt++ < max_retries_) {
    try {
      establishConnection();
      connected = true;
      break;
    } catch (std::runtime_error& e) {
    }

    sleep(retry_timeout_ / 1000);
  }

  return connected;
}
ClientConnection::ClientConnection(std::string ip, int port, int retry_timeout,
                                   int max_retries, bool retry_on_error)
    : Connection(ip, port),
      retry_timeout_(retry_timeout),
      max_retries_(max_retries),
      retry_on_error_(retry_on_error) {}