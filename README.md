# Chatroom-in-Terminal
# Chatroom Application - C Programming

This is a simple chatroom application implemented in C programming language. The application consists of two parts: a server-side program and a client-side program. The server-side program allows multiple clients to connect and chat with each other through the server.

## Features

- Real-time messaging: Users can send and receive messages in real-time within the chatroom.
- Multiple clients: The server supports multiple clients connecting simultaneously and exchanging messages.
- Simple user interface: The client-side program provides a simple and intuitive command-line interface for typing and sending messages.

## How to Use

1. Clone the repository to your local machine.
2. Compile the server-side code using the following command:
3.  Compile the client-side code using the following command:
gcc client.c -o client

4. Run the server-side program:
./server
The server will start running and listening for incoming connections on port 8080.

5. Run the client-side program on separate terminals for different clients:
./client
The client will connect to the server at the address "127.0.0.1" and port 8080.

6. Start typing your messages in the client-side terminal and press Enter to send them to the chatroom.

## Dependencies

The application uses standard C libraries for socket programming, string manipulation, and user input handling. There are no additional external dependencies.
