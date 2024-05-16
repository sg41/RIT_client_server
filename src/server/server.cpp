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

Server::Server(int port, bool log) : port_(port), log_(log) {
  connection_ = std::make_shared<ServerConnection>(kDefaultIP, port);
}

bool Server::startServer() {
  try {
    connection_->establishConnection();
  } catch (std::runtime_error& e) {
    if (log_) std::cout << "Server: " << e.what() << std::endl << std::endl;
    return false;
  }

  if (log_) std::cout << "Server: listening on port " << port_ << std::endl;
  return true;
}

void Server::acceptConnections() {
  is_running_ = true;
  while (isRunning()) {
    if (connection_->checkNewIncomingConnection()) {
      acceptNewClient();
    }
  }
  if (log_) std::cout << "Server: stopped." << std::endl;
}

void Server::acceptNewClient() {
  std::shared_ptr<Connection> client_connection;
  try {
    client_connection = connection_->acceptIncomingConnection();
  } catch (std::runtime_error& e) {
    if (log_) std::cout << "Server: " << e.what() << std::endl << std::endl;
    return;
  }
  std::string client_id;
  {  // Add to the clients map - protected by a mutex
    std::lock_guard<std::mutex> lock(server_mutex_);
    client_id = "client_" + std::to_string(next_client_id_++);
    clients_[client_id] = std::make_shared<ClientHandler>(
        std::move(client_connection), client_id, this);
    client_threads_.push_back(
        std::thread(&ClientHandler::handleClient, clients_[client_id]));
  }
  if (log_)
    std::cout << "Server: New client connected with ID: " << client_id
              << std::endl;
}

void Server::shutdown() { is_running_ = false; }

void Server::removeClient(const std::string& client_id) {
  std::lock_guard<std::mutex> lock(server_mutex_);
  auto it = clients_.find(client_id);
  if (it != clients_.end()) {
    clients_.erase(it);
  }
}

// TODO: Maybe sending message is a duty of ClientHandler
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
      std::cout << "Server: Client with ID " << receiver_id << " not found."
                << std::endl;
  }
  return message_sent;
}

const std::map<std::string, std::shared_ptr<ClientHandler>>&
Server::getClients() const {
  return clients_;
}

Server::~Server() {
  for (auto& thread : client_threads_) {
    if (thread.joinable()) thread.join();
  }
}
