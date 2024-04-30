#ifndef CLIENT_CLIENT_H
#define CLIENT_CLIENT_H
#include <string>

const int kMaxRetries = 3;
const int kRetryTimeout = 3;
const int kBufferSize = 1024;

class Client {
 public:
  Client(const std::string& ip, int port, bool log = false);
  bool connectToServer();
  bool checkHaveMessage();
  bool sendMessage(const std::string& message);
  bool receiveMessage(std::string& message);
  bool reconnect();
  ~Client();

 private:
  std::string server_ip_;
  int server_port_;
  int sockfd_ = -1;
  bool log_ = false;
};

#endif  // CLIENT_CLIENT_H