#include "client_connection.h"

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include <chrono>
#include <cstring>
#include <stdexcept>
#include <thread>

void ClientConnection::establishConnection() {
  if (!reconnect()) {
    throw std::runtime_error(
        "EstablishConnection: Error reconnecting to server");
  };
}

ClientConnection::ClientConnection(const std::string& ip, int port,
                                   int retry_timeout, int max_retries,
                                   bool retry_on_error)
    : Connection(ip, port),
      retry_timeout_(retry_timeout),
      max_retries_(max_retries),
      retry_on_error_(retry_on_error) {}

void ClientConnection::tryToConnect() {
  sockfd_ = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd_ < 0) {
    throw std::runtime_error("tryToConnect: Error creating socket");
  }

  sockaddr_in serv_addr;
  memset(&serv_addr, 0, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(port_);
  inet_pton(AF_INET, ip_.c_str(), &serv_addr.sin_addr);

  if (connect(sockfd_, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
    throw std::runtime_error("tryToConnect: Error connecting to server");
  }
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
      tryToConnect();
      connected = true;
      break;
    } catch (const std::runtime_error& e) {
    }

    // sleep(retry_timeout_ / 1000);  // Convert timeout to seconds
    std::this_thread::sleep_for(std::chrono::milliseconds(retry_timeout_));
  }

  return connected;
}
