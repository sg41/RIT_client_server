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

bool checkFDHaveData(int fd, int timeout = 100);

class Connection {
 public:
  Connection(std::string ip = "0.0.0.0", int port = 8080);
  Connection(const Connection&) = delete;
  Connection& operator=(const Connection&) = delete;
  Connection(Connection&&) = delete;
  Connection& operator=(Connection&&) = delete;
  virtual ~Connection();
  void setEventTimeout(int timeout) { event_timeout_ = timeout; }
  int getEventTimeout() { return event_timeout_; }

  virtual void establishConnection() = 0;
  virtual void disconnect();
  bool checkHaveEvent();
  virtual void sendMessage(const std::string& message);
  virtual std::string receiveMessage();

 protected:
  SocketType type_ = SocketType::kServer;
  std::string ip_ = "0.0.0.0";
  int port_ = 8080;
  int sockfd_ = -1;
  int event_timeout_ = 100;
};

class ServerConnection : public Connection {
 public:
  void establishConnection() override;
  bool checkNewClient();
  int acceptNewClient();
};

class ClientConnection : public Connection {
  ClientConnection(std::string ip = "127.0.0.1", int port = 8080,
                   int retry_timeout = 3000, int max_retries = 3,
                   bool retry_on_error = true);
  void establishConnection() override;
  bool reconnect();
  void sendMessage(const std::string& message) override;
  std::string receiveMessage() override;

 protected:
  int retry_timeout_ = 3000;
  int max_retries_ = 3;
  bool retry_on_error_ = true;
};
#endif  // TOOLS_COMMUNICATOR_H