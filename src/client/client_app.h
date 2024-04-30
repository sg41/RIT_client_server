#ifndef CLIENT_CLIENT_APP_H
#define CLIENT_CLIENT_APP_H
#include <future>
#include <map>

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
  void exit();
  void help();

 private:
  Client client_;
  std::string message_{};
  std::future<std::string> input_thread_;
  bool running_ = false;
  // Map of valid commands served by client side
  std::map<std::string, void (ClientApp::*)()> valid_commands_ = {
      {"exit", &ClientApp::exit},
      {"help", &ClientApp::help},
      {"quit", &ClientApp::exit},
      {"!q", &ClientApp::exit},
      {"?", &ClientApp::help}};
};
#endif  // CLIENT_CLIENT_APP_H