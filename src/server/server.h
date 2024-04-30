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
  /**
   * Starts the server by creating a socket, binding it to a specific address
   * and port, and listening for incoming connections.
   *
   * @return true if the server started successfully, false otherwise
   *
   * @throws None
   */
  bool startServer();
  /**
   * Function to accept incoming connections and handle them asynchronously.
   */
  void acceptConnections();
  void acceptNewClient();
  void shutdown();
  /**
   * Removes a client from the server's client map.
   *
   * @param client_id The ID of the client to be removed.
   *
   * @return None
   *
   * @throws None
   */
  void removeClient(const std::string& client_id);
  /**
   * Routes a message from a sender to a receiver.
   *
   * @param sender_id The ID of the message sender
   * @param receiver_id The ID of the message receiver
   * @param message The content of the message
   *
   * @return A boolean indicating if the message was successfully sent
   *
   * @throws None
   */
  bool routeMessage(const std::string& sender_id,
                    const std::string& receiver_id, const std::string& message);
  const std::map<std::string, std::shared_ptr<ClientHandler>>& getClients()
      const;
  ~Server();
  std::mutex& getServerMutex() { return server_mutex_; }
  bool isLogEnabled() const { return log_; }
  bool isRunning() const { return is_running_; }

 private:
  bool is_running_ = false;
  std::mutex server_mutex_;
  int server_socket_ = -1;
  int port_ = 8080;  // Default port
  bool log_ = false;
  std::map<std::string, std::shared_ptr<ClientHandler>>
      clients_;                  // Connected clients
  unsigned next_client_id_ = 1;  // To generate unique IDs
  std::vector<std::thread> client_threads_;
};

#endif  // SERVER_SERVER_H