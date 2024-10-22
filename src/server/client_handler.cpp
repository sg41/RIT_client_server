/**
 * @file client_handler.cpp
 * @author Sergey Gorbatenko (sg41@mail.ru)
 * @brief Client handler class implementation
 * @version 0.1
 * @date 2024-05-01
 *
 * @copyright Copyright (c) 2024
 *
 */
#include "client_handler.h"

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include <chrono>
#include <cstring>
#include <iostream>
#include <map>
#include <string>
#include <thread>

#include "command.h"
#include "server.h"

ClientHandler::ClientHandler(std::shared_ptr<Connection>&& connection,
                             const std::string& id, Server* server)
    : connection_(connection), client_id(id), server(server) {}

void ClientHandler::handleClient() {
  std::string message;
  while (server->isRunning()) {
    if (connection_->checkHaveEvent()) {
      message = receiveMessage();
    } else {
      continue;
    }
    if (message.empty()) {
      break;  // Client disconnected
    }
    // Process message and send response
    if (server->isLogEnabled())
      std::cout << "Server: received from " << client_id << ": " << message
                << std::endl;
    std::string response = processMessage(message);
    sendMessage(response);
  }

  server->removeClient(client_id);
  if (server->isLogEnabled())
    std::cout << "Server: Client disconnected." << std::endl;
}

std::string ClientHandler::receiveMessage() {
  std::string message;
  try {
    message = connection_->receiveMessage();
  } catch (std::runtime_error& e) {
    if (server->isLogEnabled())
      std::cout << "Server: Error receiving message: " << e.what() << std::endl;
    message = "";
  }
  return message;
}

std::string ClientHandler::sendMessage(const std::string& message) {
  std::string answer = "Ok";
  try {
    connection_->sendMessage(message);
  } catch (std::runtime_error& e) {
    answer = std::string("Server: ") + e.what();
  }
  return answer;
}

std::string ClientHandler::countLetters(const std::string& message) {
  std::unordered_map<char, int> letter_counts;
  for (char c : message) {
    letter_counts[c]++;
  }
  std::string response = "\nMessage |" + message + "\n";

  for (auto letter : message) {
    if (letter_counts[letter] != 0) {
      response += std::string(1, letter) + "       |" +
                  std::to_string(letter_counts[letter]) + "\n";
      letter_counts[letter] = 0;
    }
  }
  return response;
}

std::string ClientHandler::processMessage(const std::string& message) {
  // std::map<std::string, std::unique_ptr<Command>> commands;
  // commands["send"] = std::make_unique<CommunicateCommand>();
  // commands["show"] = std::make_unique<ShowCommand>();
  static const std::map<std::string,
                        std::string (ClientHandler::*)(const std::string&)>
      commands{{"send", &ClientHandler::sendMessageToClient},
               {"show", &ClientHandler::showConnections},
               {"shutdown", &ClientHandler::shutdownServer}};

  Parser parser(message, commands, "", "");
  if (!parser.hasCommand()) {
    return countLetters(message);
  }

  auto it = commands.find(parser.getCommand());
  if (it != commands.end()) {
    // return it->second->execute(*this, parser.getArgument());
    return (this->*it->second)(parser.getArgument());
  } else {
    return "Unknown command.";
  }
}

std::string ClientHandler::showConnections(const std::string& message) {
  Parser parser(message, {"number", "list", "count"}, "", "");
  if (!parser.hasCommand()) {
    return "Invalid command format";
  }

  std::lock_guard<std::mutex> lock(server->getServerMutex());
  auto clients = server->getClients();
  if (parser.getCommand() == "list") {
    std::string response = "\nClient ID:\n";
    for (auto it = clients.begin(); it != clients.end(); ++it) {
      response += it->first + "\n";
    }
    return response;
  } else {
    return std::to_string(clients.size());
  }
  return "Invalid command format";
}
std::string ClientHandler::sendMessageToClient(const std::string& message) {
  // Get the client ID from the message in "<client_id> message" format
  Parser parser(message);

  if (parser.hasCommand()) {
    auto receiver_id = parser.getCommand();
    return server->routeMessage(client_id, receiver_id, parser.getArgument())
               // if message sent to itself - no extra answer needed
               ? ((client_id == receiver_id || receiver_id == "self")
                      ? ""
                      : "Message sent")
               : "Error";
  } else {
    return "Invalid command format";
  }
};

std::string ClientHandler::shutdownServer(
    [[maybe_unused]] const std::string& message) {
  server->shutdown();
  return "Server shut down";
}

void ClientHandler::disconnect() { connection_->disconnect(); }