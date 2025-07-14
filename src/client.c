#include "client.h"
#include <pthread.h>

int client_fd;

void *receive_msg(void *arg) {
    char buff[1024];
    while (1) {
        ssize_t bytes_read = recv(client_fd, buff, sizeof(buff) - 1, 0);
        if (bytes_read <= 0) {
            printf("Server disconnected.\n");
            break;
        }
        buff[bytes_read] = '\0';
        printf("\nServer Message: %s\n", buff);
    }
    close(client_fd);
    exit(0);
    return NULL;
}

void *send_reply(void *arg) {
    char msg[1024];
    while (1) {
        printf("Enter the Client Message: ");
        fgets(msg, sizeof(msg), stdin);
        msg[strcspn(msg, "\n")] = 0;
        if (strcmp(msg, "exit") == 0) break;
        send_message(client_fd, msg);
    }
    close(client_fd);
    exit(0);
    return NULL;
}

int main() {
    client_fd = create_client_socket();
    connect_to_server(client_fd, "127.0.0.1", 8080);

    pthread_t rec_msg, send_msg;
    pthread_create(&rec_msg, NULL, receive_msg, NULL);
    pthread_create(&send_msg, NULL, send_reply, NULL);

    pthread_join(rec_msg, NULL);
    pthread_join(send_msg, NULL);
    return 0;
}
