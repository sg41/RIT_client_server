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

#ifndef TOOLS_COMMUNICATIONS_CONNECTION_H
#define TOOLS_COMMUNICATIONS_CONNECTION_H
#include <string>

const int kMaxRetries = 3;
const int kRetryTimeout = 3;
const int kBufferSize = 1024;

// typedef enum class SocketType { kServer, kClient };

bool checkFDHaveData(int fd, int timeout = 100);

class Connection {
 public:
  explicit Connection(int fd);
  explicit Connection(const std::string& ip = "0.0.0.0",
                      int port = 8080);  //? maybe move it to derivatives?
  Connection(const Connection&) = delete;
  Connection& operator=(const Connection&) = delete;
  Connection(Connection&&) = default;
  Connection& operator=(Connection&&) = default;
  virtual ~Connection();

  void setEventTimeout(int timeout) { event_timeout_ = timeout; }
  int getEventTimeout() { return event_timeout_; }

  virtual void establishConnection(){};
  virtual void disconnect(){};
  bool checkHaveEvent();
  virtual void sendMessage(const std::string& message);
  virtual std::string receiveMessage();

 protected:
  // SocketType type_ = SocketType::kServer;
  std::string ip_ = "0.0.0.0";
  int port_ = 8080;
  int sockfd_ = -1;
  int event_timeout_ = 100;
};

#endif  // TOOLS_COMMUNICATIONS_CONNECTION_H