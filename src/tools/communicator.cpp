#include "communicator.h"

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

Communicator::Communicator(SocketType type, std::string ip, int port)
    : type_{type}, ip_{ip}, port_{port} {
  // Constructor implementation
}

Communicator::~Communicator() {
  // Destructor implementation
}

void Communicator::createServerSocket() {
  sockfd_ = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd_ < 0) {
    // TODO: throw error
  }

  int option = 1;
  setsockopt(sockfd_, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));

  sockaddr_in address;
  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons(port_);

  if (bind(sockfd_, reinterpret_cast<const struct sockaddr*>(&address),
           sizeof(address)) < 0) {
    // TODO: throw error
  }

  if (listen(sockfd_, 5) < 0) {
    // TODO: throw error
  }
}

void Communicator::bindSocket(int port) {
  // Implementation
}

void Communicator::listenSocket(int max_connections) {
  // Implementation
}

int Communicator::acceptConnection() {
  // Implementation
}

void Communicator::sendMessage(const std::string& message) {
  // Implementation
}

std::string Communicator::receiveMessage() {
  // Implementation
}

void Communicator::closeSocket() {
  // Implementation
}