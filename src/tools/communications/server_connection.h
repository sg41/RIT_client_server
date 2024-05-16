/**
 * @file server_connection.h
 * @author Sergey Gorbatenko (sg41@mail.ru)
 * @brief Server socket connection class header
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

/**
 * @brief A class representing a server connection that inherits from
 * Connection. This class provides methods to establish a connection, check for
 * new incoming connections, and accept incoming connections.
 */
class ServerConnection : public Connection {
 public:
  using Connection::Connection;
  void establishConnection() override;
  bool checkNewIncomingConnection();
  std::shared_ptr<Connection> acceptIncomingConnection();
};
#endif  // TOOLS_COMMUNICATIONS_SERVER_CONNECTION_H