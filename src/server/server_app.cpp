#include "server.h"

int main(int argc, char** argv) {
  int port = 8080;  // Default port
  bool log = true;
  if (argc > 1) {
    port = atoi(argv[1]);
  }

  if (argc > 2) {
    if (std::string(argv[2]) == "silent") {
      log = false;
    }
  }

  Server server(port, log);
  if (server.startServer()) {
    server.acceptConnections();
  }

  return 0;
}