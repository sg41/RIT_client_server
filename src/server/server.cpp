/**
 * @file server.cpp
 * @author Sergey Gorbatenko (sg41@mail.ru)
 * @brief Implementation of Server class
 * @version 0.1
 * @date 2024-05-01
 *
 * @copyright Copyright (c) 2024
 *
 */
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
  fd_set read_fds;
  while (isRunning()) {
    FD_ZERO(&read_fds);
    FD_SET(server_socket_, &read_fds);
    struct timeval timeout;
    timeout.tv_sec = 0;
    timeout.tv_usec = 100;
    int ret = select(server_socket_ + 1, &read_fds, nullptr, nullptr, &timeout);
    if (ret < 0) {
      if (log_)
        std::cerr << "Error in select: " << strerror(errno) << std::endl;
      continue;
    } else if (ret == 0) {
      // Timeout occurred, continue to the next iteration
      continue;
    }
    acceptNewClient();
  }
  if (log_) std::cout << "Server stopped." << std::endl;
}

void Server::acceptNewClient() {
  sockaddr_in client_address;
  socklen_t client_addr_len = sizeof(client_address);
  int client_socket = accept(
      server_socket_, reinterpret_cast<struct sockaddr*>(&client_address),
      &client_addr_len);
  if (client_socket < 0) {
    if (log_)
      std::cerr << "Error accepting connection: " << strerror(errno)
                << std::endl;
    return;
  }

  std::string client_id;

  {  // Add to the clients map - protected by a mutex
    std::lock_guard<std::mutex> lock(server_mutex_);
    client_id = "client_" + std::to_string(next_client_id_++);
    clients_[client_id] =
        std::make_shared<ClientHandler>(client_socket, client_id, this);
    client_threads_.push_back(
        std::thread(&ClientHandler::handleClient, clients_[client_id]));
  }
  if (log_)
    std::cout << "New client connected with ID: " << client_id << std::endl;
}

void Server::shutdown() {
  std::lock_guard<std::mutex> lock(server_mutex_);
  is_running_ = false;
}

void Server::removeClient(const std::string& client_id) {
  std::lock_guard<std::mutex> lock(server_mutex_);
  auto it = clients_.find(client_id);
  if (it != clients_.end()) {
    clients_.erase(it);
  }
}

bool Server::routeMessage(const std::string& sender_id,
                          const std::string& receiver_id,
                          const std::string& message) {
  bool message_sent = false;
  auto it = clients_.find(receiver_id);
  if (receiver_id == "self") it = clients_.find(sender_id);
  if (it != clients_.end()) {
    it->second->sendMessage(sender_id + ": " + message);
    message_sent = true;
  } else {
    message_sent = false;
    if (log_)
      std::cout << "Client with ID " << receiver_id << " not found."
                << std::endl;
  }
  return message_sent;
}

const std::map<std::string, std::shared_ptr<ClientHandler>>&
Server::getClients() const {
  return clients_;
}

Server::~Server() {
  if (server_socket_ >= 0) {
    close(server_socket_);
  }
  for (auto& thread : client_threads_) {
    if (thread.joinable()) thread.join();
  }
}
