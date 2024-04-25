
#include <unordered_map>
class ClientHandler {
 private:
  int client_socket;

 public:
  ClientHandler(int socket) : client_socket(socket) {}

  void handleClient() {
    while (true) {
      std::string message = receiveMessage();
      if (message.empty()) {
        break;  // Client disconnected
      }

      // Process message and send response
      std::cout << "Received from client: " << message << std::endl;
      std::string response = processMessage(message);
      //   sendMessage("Response from server");
      sendMessage(response);
    }

    close(client_socket);
    std::cout << "Client disconnected." << std::endl;
  }

 private:
  std::string receiveMessage() {
    char buffer[1024] = {0};
    int bytes_received = recv(client_socket, buffer, 1024, 0);
    if (bytes_received <= 0) {
      return "";
    }
    return std::string(buffer, bytes_received);
  }

  void sendMessage(const std::string& message) {
    send(client_socket, message.c_str(), message.length(), 0);
  }

  std::string countLetters(const std::string& message) {
    std::unordered_map<char, int> letter_counts;
    for (char c : message) {
      letter_counts[c]++;
    }
    std::string response = "Message |" + message + "\n";
    // for (auto& pair : letter_counts) {
    //   response += std::string(1, pair.first) + "       |" +
    //               std::to_string(pair.second) + "\n";
    // }
    for (auto letter : message) {
      if (letter_counts[letter] != 0) {
        response += std::string(1, letter) + "       |" +
                    std::to_string(letter_counts[letter]) + "\n";
        letter_counts[letter] = 0;
      }
    }
    return response;
  }
  std::string processMessage(const std::string& message) {
    // TODO: Implement message processing logic here
    // if(message == "communicate")
    auto response = countLetters(message);

    return response;
  }
};