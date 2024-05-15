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

#include <unistd.h>

#include <chrono>
#include <cstring>
#include <iostream>
#include <string>
#include <thread>

Client::Client(const std::string& ip, int port, bool log)
    : server_ip_(ip), server_port_(port), log_(log) {
  connection_ = std::make_shared<ClientConnection>(server_ip_, server_port_);
}

bool Client::connectToServer() {
  try {
    connection_->establishConnection();
    if (log_) std::cout << "Client: Connected to server!" << std::endl;
  } catch (std::runtime_error& e) {
    if (log_) std::cout << "Client: " << e.what() << std::endl << std::endl;
    return false;
  }
  return true;
}

Event Client::checkHaveEvent() {
  if (connection_->checkHaveEvent()) {
    return Event::kServerMessage;
  } else if (::checkFDHaveData(STDIN_FILENO)) {
    return Event::kUserInput;
  }
  return Event::kNoEvent;
}

bool Client::sendMessage(const std::string& message) {
  try {
    connection_->sendMessage(message);
  } catch (std::runtime_error& e) {
    if (log_) std::cout << "Client: " << e.what() << std::endl << std::endl;
    return false;
  }
  return true;
}

bool Client::receiveMessage(std::string& message) {
  try {
    message = connection_->receiveMessage();
  } catch (std::runtime_error& e) {
    if (log_) std::cout << "Client: " << e.what() << std::endl << std::endl;
    return false;
  }
  return true;
}

void Client::disconnect() { connection_->disconnect(); }