#include "client_handler.h"

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include <chrono>
#include <cstring>
#include <iostream>
#include <string>
#include <thread>

#include "server.h"

ClientHandler::ClientHandler(int socket, const std::string& id, Server* server)
    : client_socket(socket), client_id(id), server(server) {}

std::string ClientHandler::sendMessageToClient(const std::string& message) {
  return sendMessage(message);  // Use the existing sendMessage method
}

std::string ClientHandler::getClientID() const { return client_id; }

void ClientHandler::handleClient() {
  while (true) {
    std::string message = receiveMessage();
    if (message.empty()) {
      break;  // Client disconnected
    }

    // Process message and send response
    std::cout << "Server received from " << client_id << ": " << message
              << std::endl;
    std::string response = processMessage(message);
    sendMessage(response);
  }

  close(client_socket);
  server->removeClient(client_id);
  std::cout << "Client disconnected." << std::endl;
}

std::string ClientHandler::receiveMessage() {
  char buffer[1024] = {0};
  int bytes_received = recv(client_socket, buffer, 1024, 0);
  if (bytes_received <= 0) {
    return "";
  }
  return std::string(buffer, bytes_received);
}

std::string ClientHandler::sendMessage(const std::string& message) {
  int result = send(client_socket, message.c_str(), message.length(), 0);
  return result == message.length() ? "Ok" : "Error";
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
  std::string response;
  // TODO provide command_list  to parser constructor from here
  // TODO use pointers to metods in command_list
  std::map<std::string, int> valid_commands{{"send", 2}, {"show", 1}};
  std::map<std::string,
           std::string (ClientHandler::*)(const std::string& message)>
      commands = {{"send", &ClientHandler::sendMessage},
                  {"show", &ClientHandler::countLetters}};
  Parser parser(message, valid_commands);

  if (parser.hasCommand()) {
    if (parser.getCommand() == "communicate") {
      auto args = parser.getArguments();
      if (args.size() == 2) {
        server->routeMessage(client_id, args[0], args[1]);
        return "Message sent";
      } else {
        return "Invalid 'communicate' command format";
      }
    }
    response = "Command: " + parser.getCommand() + " not implemented";
  } else {
    response = countLetters(message);
  }

  return response;
}
