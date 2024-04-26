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

#include "client.h"

int main(int argc, char** argv) {
  if (argc < 3) {
    std::cerr << "Usage: " << argv[0] << " <server_ip> <server_port>"
              << std::endl;
    return 1;
  }

  std::string server_ip = argv[1];
  int server_port = atoi(argv[2]);

  Client client(server_ip, server_port, ENABLE_OUTPUT);
  if (!client.connectToServer() && !client.reconnect()) {
    return 1;
  }

  std::string message;
  auto readUserInput = [] {
    std::string message;
    std::getline(std::cin, message);
    return message;
  };
  std::future<std::string> message_thread =
      std::async(std::launch::async, readUserInput);

  while (true) {
    std::string response;
    bool server_message = false;
    bool user_message = false;
    std::cout << "> ";
    if (!message_thread.valid())
      message_thread = std::async(std::launch::async, readUserInput);

    // Check if the client has a message from server or from user input
    while (true) {
      if (client.checkHaveMessage()) {
        server_message = true;
        break;
      }
      if (message_thread.wait_for(std::chrono::milliseconds(100)) ==
          std::future_status::ready) {
        message = message_thread.get();
        user_message = true;
        break;
      }
    }

    if (server_message) {
      if (!client.receiveMessage(response)) {
        if (client.reconnect()) {
          continue;
        } else {
          std::cout << "Server disconnected." << std::endl;
          break;
        }
      }
      std::cout << "Got new message: " << response << std::endl;
    }

    if (!user_message) {
      continue;
    }

    {  // Got user message block:
      if (message == "exit" || std::cin.eof()) {
        break;
      }

      if (message == "") {
        continue;
      }

      if (!client.sendMessage(message)) {
        if (client.reconnect()) {
          continue;
        } else {
          std::cout << "Server disconnected." << std::endl;
          break;
        }
      };

      if (!client.receiveMessage(response)) {
        if (client.reconnect()) {
          continue;
        } else {
          std::cout << "Server disconnected." << std::endl;
          break;
        }
      }
      std::cout << "Server: " << response << std::endl;
    }  // End of got user message block
  }

  std::cout << "\nBye!" << std::endl;
  return 0;
}