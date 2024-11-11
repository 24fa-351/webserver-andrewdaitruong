//PROGRESS: I know the multithreading should be near the echo after Accepting the connection. I just havent figured where to put it yet
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h>

#define PORT 80
#define LISTEN_BACKLOG 5

void handleConnection(int* socket_fd_ptr)
{
    int socket_fd = *socket_fd_ptr;
    free(socket_fd_ptr);

    char buffer[1024];
    int bytes_read = read(socket_fd, buffer, sizeof(buffer));
    printf("RECEIVED: %s\n", buffer);
    write(socket_fd, buffer, bytes_read);
}

int main(int argc, char* argv[]) 
{
    int port = PORT;

    //Create TCP Socket
    //AF_INET is an address family that is used to designate the type of addresses that your socket can communicate with
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) {
        perror("SOCKET FAILED TO CREATE\n");
        exit(EXIT_FAILURE);
    }

    //Accepting server addresses
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    server_address.sin_port = htons(PORT);
    
    //bind and listen
    int returnval;
    returnval = bind(
        server_socket, (struct sockaddr*)&
        server_address, sizeof(server_address));
    returnval = listen(server_socket, 5);

    //Listen for connections
    if(returnval = listen(server_socket, 5) < 0)
    {
        perror("LISTEN FAILED\n");
        close(server_socket);
        exit(EXIT_FAILURE);
    }
    printf("Server listening on port %d\n", port);

    //Client Address
    struct sockaddr_in client_address;
    socklen_t client_address_len = sizeof(client_address);  

    //Accept Connection/Reconnection
    while (1)
    {
        pthread_t t_id;
        int *client_socket_buffer = malloc(sizeof(int));

        *client_socket_buffer = accept(
            server_socket, 
            (struct sockaddr*)&client_address, 
            &client_address_len);
        
        if (*client_socket_buffer < 0)
        {
            perror("ACCEPT FAILURE\n");
            continue;
        }
        printf("CLIENT CONNECTED.\n");

        pthread_create(&t_id, NULL, (void* (*) (void*))handleConnection, (void*)client_socket_buffer);
    }
    close(server_socket);
    return 0;
}