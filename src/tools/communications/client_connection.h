/**
 * @file client_connection.h
 * @author Sergey Gorbatenko (sg41@mail.ru)
 * @brief Client connection class header
 * @version 0.1
 * @date 2024-05-16
 *
 * @copyright Copyright (c) 2024
 *
 */
#ifndef TOOLS_COMMUNICATIONS_CLIENT_CONNECTION_H
#define TOOLS_COMMUNICATIONS_CLIENT_CONNECTION_H
#include "connection.h"

const std::string kServerDefaultIP = "127.0.0.1";  //< Default server IP
const int kMaxRetries = 3;                         //< Default Max Retries
const int kRetryTimeout = 300;  //< Default Timeout in milliseconds

/**
 * @brief The `ClientConnection` class is a subclass of the `Connection` class
 * and represents a client connection in a communication system. It includes
 * functionality to establish a connection to a server, reconnect to the server,
 * send messages, and receive messages. The class has default values for server
 * IP, maximum retries, retry timeout, and retry on error behavior. It provides
 * methods to establish a connection, reconnect, send messages, and receive
 * messages. If retry_on_error_ is set to true (default), all methods tries to
 * reconnect in case of failure as specified by max_retries_ and
 * retry_timeout_ parameters.
 */
class ClientConnection : public Connection {
 public:
  explicit ClientConnection(const std::string& ip = kServerDefaultIP,
                            int port = kDefaultPort,
                            int retry_timeout = kRetryTimeout,
                            int max_retries = kMaxRetries,
                            bool retry_on_error = true);
  void establishConnection() override;
  bool reconnect();
  void sendMessage(const std::string& message) override;
  std::string receiveMessage() override;

 private:
  void tryToConnect();

 protected:
  int retry_timeout_ = kRetryTimeout;
  int max_retries_ = kMaxRetries;
  bool retry_on_error_ = true;
};
#endif  // TOOLS_COMMUNICATIONS_CLIENT_CONNECTION_H