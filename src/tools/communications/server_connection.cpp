#include "server_connection.h"

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cstring>
#include <stdexcept>

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

bool ServerConnection::checkNewIncomingConnection() {
  return ::checkFDHaveData(sockfd_, event_timeout_);
}

std::shared_ptr<Connection> ServerConnection::acceptIncomingConnection() {
  sockaddr_in client_addr;
  socklen_t client_addr_size = sizeof(client_addr);
  int client_fd = accept(sockfd_, (sockaddr*)&client_addr, &client_addr_size);
  if (client_fd < 0) {
    throw std::runtime_error("Error accepting new connection");
  }
  return std::make_shared<Connection>(client_fd);
}