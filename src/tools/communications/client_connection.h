#ifndef TOOLS_COMMUNICATIONS_CLIENT_CONNECTION_H
#define TOOLS_COMMUNICATIONS_CLIENT_CONNECTION_H
#include "connection.h"

const std::string kServerDefaultIP = "127.0.0.1";  //< Default server IP
const int kMaxRetries = 3;                         //< Default Max Retries
const int kRetryTimeout = 300;  //< Default Timeout in milliseconds

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