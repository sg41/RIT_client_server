/**
 * @file server.h
 * @author Sergey Gorbatenko (sg41@mail.ru)
 * @brief Server class for TCP/IP communication
 * @version 0.1
 * @date 2024-05-01
 *
 * @copyright Copyright (c) 2024
 *
 */
#ifndef SERVER_SERVER_H
#define SERVER_SERVER_H
#include <atomic>
#include <map>
#include <memory>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

#include "client_handler.h"
#include "server_connection.h"

/**
 * The Server class represents a TCP/IP server that listens for incoming
 * connections and handles them asynchronously. It maintains a map of
 * connected clients and allows for sending messages between clients.
 *
 * This class provides methods for starting the server, accepting incoming
 * connections, and handling client messages. It also allows for removing
 * clients and routing messages between clients.
 *
 * Example Usage:
 *
 * Server server(8080, true); // Create a server instance on port 8080 with
 * logging enabled
 * server.startServer(); // Start the server
 *
 * Note: Private variables are not documented individually.
 *
 */
class Server {
 public:
  explicit Server(int port, bool log = false);
  /**
   * Rule of 5 implementation
   */
  Server(const Server&) = delete;
  Server(Server&&) = delete;
  Server& operator=(const Server&) = delete;
  Server& operator=(Server&&) = delete;
  ~Server();

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
  std::mutex& getServerMutex() { return server_mutex_; }
  bool isLogEnabled() const { return log_; }
  bool isRunning() const { return is_running_; }

 private:
  std::atomic<bool> is_running_ = false;
  std::mutex server_mutex_;
  std::shared_ptr<ServerConnection> connection_;
  int port_ = 8080;  // Default port
  bool log_ = false;
  std::map<std::string, std::shared_ptr<ClientHandler>>
      clients_;                  // Connected clients
  unsigned next_client_id_ = 1;  // To generate unique IDs
  std::vector<std::thread> client_threads_;
};

#endif  // SERVER_SERVER_H