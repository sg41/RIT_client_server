#ifndef SERVER_CLIENT_HANDLER_H
#define SERVER_CLIENT_HANDLER_H
#include <unordered_map>

#include "parser.h"
class Server;

class ClientHandler {
 public:
  ClientHandler(int socket, const std::string& id, Server* server);
  void sendMessageToClient(const std::string& message);
  std::string getClientID() const;
  void handleClient();

 private:
  std::string receiveMessage();
  void sendMessage(const std::string& message);
  std::string countLetters(const std::string& message);
  std::string processMessage(const std::string& message);

  int client_socket;
  std::string client_id;  // To store the unique client ID
  Server* server;         // Reference to the server for message routing
};
#endif  // SERVER_CLIENT_HANDLER_H