/**
 * @file command.h
 * @author sg41@mail.ru
 * @brief "Command" pattern for future development of server
 * @version 0.1
 * @date 2024-04-30
 *
 * @copyright Copyright (c) 2024
 *
 */
#ifndef SERVER_COMMAND_H
#define SERVER_COMMAND_H
#include <stdarg.h>

#include <string>
#include <vector>

#include "client_handler.h"
#include "parser.h"
class Command {
 public:
  virtual ~Command() {}
  virtual std::string execute(ClientHandler& handler,
                              const std::string& message) = 0;

 protected:
  int nargs = 0;
  std::string name_;
};

class CommunicateCommand : public Command {
 public:
  CommunicateCommand(){};
  std::string execute(ClientHandler& handler,
                      const std::string& message) override {
    return handler.sendMessageToClient(message);
  };
};

class ShowCommand : public Command {
 public:
  ShowCommand(){};
  std::string execute(ClientHandler& handler,
                      const std::string& message) override {
    return handler.showConnections(message);
  };
};
#endif  // SERVER_COMMAND_H