/**
 * @file client_app.h
 * @author Sergey Gorbatenko (sg41@mail.ru)
 * @brief Client app class header
 * @version 0.1
 * @date 2024-05-01
 *
 * @copyright Copyright (c) 2024
 *
 */
#ifndef CLIENT_CLIENT_APP_H
#define CLIENT_CLIENT_APP_H
#include <future>
#include <map>

#include "client.h"

/**
 * @brief The ClientApp class represents a client application that connects to a
 * server and handles server messages and user inputs.
 *
 * It provides methods for connecting to the server, running the client
 * application loop, and interacting with the server.
 */
class ClientApp {
 public:
  enum class Event { kUserInput, kServerMessage, kNoEvent };
  ClientApp(const std::string& server_ip, int server_port, bool log = false);
  ~ClientApp() = default;

  /**
   * Connects to the server.
   *
   * @return true if the connection is successful, false otherwise.
   *
   * @throws None
   */
  bool connectToServer();
  /**
   * Runs the client application loop, handling server messages and user inputs.
   *
   * @return The error code after running the loop.
   *
   * @throws None
   */
  int run();

 private:
  /**
   * Event loop function for the ClientApp class.
   *
   * This function continuously checks for server messages and user inputs. If a
   * server message is received, it returns Event::kServerMessage. If user input
   * is available, it returns Event::kUserInput.
   *
   * @return The Event::kServerMessage if a server message is received,
   * Event::kUserInput if user input is available, or blocks indefinitely if
   * neither is the case until either the client or server disconnects or
   * this->is_running_ is set to false.
   *
   * @throws None
   */
  Event eventLoop();
  /**
   * Receives a message from the server. Tryes to reconnect if necessary.
   *
   * @param response The reference to a string where the received message will
   * be stored.
   *
   * @return true if the message was received successfully, false otherwise.
   *
   * @throws None
   */
  bool receiveServerMessage(std::string& response);
  /**
   * Sends the this->message_ to the server and receives a response.
   *
   * @param response a reference to a string where the server's response will be
   * stored
   *
   * @return true if the message was sent successfully and a response was
   * received, false otherwise
   *
   * @throws None
   */
  bool talkToServer(std::string& response);
  void shutdownServer();
  void performExit();
  void showHelp();

 private:
  Client client_;
  std::string message_{};
  std::future<std::string> input_future_;
  bool running_ = false;
  // Map of valid commands served by client side
  std::map<std::string, void (ClientApp::*)()> valid_commands_ = {
      {"shutdown", &ClientApp::shutdownServer},
      {"exit", &ClientApp::performExit},
      {"help", &ClientApp::showHelp},
      {"quit", &ClientApp::performExit},
      {"!q", &ClientApp::performExit},
      {"?", &ClientApp::showHelp}};
};
#endif  // CLIENT_CLIENT_APP_H