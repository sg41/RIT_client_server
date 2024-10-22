/**
 * @file client_app.cpp
 * @author Sergey Gorbatenko (sg41@mail.ru)
 * @brief Client application class implementation
 * @version 0.1
 * @date 2024-05-01
 *
 * @copyright Copyright (c) 2024
 *
 */
#include "client_app.h"

#include <chrono>
#include <cstring>
#include <future>
#include <iostream>
#include <string>
#include <thread>

#include "parser.h"

#define ENABLE_OUTPUT 1

ClientApp::ClientApp(const std::string& server_ip, int server_port, bool log)
    : client_(server_ip, server_port, log) {}

bool ClientApp::connectToServer() { return client_.connectToServer(); }

Event ClientApp::eventLoop() {
  while (running_) {
    auto event = client_.checkHaveEvent();
    if (event != Event::kNoEvent) {
      return event;
    }
  }
  return Event::kNoEvent;
}

bool ClientApp::receiveServerMessage(std::string& response) {
  if (!client_.receiveMessage(response)) {
    return false;
  }
  return true;
}

bool ClientApp::talkToServer(std::string& response) {
  bool got_error = false;
  // Sending user input to server
  if (!client_.sendMessage(message_)) {
    got_error = true;
  }

  // Receiving response from server
  if (receiveServerMessage(response)) {
    std::cout << "Server: " << response << std::endl;
  } else {
    got_error = true;
  }
  return !got_error;
}

void ClientApp::shutdownServer() {
  if (!client_.sendMessage("shutdown")) {
    std::cout << "Failed to send shutdown message to server." << std::endl;
  } else {
    std::cout << "Shutdow sent to server." << std::endl;
    performExit();
  }
}

void ClientApp::performExit() { running_ = false; }

void ClientApp::showHelp() {
  std::cout << "Available commands:" << std::endl;
  std::cout << "  Client side:" << std::endl;
  std::cout << "    exit|quit|!q - exit the program" << std::endl;
  std::cout << "    help|? - display this help" << std::endl;
  std::cout << "  Server side:" << std::endl;
  std::cout << "    show list - show the list of connected clients"
            << std::endl;
  std::cout << "    show number|count - show the number of connected clients"
            << std::endl;
  std::cout << "    send <client_ID> message - send a message to the client"
            << std::endl;
  std::cout << "    shutdown - shutdown the server" << std::endl;
}
int ClientApp::run() {
  running_ = true;
  int error_code = 0;

  Parser parser(message_, valid_commands_, "", "");
  while (running_) {
    std::string response;
    std::cout << "> " << std::flush;

    // Check if the client has a message from server or from user input
    Event event = eventLoop();

    switch (event) {
      case Event::kServerMessage:
        if (receiveServerMessage(response)) {
          std::cout << "Got new message: " << response << std::endl;
        } else {
          running_ = false;
          error_code = 1;
          std::cout << "Server disconnected." << std::endl;
        }
        break;
      case Event::kUserInput:  // Got user message block:
        std::getline(std::cin, message_);
        if (std::cin.eof()) {
          running_ = false;
          break;
        }
        if (message_ == "") {
          break;
        }
        parser.parse(message_);
        if (parser.hasCommand()) {  // Execute client-side commands
          valid_commands_[parser.getCommand()](this);
          break;
        }
        if (!talkToServer(response)) {
          running_ = false;
          error_code = 1;
          std::cout << "Server disconnected." << std::endl;
          break;
        }
      default:
        break;
    }
  }
  return error_code;
}
