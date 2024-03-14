#include <stdio.h>
#include <stdbool.h>
#include <winsock2.h>
#include <pthread.h>
#include "client.h"

#define BUFFER_SIZE 4096

enum request_types
{
    HTTP,
    FTP,
    SMTP,
    OTHER
};

void *client_thread(void *arg)
{
    struct client *client_data = (struct client *)arg;
    char *request = calloc(BUFFER_SIZE, sizeof(char));
    char *response = calloc(BUFFER_SIZE, sizeof(char));
    enum client_errors error = CLIENT_NO_ERROR;
    client_data->init = true;

    printf("Client thread is created. Socket : %d\n", client_data->socket);

    while (client_data->stop == false)
    {
        FD_ZERO(&client_data->fd);
        FD_SET(client_data->socket, &client_data->fd);
        struct timeval timeout = {0};
        timeout.tv_usec = 1000;
        fd_set temp_fd = client_data->fd;

        int socket_count = select(0, &temp_fd, NULL, NULL, &timeout);
        if (socket_count == SOCKET_ERROR)
        {
            fprintf(stderr, "Select error. Socket : %d\n", client_data->socket);
            error = CLIENT_RECEIVE_ERROR;
            break;
        }
        if (socket_count == 0)
        {
            continue;
        }

        int bytes_received = recv(client_data->socket, request, BUFFER_SIZE - 1, 0);
        if (bytes_received == SOCKET_ERROR)
        {
            fprintf(stderr, "Message receive error. Socket : %d\n", client_data->socket);
            error = CLIENT_RECEIVE_ERROR;
            break;
        }
        if (bytes_received == 0)
        {
            fprintf(stderr, "Client is disconnected. Socket : %d\n", client_data->socket);
            break;
        }

        request[bytes_received] = '\0';
        printf("Bytes received : %d\n", bytes_received);
        printf("Message received : %s\n", request);

        strcpy(response, request);
        int send_status = send(client_data->socket, response, strlen(response), 0);
        if (send_status == SOCKET_ERROR)
        {
            fprintf(stderr, "Message send error. Socket : %d\n", client_data->socket);
            error = CLIENT_SEND_ERROR;
            break;
        }

        printf("Message sent : %s\n", response);
    }

    free(request);
    free(response);
    client_data->init = false;
    closesocket(client_data->socket);
    printf("Client thread is closed. Socket : %d\n", client_data->socket);
    pthread_exit((void *)error);
}

enum request_types detect_request_type()
{
}
