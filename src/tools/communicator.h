/**
 * @file socket_utils.h
 * @author Sergey Gorbatenko (sg41@mail.ru)
 * @brief socket utilities library
 * @version 0.1
 * @date 2024-05-14
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef TOOLS_COMMUNICATOR_H
#define TOOLS_COMMUNICATOR_H
#include <string>
const int kMaxRetries = 3;
const int kRetryTimeout = 3;
const int kBufferSize = 1024;
typedef enum class SocketType { kServer, kClient };
class Connection {
 public:
  Connection(std::string ip = "0.0.0.0", int port = 8080);
  Connection(const Connection&) = delete;
  Connection& operator=(const Connection&) = delete;
  Connection(Connection&&) = delete;
  Connection& operator=(Connection&&) = delete;
  virtual ~Connection();

  virtual void establishConnection();
  virtual void disconnect();
  bool checkHaveMessage(int fd, int timeout = 100);
  void sendMessage(const std::string& message);
  std::string receiveMessage();

 protected:
  SocketType type_ = SocketType::kServer;
  std::string ip_ = "0.0.0.0";
  int port_ = 8080;
  int sockfd_ = -1;
};

class ServerConnection : public Connection {
 public:
  void establishConnection() override;
  bool checkNewClient();
  int acceptNewClient();
};

class ClientConnection : public Connection {
  void establishConnection() override;
  bool reconnect(int timeout = 3, int maxAttempts = 0);
};
#endif  // TOOLS_COMMUNICATOR_H