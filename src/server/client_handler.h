/**
 * @file client_handler.h
 * @author Sergey Gorbatenko (sg41@mail.ru)
 * @brief Client handler class for Server class
 * @version 0.1
 * @date 2024-05-01
 *
 * @copyright Copyright (c) 2024
 *
 */
#ifndef SERVER_CLIENT_HANDLER_H
#define SERVER_CLIENT_HANDLER_H
#include <memory>
#include <unordered_map>

#include "connection.h"
#include "parser.h"
class Server;

/**
 * The `ClientHandler` class is responsible for handling client interactions
 * with the server. It maintains a connection with a specific client and
 * provides methods to receive and send messages to/from the client.
 *
 * The class takes in the client's socket, unique client ID, and a reference to
 * the server. It provides various message handling methods, such as
 * `processMessage()`, `sendMessage()`, and `sendMessageToClient()`, which
 * parse incoming messages and execute corresponding commands.
 *
 * The class also provides methods to count letter occurrences in a message
 * (`countLetters()`) and display the number of connected clients or a list of
 * their IDs (`showConnections()`).
 *
 * Lastly, it provides a method to shut down the server (`shutdownServer()`)
 * when the client sends a specific command.
 *
 * This class is designed to be used in conjunction with the `Server` class,
 * and should not be used standalone.
 *
 */
class ClientHandler {
 public:
  ClientHandler(std::shared_ptr<Connection>&& connection, const std::string& id,
                Server* server);

  void handleClient();

  std::string receiveMessage();
  /**
   * Sends a message to the current client.
   *
   * @param message The message to send.
   *
   * @return "Ok" if the message was successfully sent, "Error" otherwise.
   *
   * @throws None
   */
  std::string sendMessage(const std::string& message);

  /**
   * Processes the incoming message from a client by parsing it and executing
   * the corresponding command.
   *
   * @param message The message received from the client.
   *
   * @return The response message based on the executed command.
   *
   * @throws None
   */
  std::string processMessage(const std::string& message);

  /**
   * Handles sending a message from current client to another client connected
   * to server, based on the input message.
   *
   * @param message The message in "<client_id> message" format.
   *
   * @return Returns "Message sent" if the message is successfully routed, an
   * empty string if the message is sent to itself, "Error" if routing fails, or
   * "Invalid command format" if the message format is incorrect.
   *
   * @throws None
   */
  std::string sendMessageToClient(const std::string& message);
  /**
   * Counts the occurrences of each letter in the input message and generates a
   * formatted response.
   *
   * @param message The input message to count letter occurrences from.
   *
   * @return A formatted response showing each unique letter in the message and
   * its count.
   *
   * @throws None
   */
  std::string countLetters(const std::string& message);
  /**
   * Displays the number of connected clients or a list of their IDs based on
   * the command received in the message.
   *
   * @param message The message containing the command.
   *
   * @return The number of connected clients as a string if the command is
   * "count", a list of client IDs separated by newlines if the command is
   * "list", or an error message if the command format is invalid.
   *
   * @throws None.
   */
  std::string showConnections(const std::string& message);
  /**
   * Shuts down the server.
   *
   * @param message The message passed to the function (unused).
   *
   * @return A string indicating that the server has been shut down.
   *
   * @throws None
   */
  std::string shutdownServer(const std::string& message);
  void disconnect();

 private:
  std::shared_ptr<Connection> connection_;
  std::string client_id;  // To store the unique client ID
  Server* server;         // Reference to the server for message routing
};
#endif  // SERVER_CLIENT_HANDLER_H