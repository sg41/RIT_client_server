#ifndef SERVER_COMMAND_H
#define SERVER_COMMAND_H
#include <stdarg.h>

#include <string>
#include <vector>

#include "client_handler.h"
class Command {
 public:
  virtual ~Command() {}
  virtual std::string execute(ClientHandler& handler, ...) = 0;

 protected:
  int nargs;
  std::string name_;
};

class CommunicateCommand : public Command {
 public:
  CommunicateCommand(const std::string& name, int nargs);
  std::string execute(ClientHandler& handler, ...) override {
    //   if (args.size() < 2) {
    //     return "Error: Not enough arguments for communicate command.";
    //   }
    //   std::string target_client_id = args[0];
    //   std::string message = args[1];

    //   // Access the server's client map to find the target client
    //   auto it = handler.getServer().getClients().find(target_client_id);
    //   if (it != handler.getServer().getClients().end()) {
    //     it->second->sendMessageToClient(message);
    //     return "Message sent to client " + target_client_id;
    //   } else {
    //     return "Error: Client not found.";
    //   }
    // }
  };
};
#endif  // SERVER_COMMAND_H