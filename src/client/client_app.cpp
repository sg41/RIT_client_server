#include "client_app.h"

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include <chrono>
#include <cstring>
#include <future>
#include <iostream>
#include <string>
#include <thread>

#define ENABLE_OUTPUT 1

ClientApp::ClientApp(const std::string& server_ip, int server_port, bool log)
    : client_(server_ip, server_port, log) {}

bool ClientApp::connectToServer() {
  if (!client_.connectToServer() && !client_.reconnect()) {
    return false;
  }
  return true;
}

ClientApp::Event ClientApp::eventLoop() {
  auto readUserInput = []() {
    std::string message;
    std::getline(std::cin, message);
    return message;
  };
  if (!input_thread_.valid())
    input_thread_ = std::async(std::launch::async, readUserInput);
  while (true) {
    if (client_.checkHaveMessage()) {
      return Event::kServerMessage;
    }
    if (input_thread_.wait_for(std::chrono::milliseconds(100)) ==
        std::future_status::ready) {
      message_ = input_thread_.get();
      return Event::kUserInput;
    }
  }
}

bool ClientApp::receiveServerMessage(std::string& response) {
  if (!client_.receiveMessage(response)) {
    return client_.reconnect();
  }
  return true;
}

bool ClientApp::talkToServer(std::string& response) {
  bool got_error = false;
  // Sending user input to server
  if (!client_.sendMessage(message_)) {
    if (!client_.reconnect()) {
      got_error = true;
    }
  }

  // Receiving response from server
  if (receiveServerMessage(response)) {
    std::cout << "Server: " << response << std::endl;
  } else {
    got_error = true;
  }
  return !got_error;
}

int ClientApp::run() {
  running_ = true;
  int error_code = 0;

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
        if (message_ == "exit" || std::cin.eof()) {
          running_ = false;
          break;
        }

        if (message_ == "") {
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
