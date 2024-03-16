#include <stdio.h>
#include <stdbool.h>
#include <winsock2.h>
#include <pthread.h>
#include "client.h"
#include "http.h"

enum protocol_types
{
    PROTOCOL_OTHER,
    PROTOCOL_HTTP,
    PROTOCOL_FTP,
    PROTOCOL_SMTP
};

struct protocol_data
{
    enum protocol_types type;
    const char *string;
};

static void *client_thread(void *arg);
static void client_clear(struct client_data *client);
static struct protocol_data detect_protocol_type(const char *request);

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

        struct protocol_data protocol = detect_protocol_type((const char *)client->request);
        printf("Socket : %d Protocol Type : %s\n", client->socket, protocol.string);

        if (protocol.type == PROTOCOL_HTTP)
        {
            handle_http_protocol((const char *)client->request, client->response, BUFFER_SIZE);
        }
        else if (protocol.type == PROTOCOL_FTP)
        {
            snprintf(client->response, BUFFER_SIZE, "501 Not Implemented\r\n");
        }
        else if (protocol.type == PROTOCOL_SMTP)
        {
            snprintf(client->response, BUFFER_SIZE, "501 Not Implemented\r\n");
        }
        else if (protocol.type == PROTOCOL_OTHER)
        {
            snprintf(client->response, BUFFER_SIZE, "501 Not Implemented\r\n");
        }
        else
        {
            snprintf(client->response, BUFFER_SIZE, "400 Bad Request\r\n");
            fprintf(stderr, "Socket : %d Unexpected protocol.", client->socket);
        }

        int send_status = send(client->socket, client->response, strlen(client->response), 0);
        if (send_status == SOCKET_ERROR)
        {
            fprintf(stderr, "Socket : %d Message send error.\n", client->socket);
            client->error = CLIENT_RUNTIME_ERROR;
            break;
        }

        printf("Socket : %d Message sent : %s\n", client->socket, client->response);
        break;
    }

    client_clear(client);
    printf("Socket : %d Client thread is closed.\n", client->socket);
    pthread_exit(NULL);
}

struct protocol_data detect_protocol_type(const char *request)
{
    struct protocol_data protocol_list[] = {
        {.type = PROTOCOL_OTHER, .string = "OTHER"},
        {.type = PROTOCOL_HTTP, .string = "HTTP"},
        {.type = PROTOCOL_FTP, .string = "FTP"},
        {.type = PROTOCOL_SMTP, .string = "SMTP"},
    };

    struct protocol_data protocol = protocol_list[0];

    for (size_t i = 0; i < sizeof(protocol_list) / sizeof(struct protocol_data); i++)
    {
        for (size_t j = 0; j < strlen(request); j++)
        {
            if (strncmp(protocol_list[i].string, request + j, strlen(protocol_list[i].string)) == 0)
            {
                protocol = protocol_list[i];
                break;
            }
        }
    }

    return protocol;
}
