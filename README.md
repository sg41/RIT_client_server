# RIT Client-Server Application

## Introduction

This project is a work-in-progress implementation of a basic client-server application using TCP/IP communication in C++. The client can send messages to the server, which processes them and sends back responses.

## Features

**Client:**

* Connect to server via IP address and port.
* Send text messages to the server.
* Receive responses from the server.
* Automatic reconnection on connection loss.

**Server:**

* Handle multiple client connections simultaneously.
* Parse incoming messages and count the occurrence of different letters.
* Provide the number of current connections upon request.
* Enable communication between clients through the server.
* Offer shutdown functionality.

## Technical Details

* Programming Language: C++
* Build System: CMake and Make
* Unit Testing: Google Test
* Integration Testing: Python
* Code Coverage: gcovr
* Documentation: Doxygen

## Dependencies

* Linux operating system or MacOS
* C++ compiler
* CMake and Make
* Google Test
* Python3 and pytest

## Building and Running

**Building with Make:**

1. Clone the repository.
2. Navigate to the project directory.
3. Build the project: `make`

**Running the Server:**

1. Execute the server binary with the desired port: `./server <port> [silent]`
    * The `silent` option disables server console output.

**Running the Client:**

1. In a separate terminal, run the client binary: `./client <server_ip> <server_port>`

**Running Tests:**

1. Build and run automatic tests with `make test`
2. Check code coverage with `make coverage`
3. Check memory leaks with `make leaks`
4. Build and run manual tests with `make debug_run`
* Code is always statically analyzed with `cppcheck`

**Generating Documentation:**

1. Generate the documentation with `make docs`
2. The documentation will be generated in the `docs` directory.
3. Open the `index.html` file in the `docs/html` directory to view the documentation.

## Example Usage

**Basic Client-Server Communication:**

1. Start the server on port 8080: `./server 8080`
2. In another terminal, connect a client: `./client 127.0.0.1 8080`
3. Type a message in the client, such as "Hello, world!".
4. The server will respond with the count of each letter:
```
H |1
e |1
l |3
o |2
, |1
  |1
w |1
r |1
d |1
! |1
```

**Client-to-Client Communication:**

1. Connect two clients to the server.
2. In one client, send a message to the other client using the format: `send <client_id> <message>`
    * For example: `send <client_2> Hi there!`
3. The other client will receive the message: `client_1: Hi there!`
* Do not forget to use the `<` and `>` symbols to enclose the client ID.

**Server Commands:**

* `show list`: Display a list of connected clients with their IDs.
* `show number`: Show the number of currently connected clients.
* `shutdown`: Shut down the server gracefully.

## Future Development

* Implement command line editing and history for the client.
* Add support for file transfer between clients.
* Enhance security features with encryption and authentication.
* Explore other communication protocols like UDP.

## Contributing

Contributions are welcome! Please feel free to submit pull requests or open issues for bug reports and feature suggestions.

