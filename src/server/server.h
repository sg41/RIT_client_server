#ifndef SERVER_SERVER_H
#define SERVER_SERVER_H
#include <map>
#include <memory>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

#include "client_handler.h"

class Server {
 public:
  explicit Server(int port, bool log = false);
  bool startServer();
  void acceptConnections();
  void acceptNewClient();
  void shutdown();
  void removeClient(const std::string& client_id);
  bool routeMessage(const std::string& sender_id,
                    const std::string& receiver_id, const std::string& message);
  const std::map<std::string, std::shared_ptr<ClientHandler>>& getClients()
      const;
  ~Server();
  std::mutex& getClientsMutex() { return clients_mutex_; }
  bool logEnabled() const { return log_; }
  bool isRunning() const { return is_running_; }

 private:
  bool is_running_ = false;
  std::mutex clients_mutex_;
  int server_socket_ = -1;
  int port_ = 8080;  // Default port
  bool log_ = false;
  std::map<std::string, std::shared_ptr<ClientHandler>> clients_;
  unsigned next_client_id_ = 1;  // To generate unique IDs
  std::vector<std::thread> client_threads_;
};

#endif  // SERVER_SERVER_H