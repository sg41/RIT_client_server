#ifndef SERVER_SERVER_H
#define SERVER_SERVER_H
#include "client_handler.h"

class Server {
 public:
  Server(int port, bool log = false);
  bool startServer();
  void acceptConnections();
  void removeClient(const std::string& client_id);
  void routeMessage(const std::string& sender_id,
                    const std::string& receiver_id, const std::string& message);
  ~Server();

 private:
  int server_socket_ = -1;
  int port_;
  bool log_ = false;
  std::map<std::string, ClientHandler*> clients;
  int next_client_id = 1;  // To generate unique IDs
};
#endif  // SERVER_SERVER_H