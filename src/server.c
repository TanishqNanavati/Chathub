#include "server.h"
#include <pthread.h>

void *receive_from_client(void *arg) {
    int client = *(int *)arg;
    char buff[1024];
    while (1) {
        ssize_t bytes_read = recv(client, buff, sizeof(buff) - 1, 0);
        if (bytes_read <= 0) {
            printf("Client disconnected.\n");
            break;
        }
        buff[bytes_read] = '\0';
        printf("\nClient Message: %s\n", buff);
    }
    close(client);
    exit(0);
    return NULL;
}

void *send_to_client(void *arg) {
    int client = *(int *)arg;
    char msg[1024];
    while (1) {
        printf("Enter the server message: ");
        fgets(msg, sizeof(msg), stdin);
        msg[strcspn(msg, "\n")] = 0;
        if (strcmp(msg, "exit") == 0) break;
        send(client, msg, strlen(msg), 0);
    }
    close(client);
    exit(0);
    return NULL;
}

void *handle_client(void *arg) {
    int client = *(int *)arg;
    free(arg);

    pthread_t recv_thread, send_thread;
    pthread_create(&recv_thread, NULL, receive_from_client, &client);
    pthread_create(&send_thread, NULL, send_to_client, &client);

    pthread_join(recv_thread, NULL);
    pthread_join(send_thread, NULL);

    return NULL;
}

int main() {
    int server_fd = create_server_socket();
    bind_server_socket(server_fd);
    listen_on_server_socket(server_fd);
    printf("Server is listening...\n");

    while (1) {
        int *client = malloc(sizeof(int));
        *client = accept_client_connection(server_fd);
        printf("Client connected.\n");

        pthread_t client_thread;
        pthread_create(&client_thread, NULL, handle_client, client);
        pthread_detach(client_thread);
    }

    close(server_fd);
    return 0;
}
