#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include <chrono>
#include <cstring>
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
  if (!client.connectToServer()) {
    return 1;
  }

  while (true) {
    std::string message;
    std::string response;
    bool have_message = false;
    std::cout << "> ";
    while (true) {
      char c;
      // c = std::cin.get();
      std::cin.readsome(&c, 1);
      if (!std::cin.eof()) {
        if (c == '\n') break;
        message.append(&c, 1);
      } else {
        break;  // Will exit because of std::cin.eof()
      }
      if (client.checkHaveMessage()) {
        have_message = true;
        break;
      }
    }
    if (have_message) {
      if (!client.receiveMessage(response)) {
        client.reconnect();
        continue;
      }
      std::cout << "Got new message: " << response << std::endl;
      continue;
    }

    if (message == "exit" || std::cin.eof()) {
      break;
    }

    if (message == "") {
      continue;
    }

    if (!client.sendMessage(message)) {
      client.reconnect();
      continue;
    };

    if (!client.receiveMessage(response)) {
      client.reconnect();
      continue;
    }
    std::cout << "Server: " << response << std::endl;
  }

  return 0;
}