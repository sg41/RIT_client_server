/**
 * @file server_connection.h
 * @author Sergey Gorbatenko (sg41@mail.ru)
 * @brief
 * @version 0.1
 * @date 2024-05-15
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef TOOLS_COMMUNICATIONS_SERVER_CONNECTION_H
#define TOOLS_COMMUNICATIONS_SERVER_CONNECTION_H
#include <memory>

#include "connection.h"

class ServerConnection : public Connection {
 public:
  void establishConnection() override;
  bool checkNewIncomingConnection();
  std::shared_ptr<Connection> acceptIncomingConnection();
};
#endif  // TOOLS_COMMUNICATIONS_SERVER_CONNECTION_H