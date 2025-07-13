#ifndef CLIENT_H
#define CLIENT_H

#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

static int create_client_socket() {
    int client_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (client_fd == -1) {
        perror("Failed to create client socket");
        exit(EXIT_FAILURE);
    }
    return client_fd;
}

static int connect_to_server(int client_fd, const char* server_ip, int server_port) {
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(server_port);
    
    if (inet_pton(AF_INET, server_ip, &server_address.sin_addr) <= 0) {
        perror("Invalid address / Address not supported");
        close(client_fd);
        exit(EXIT_FAILURE);
    }

    if (connect(client_fd, (struct sockaddr*)&server_address, sizeof(server_address)) == -1) {
        perror("Failed to connect to server");
        close(client_fd);
        exit(EXIT_FAILURE);
    }

    return 0; 
}


static void send_message(int client_fd, const char* message) {
    ssize_t bytes_sent = send(client_fd, message, strlen(message), 0);
    if (bytes_sent == -1) {
        perror("Failed to send message");
        close(client_fd);
        exit(EXIT_FAILURE);
    }
    printf("Message sent successfully..\n");
}

#endif
