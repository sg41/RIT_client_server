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

const int kEventTimeout = 50;   //< Default event  timeout in milliseconds
const int kBufferSize = 1024;   //< Default Buffer Size
const int kDefaultPort = 8080;  //< Default port
const std::string kDefaultIP = "0.0.0.0";  //< Default IP

bool checkFDHaveData(int fd, int timeout = 100);

class Connection {
 public:
  explicit Connection(int fd);
  explicit Connection(
      const std::string& ip = kDefaultIP,
      int port = kDefaultPort);  //? maybe move it to derivatives?
  Connection(const Connection&) = delete;
  Connection& operator=(const Connection&) = delete;
  Connection(Connection&&) = default;
  Connection& operator=(Connection&&) = default;
  virtual ~Connection();

  void setEventTimeout(int timeout) { event_timeout_ = timeout; }
  int getEventTimeout() { return event_timeout_; }

  virtual void establishConnection() {};
  void disconnect();
  bool checkHaveEvent();
  virtual void sendMessage(const std::string& message);
  virtual std::string receiveMessage();

 protected:
  std::string ip_ = kDefaultIP;
  int port_ = kDefaultPort;
  int sockfd_ = -1;
  int event_timeout_ = kEventTimeout;
};

#endif  // TOOLS_COMMUNICATIONS_CONNECTION_H