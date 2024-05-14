/**
 * @file client.h
 * @author Sergey Gorbatenko (sg41@mail.ru)
 * @brief Client class declaration
 * @version 0.1
 * @date 2024-05-01
 *
 * @copyright Copyright (c) 2024
 *
 */
#ifndef CLIENT_CLIENT_H
#define CLIENT_CLIENT_H
#include <string>

const int kMaxRetries = 3;
const int kRetryTimeout = 3;
const int kBufferSize = 1024;

/**
 * @brief The Client class represents a client that connects to a server.
 *
 * It provides methods for connecting to the server, sending and receiving
 * messages, and handling errors.
 */
class Client {
 public:
  /**
   * @brief Constructor for Client.
   *
   * @param ip The IP address of the server to connect to.
   * @param port The port of the server to connect to.
   * @param log Flag indicating whether logging is enabled.
   */
  Client(const std::string& ip, int port, bool log = false);

  /**
   * Rule of 5 implementation
   */
  Client(const Client&) = delete;
  Client& operator=(const Client&) = delete;
  Client(Client&&) = delete;
  Client& operator=(Client&&) = delete;
  ~Client();

  /**
   * Connects to the server. No retries are done in case of failure.
   *
   * @return true if the connection is successful, false otherwise.
   *
   * @throws None
   */
  bool connectToServer();
  /**
   * Checks if there is any data available to be read from the socket or other
   * file descriptor.
   *
   * @return true if there is data available, false otherwise.
   *
   * @throws None
   */
  bool checkHaveMessage(int fd);
  /**
   * Sends a message over the socket connection.
   *
   * @param message The message to be sent.
   *
   * @return true if the message was sent successfully, false otherwise.
   *
   * @throws None
   */
  bool sendMessage(const std::string& message);
  /**
   * Receives a message from the server.
   *
   * @param message The reference to a string where the received message will be
   * stored.
   *
   * @return true if the message was received successfully, false otherwise.
   *
   * @throws None
   */
  bool receiveMessage(std::string& message);
  /**
   * Try to reconnect to the server up to kMaxRetries times.
   *
   * @return true if the reconnection is successful, false otherwise.
   *
   * @throws None
   */
  bool reconnect();
  auto getSocketFD() const { return sockfd_; }

 private:
  std::string server_ip_;
  int server_port_;
  int sockfd_ = -1;
  bool log_ = false;
};

#endif  // CLIENT_CLIENT_H