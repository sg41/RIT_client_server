#include "server.h"

int main(int argc, char** argv) {
  int port = 8080;  // Default port
  if (argc > 1) {
    port = atoi(argv[1]);
  }

  Server server(port, true);
  if (server.startServer()) {
    server.acceptConnections();
  }

  return 0;
}