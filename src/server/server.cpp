#include "server.h"

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include <chrono>
#include <cstring>
#include <iostream>
#include <string>
#include <thread>

Server::Server(int port, bool log)
    : server_socket_(-1), port_(port), log_(log) {}

bool Server::startServer() {
  server_socket_ = socket(AF_INET, SOCK_STREAM, 0);
  if (server_socket_ < 0) {
    if (log_)
      std::cerr << "Error creating socket: " << strerror(errno) << std::endl;
    return false;
  }

  int option = 1;
  setsockopt(server_socket_, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));

  sockaddr_in address;
  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons(port_);

  if (bind(server_socket_, reinterpret_cast<const struct sockaddr*>(&address),
           sizeof(address)) < 0) {
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

void Server::acceptConnections() {
  is_running_ = true;
  while (isRunning()) {
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
    std::string client_id = "client_" + std::to_string(next_client_id_++);
    ClientHandler* client_handler =
        new ClientHandler(client_socket, client_id, this);
    {  // Add to the clients map - protected by a mutex
      std::lock_guard<std::mutex> lock(clients_mutex_);
      clients_[client_id] = client_handler;
    }
    if (log_)
      std::cout << "New client connected with ID: " << client_id << std::endl;

    std::thread client_thread(&ClientHandler::handleClient, client_handler);
    client_thread.detach();
  }
  if (log_) std::cout << "Server stopped." << std::endl;
}

void Server::shutdown() {
  std::lock_guard<std::mutex> lock(clients_mutex_);
  if (server_socket_ >= 0) close(server_socket_);  // close server_socket_
  is_running_ = false;
}

void Server::removeClient(const std::string& client_id) {
  auto it = clients_.find(client_id);
  if (it != clients_.end()) {
    std::lock_guard<std::mutex> lock(clients_mutex_);
    delete it->second;
    clients_.erase(it);
  }
}

bool Server::routeMessage(const std::string& sender_id,
                          const std::string& receiver_id,
                          const std::string& message) {
  bool message_sent = false;
  auto it = clients_.find(receiver_id);
  if (it != clients_.end()) {
    it->second->sendMessage(sender_id + ": " + message);
    message_sent = true;
  } else {
    // Handle the case where the recipient is not found
    message_sent = false;
    if (log_)
      std::cout << "Client with ID " << receiver_id << " not found."
                << std::endl;
  }
  return message_sent;
}

const std::map<std::string, ClientHandler*>& Server::getClients() const {
  return clients_;
}

Server::~Server() {
  if (server_socket_ >= 0) {
    close(server_socket_);
  }
  for (auto it = clients_.begin(); it != clients_.end(); ++it) {
    delete it->second;
  }
}
