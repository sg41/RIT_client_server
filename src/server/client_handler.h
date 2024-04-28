#ifndef SERVER_CLIENT_HANDLER_H
#define SERVER_CLIENT_HANDLER_H
#include <unordered_map>

#include "parser.h"
class Server;

class ClientHandler {
 public:
  ClientHandler(int socket, const std::string& id, Server* server);
  void handleClient();
  std::string getClientID() const;
  const Server* getServer() const;

  std::string receiveMessage();
  std::string sendMessage(const std::string& message);
  std::string processMessage(const std::string& message);

  std::string sendMessageToClient(const std::string& message);
  std::string countLetters(const std::string& message);
  std::string showConnections(const std::string& message);

 private:
  int client_socket;
  std::string client_id;  // To store the unique client ID
  Server* server;         // Reference to the server for message routing
};
#endif  // SERVER_CLIENT_HANDLER_H