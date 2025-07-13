#include "server.h"
#include <pthread.h>


void * handle_client(void * arg){
    int client = *(int *)arg;
    free(arg);

    char buff[1024];
    while(1){
        ssize_t bytes_read = recv(client,buff,sizeof(buff)-1,0);

        if(bytes_read<=0){
            printf("Client Disconnected.\n");
            break;
        }

        buff[bytes_read]='\0';
        printf("Client Message: %s\n",buff);

        char bytes_send[1024];
        printf("Enter the server message: ");
        fgets(bytes_send,sizeof(bytes_send),stdin);
        bytes_send[strcspn(bytes_send,"\n")]=0;
        send(client,bytes_send,strlen(bytes_send),0);
    }

    close(client);
    return NULL;

}

int main(){
    int server_fd = create_server_socket();
    bind_server_socket(server_fd);
    listen_on_server_socket(server_fd);
    printf("Server is listening...\n");

    while (1) {
        int* client = malloc(sizeof(int));
        *client = accept_client_connection(server_fd);
        printf("Client connected.\n");

        pthread_t client_thread;
        pthread_create(&client_thread, NULL, handle_client, client);
        pthread_detach(client_thread); 
    }

    close(server_fd);
    return 0;
}


// int main() {
//     int server_fd=create_server_socket();

//     bind_server_socket(server_fd);
//     listen_on_server_socket(server_fd);

//     printf("Server is listening..\n");

//     int client=accept_client_connection(server_fd);
//     printf("Client Connected\n");

//     char buff[1024]={0};
//     ssize_t bytes_read=recv(client,buff,sizeof(buff)-1,0);
//     if(bytes_read>0){
//         buff[bytes_read]='\0';
//         printf("Message from Client : %s\n",buff);
//     }else{
//         printf("Failed to receive message from client.\n");
//         close(client);
//         close(server_fd);
//         return 1;
//     }

//     char res[1024];
//     printf("Enter the message to sent to client : ");
//     fgets(res,sizeof(res),stdin);
//     res[strcspn(res,"\n")]=0;

//     ssize_t sent_msg=send(client,res,strlen(res),0);
//     if(sent_msg==-1){
//         perror("Failed to sent response to client");
//     }else{
//         printf("Message sent successfully to client.\n");
//     }
//     close(client);
//     close(server_fd);

//     return 0;
// }
