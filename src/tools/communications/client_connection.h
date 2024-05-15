#ifndef TOOLS_COMMUNICATIONS_CLIENT_CONNECTION_H
#define TOOLS_COMMUNICATIONS_CLIENT_CONNECTION_H
#include "connection.h"

class ClientConnection : public Connection {
  ClientConnection(const std::string& ip = "127.0.0.1", int port = 8080,
                   int retry_timeout = 3000, int max_retries = 3,
                   bool retry_on_error = true);
  void establishConnection() override;
  bool reconnect();
  void sendMessage(const std::string& message) override;
  std::string receiveMessage() override;

 private:
  void tryToConnect();

 protected:
  int retry_timeout_ = 3000;
  int max_retries_ = 3;
  bool retry_on_error_ = true;
};
#endif  // TOOLS_COMMUNICATIONS_CLIENT_CONNECTION_H