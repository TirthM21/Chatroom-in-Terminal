//Made by:Tirth Mehta
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define MAX_CLIENTS 10
#define BUFFER_SIZE 1024

int main() {
    int server_socket, new_socket, client_sockets[MAX_CLIENTS], max_clients = MAX_CLIENTS, activity, i, valread, sd;
    struct sockaddr_in server_address;
    char buffer[BUFFER_SIZE];
    fd_set readfds;

    for (i = 0; i < max_clients; i++) {
        client_sockets[i] = 0;
    }

    if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(PORT);

    if (bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_socket, MAX_CLIENTS) < 0) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    printf("Chatroom server started on port %d\n", PORT);

    while (1) {
        FD_ZERO(&readfds);
        FD_SET(server_socket, &readfds);
        int max_sd = server_socket;

        for (i = 0; i < max_clients; i++) {
            sd = client_sockets[i];
            if (sd > 0) {
                FD_SET(sd, &readfds);
            }
            if (sd > max_sd) {
                max_sd = sd;
            }
        }

        activity = select(max_sd + 1, &readfds, NULL, NULL, NULL);

        if (activity < 0) {
            perror("Select error");
            exit(EXIT_FAILURE);
        }

        if (FD_ISSET(server_socket, &readfds)) {
            if ((new_socket = accept(server_socket, (struct sockaddr *)&server_address, (socklen_t *)&server_address)) < 0) {
                perror("Accept failed");
                exit(EXIT_FAILURE);
            }

            printf("New connection, socket fd is %d, IP is : %s, Port : %d\n", new_socket, inet_ntoa(server_address.sin_addr), ntohs(server_address.sin_port));

            for (i = 0; i < max_clients; i++) {
                if (client_sockets[i] == 0) {
                    client_sockets[i] = new_socket;
                    break;
                }
            }
        }

        for (i = 0; i < max_clients; i++) {
            sd = client_sockets[i];
            if (FD_ISSET(sd, &readfds)) {
                valread = read(sd, buffer, BUFFER_SIZE);
                if (valread == 0) {
                    getpeername(sd, (struct sockaddr *)&server_address, (socklen_t *)&server_address);
                    printf("Host disconnected, IP %s, Port %d\n", inet_ntoa(server_address.sin_addr), ntohs(server_address.sin_port));
                    close(sd);
                    client_sockets[i] = 0;
                } else {
                    buffer[valread] = '\0';
                    printf("Client %d: %s\n", sd, buffer);

                    // Broadcast the message to all other clients
                    for (int j = 0; j < max_clients; j++) {
                        if (client_sockets[j] != 0 && client_sockets[j] != sd) {
                            send(client_sockets[j], buffer, strlen(buffer), 0);
                        }
                    }
                }
            }
        }
    }

    return 0;
}
