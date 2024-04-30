#ifndef SERVER_CLIENT_HANDLER_H
#define SERVER_CLIENT_HANDLER_H
#include <unordered_map>

#include "parser.h"
class Server;

class ClientHandler {
 public:
  ClientHandler(int socket, const std::string& id, Server* server);
  ~ClientHandler();
  void handleClient();
  std::string getClientID() const;
  const Server* getServer() const;

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

 private:
  int client_socket = -1;  // To store the client socket
  std::string client_id;   // To store the unique client ID
  Server* server;          // Reference to the server for message routing
};
#endif  // SERVER_CLIENT_HANDLER_H