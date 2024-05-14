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

#ifndef TOOLS_COMMUNICATOR_H
#define TOOLS_COMMUNICATOR_H
#include <string>

class Communicator {
 public:
  typedef enum class SocketType { Server, Client };
  Communicator(SocketType, std::string ip = "0.0.0.0", int port = 8080);
  ~Communicator();

  void createSocket();
  void bindSocket(int port);
  void listenSocket(int max_connections);
  int acceptConnection();
  void sendMessage(const std::string& message);
  std::string receiveMessage();
  void closeSocket();

 private:
  void createServerSocket();
  void createClientSocket();

 private:
  SocketType type_;
  std::string ip_;
  int port_;
  int sockfd_;
};
#endif  // TOOLS_COMMUNICATOR_H