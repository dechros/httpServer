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

static void *client_thread(void *arg);

void client_init(struct client *client_data)
{
    client_data->request = calloc(BUFFER_SIZE, sizeof(char));
    if (client_data->request == NULL)
    {
        fprintf(stderr, "Socket : %d Request buffer memory error.\n", client_data->socket);
        client_data->error = CLIENT_INIT_ERROR;
        return;
    }

    client_data->response = calloc(BUFFER_SIZE, sizeof(char));
    if (client_data->response == NULL)
    {
        fprintf(stderr, "Socket : %d Response buffer memory error.\n", client_data->socket);
        client_data->error = CLIENT_INIT_ERROR;
        free(client_data->request);
        return;
    }

    FD_ZERO(&client_data->fd);
    FD_SET(client_data->socket, &client_data->fd);
    client_data->timeout.tv_usec = 1000;
    client_data->init = true;
    client_data->stop = false;
}

void client_run(struct client *client_data)
{
    if (client_data->init == false)
    {
        fprintf(stderr, "Socket : %d Client is not initialized.\n", client_data->socket);
        client_data->error = CLIENT_INIT_ERROR;
        return;
    }

    int thread_status = pthread_create(&client_data->thread, NULL, &client_thread, (void *)client_data);
    if (thread_status != 0)
    {
        fprintf(stderr, "Socket : %d Error creating client thread. Client socket : %d\n", client_data->socket);
        client_data->error = CLIENT_RUNTIME_ERROR;
        return;
    }
}

void client_stop(struct client *client_data)
{
    client_data->stop = true;
    int join_status = pthread_join(client_data->thread, NULL);
    if (join_status != 0)
    {
        fprintf(stderr, "Socket : %d Error stopping the client thread.\n");
        client_data->error = CLIENT_RUNTIME_ERROR;
    }

    closesocket(client_data->socket);
    FD_ZERO(&client_data->fd);
    free(client_data->request);
    free(client_data->response);
    client_data->init = false;
}

void *client_thread(void *arg)
{
    struct client *client_data = (struct client *)arg;
    printf("Socket : %d Client thread is created.\n", client_data->socket);

    while (client_data->stop == false)
    {
        fd_set temp_fd = client_data->fd;
        int socket_count = select(0, &temp_fd, NULL, NULL, &client_data->timeout);
        if (socket_count == SOCKET_ERROR)
        {
            fprintf(stderr, "Socket : %d Select error.\n", client_data->socket);
            client_data->error = CLIENT_RUNTIME_ERROR;
            break;
        }
        if (socket_count == 0)
        {
            continue;
        }

        int bytes_received = recv(client_data->socket, client_data->request, BUFFER_SIZE - 1, 0);
        if (bytes_received == SOCKET_ERROR)
        {
            fprintf(stderr, "Socket : %d Message receive error.\n", client_data->socket);
            client_data->error = CLIENT_RUNTIME_ERROR;
            break;
        }
        if (bytes_received == 0)
        {
            fprintf(stderr, "Socket : %d Client is disconnected.\n", client_data->socket);
            client_data->error = CLIENT_NO_ERROR;
            break;
        }

        client_data->request[bytes_received] = '\0';
        printf("Socket : %d Bytes received : %d\n", client_data->socket, bytes_received);
        printf("Socket : %d Message received : %s\n", client_data->socket, client_data->request);

        strcpy(client_data->response, client_data->request);
        int send_status = send(client_data->socket, client_data->response, strlen(client_data->response), 0);
        if (send_status == SOCKET_ERROR)
        {
            fprintf(stderr, "Socket : %d Message send error.\n", client_data->socket);
            client_data->error = CLIENT_RUNTIME_ERROR;
            break;
        }

        printf("Socket : %d Message sent : %s\n", client_data->socket, client_data->response);
    }

    printf("Socket : %d Client thread is closed.\n", client_data->socket);
    pthread_exit(NULL);
}

enum request_types detect_request_type()
{
}
