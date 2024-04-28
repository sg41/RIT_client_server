#ifndef CLIENT_CLIENT_APP_H
#define CLIENT_CLIENT_APP_H
#include <future>

#include "client.h"
class ClientApp {
 public:
  enum class Event { kUserInput, kServerMessage };
  ClientApp(const std::string& server_ip, int server_port, bool log = false);
  ~ClientApp() = default;

  bool connectToServer();
  int run();

 private:
  Event eventLoop();
  bool receiveServerMessage(std::string& response);
  bool talkToServer(std::string& response);

 private:
  Client client_;
  std::string message_{};
  std::future<std::string> input_thread_;
  bool running_ = false;
};
#endif  // CLIENT_CLIENT_APP_H