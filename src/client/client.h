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
   * Connects to the server. No retries are done in case of failure.
   *
   * @return true if the connection is successful, false otherwise.
   *
   * @throws None
   */
  bool connectToServer();
  /**
   * Checks if there is any data available to be read from the socket.
   *
   * @return true if there is data available, false otherwise.
   *
   * @throws None
   */
  bool checkHaveMessage();
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
  ~Client();

 private:
  std::string server_ip_;
  int server_port_;
  int sockfd_ = -1;
  bool log_ = false;
};

#endif  // CLIENT_CLIENT_H