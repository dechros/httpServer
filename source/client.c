#include <stdio.h>
#include <stdbool.h>
#include <winsock2.h>
#include <pthread.h>
#include "client.h"

enum request_types
{
    HTTP,
    FTP,
    SMTP,
    OTHER
};

static void *client_thread(void *arg);
static void client_clear(struct client_data *client);

void client_init(struct client_data *client)
{
    FD_ZERO(&client->fd);
    FD_SET(client->socket, &client->fd);
    client->timeout.tv_usec = 1000;
    client->init = true;
    client->stop = false;
    client->error = CLIENT_NO_ERROR;
}

void client_run(struct client_data *client)
{
    if (client->init == false)
    {
        fprintf(stderr, "Socket : %d Client is not initialized.\n", client->socket);
        client->error = CLIENT_INIT_ERROR;
        return;
    }

    int thread_status = pthread_create(&client->thread, NULL, &client_thread, (void *)client);
    if (thread_status != 0)
    {
        fprintf(stderr, "Socket : %d Error creating client thread. Client socket : %d\n", client->socket);
        client->error = CLIENT_RUNTIME_ERROR;
        return;
    }
}

void client_stop(struct client_data *client)
{
    client->stop = true;
    int join_status = pthread_join(client->thread, NULL);
    if (join_status != 0)
    {
        fprintf(stderr, "Socket : %d Error stopping the client thread.\n", client->socket);
        client->error = CLIENT_RUNTIME_ERROR;
    }

    client_clear(client);
}

void client_clear(struct client_data *client)
{
    closesocket(client->socket);
    FD_ZERO(&client->fd);
    client->init = false;
}

void *client_thread(void *arg)
{
    struct client_data *client = (struct client_data *)arg;
    printf("Socket : %d Client thread is created.\n", client->socket);

    while (client->stop == false)
    {
        fd_set temp_fd = client->fd;
        int socket_count = select(0, &temp_fd, NULL, NULL, &client->timeout);
        if (socket_count == SOCKET_ERROR)
        {
            fprintf(stderr, "Socket : %d Select error.\n", client->socket);
            client->error = CLIENT_RUNTIME_ERROR;
            break;
        }
        if (socket_count == 0)
        {
            continue;
        }

        int bytes_received = recv(client->socket, client->request, BUFFER_SIZE - 1, 0);
        if (bytes_received == SOCKET_ERROR)
        {
            fprintf(stderr, "Socket : %d Message receive error.\n", client->socket);
            client->error = CLIENT_RUNTIME_ERROR;
            break;
        }
        if (bytes_received == 0)
        {
            fprintf(stderr, "Socket : %d Client is disconnected.\n", client->socket);
            client->error = CLIENT_DISCONNECTED;
            break;
        }

        client->request[bytes_received] = '\0';
        printf("Socket : %d Bytes received : %d\n", client->socket, bytes_received);
        printf("Socket : %d Message received : %s\n", client->socket, client->request);

        strcpy(client->response, client->request);
        int send_status = send(client->socket, client->response, strlen(client->response), 0);
        if (send_status == SOCKET_ERROR)
        {
            fprintf(stderr, "Socket : %d Message send error.\n", client->socket);
            client->error = CLIENT_RUNTIME_ERROR;
            break;
        }

        printf("Socket : %d Message sent : %s\n", client->socket, client->response);
    }

    client_clear(client);
    printf("Socket : %d Client thread is closed.\n", client->socket);
    pthread_exit(NULL);
}

enum request_types detect_request_type()
{
}
