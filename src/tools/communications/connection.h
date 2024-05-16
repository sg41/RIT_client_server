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

/**
 * Check if there is data available to read on the given file descriptor within
 * the specified timeout.
 *
 * @param fd The file descriptor to check.
 * @param timeout The timeout value in microseconds.
 *
 * @return true if there is data available to read, false otherwise.
 *
 * @throws std::runtime_error if the file descriptor is invalid.
 */
bool checkFDHaveData(int fd, int timeout = 100);

/**
 * @brief The `Connection` class represents a generic connection with basic
 * functionality for establishing, managing, and communicating over a network
 * connection. It provides methods for establishing a connection, sending and
 * receiving messages, setting event timeouts, and disconnecting from the
 * connection. The class can be used as a base class for specific types of
 * connections, such as TCP or UDP connections, by deriving from it and
 * implementing the necessary functionality for those specific types.
 * This class could be used to serve client connection on server side or ther
 * cases, where connetion is already established and represented dy file
 * descriptor.
 */
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
  virtual std::string receiveMessage();  //< blocking read

 protected:
  std::string ip_ = kDefaultIP;
  int port_ = kDefaultPort;
  int sockfd_ = -1;
  int event_timeout_ = kEventTimeout;
};

#endif  // TOOLS_COMMUNICATIONS_CONNECTION_H