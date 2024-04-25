#include "client.h"

#define ENABLE_OUTPUT true

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
    std::cout << "> ";
    std::getline(std::cin, message);

    if (message == "exit") {
      break;
    }

    if (!client.sendMessage(message)) {
      client.reconnect();
      continue;
    };

    std::string response;
    if (!client.receiveMessage(response)) {
      client.reconnect();
      continue;
    }
    std::cout << "Server: " << response << std::endl;
  }

  return 0;
}