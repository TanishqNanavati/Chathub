#ifndef SERVER_H
#define SERVER_H

#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

static int create_server_socket() {
    int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd == -1) {
        perror("Failed to create socket");
        close(socket_fd);
        exit(EXIT_FAILURE);
    }

    return socket_fd;
}

static int bind_server_socket(int socket_fd){
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(8080);
    server_address.sin_addr.s_addr = INADDR_ANY;

    int socket_bind = bind(socket_fd, (struct sockaddr*)&server_address, sizeof(server_address));

    if (socket_bind == -1) {
        perror("Failed to bind socket");
        close(socket_fd);
        exit(EXIT_FAILURE);
    }

    return socket_bind;
}
static int listen_on_server_socket(int socket_fd){
    int listen_socket=listen(socket_fd,5);
    if(listen_socket==-1){
        perror("Failed to listen on socket");
        close(socket_fd);
        exit(EXIT_FAILURE);
    }
    return listen_socket;
}

static int accept_client_connection(int socket_fd){
    int client_socket=accept(socket_fd,NULL,NULL);
    if(client_socket==-1){
        perror("Failed to accept connection on socket");

        exit(EXIT_FAILURE);  
    }
    return client_socket;
}

static void handle_client_soc(int client_socket) {
    char buff[1024];

    ssize_t bytes_read = read(client_socket, buff, sizeof(buff) - 1);
    if (bytes_read > 0) {
        buff[bytes_read] = '\0';
        printf("Message from client: %s\n", buff);

        const char *res = "Received on Server";
        write(client_socket, res, strlen(res));
    } else if (bytes_read == 0) {
        printf("Client disconnected.\n");
    } else {
        perror("Error receiving data");
    }

    close(client_socket);
}


#endif 