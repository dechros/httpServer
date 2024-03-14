#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include "server.h"
#include "client.h"

#define MAX_CLIENT 2

static void *server_thread(void *arg);

enum server_errors server_init(struct tcp_server *tcp, const int port)
{
    tcp->server.address.sin_family = AF_INET;         /* IPV4 */
    tcp->server.address.sin_addr.s_addr = INADDR_ANY; /* Listen on every interface */
    tcp->server.address.sin_port = htons(port);

    int wsa_status = WSAStartup(MAKEWORD(2, 2), &tcp->server.wsa);
    if (wsa_status != 0)
    {
        int wsa_error = WSAGetLastError();
        fprintf(stderr, "Port : %d Failed to initialize Windows socket data. Error : %d\n", port, wsa_error);
        return SERVER_INIT_ERROR;
    }

    tcp->server.socket = socket(AF_INET, SOCK_STREAM, 0);
    if (tcp->server.socket == INVALID_SOCKET)
    {
        int wsa_error = WSAGetLastError();
        fprintf(stderr, "Port : %d Failed to create server socket. Error : %d\n", port, wsa_error);
        return SERVER_INIT_ERROR;
    }

    int bind_status = bind(tcp->server.socket, (struct sockaddr *)&tcp->server.address, sizeof(tcp->server.address));
    if (bind_status == SOCKET_ERROR)
    {
        int wsa_error = WSAGetLastError();
        fprintf(stderr, "Port : %d Failed to bind server socket to the server address. Error : %d\n", port, wsa_error);
        return SERVER_INIT_ERROR;
    }

    int listen_status = listen(tcp->server.socket, SOMAXCONN);
    if (listen_status == SOCKET_ERROR)
    {
        int wsa_error = WSAGetLastError();
        fprintf(stderr, "Port : %d Failed to listen for incoming connections. Error : %d\n", port, wsa_error);
        return SERVER_INIT_ERROR;
    }

    tcp->clients = calloc(MAX_CLIENT, sizeof(struct client));
    if (tcp->clients == NULL)
    {
        fprintf(stderr, "Port : %d Failed to init clients memory. %d\n", port);
        return SERVER_INIT_ERROR;
    }

    tcp->server.init = true;
    return SERVER_NO_ERROR;
}

enum server_errors server_run(struct tcp_server *tcp)
{
    const int port = ntohs(tcp->server.address.sin_port);

    if (tcp->server.init == false)
    {
        printf("Port : %d TCP server is not initialized.\n", port);
        return SERVER_INIT_ERROR;
    }

    int thread_status = pthread_create(&tcp->server.thread, NULL, &server_thread, (void *)tcp);
    if (thread_status != 0)
    {
        fprintf(stderr, "Port : %d Error creating server thread.", port);
        return SERVER_RUNTIME_ERROR;
    }

    return SERVER_NO_ERROR;
}

enum server_errors server_stop(struct tcp_server *tcp)
{
    tcp->server.stop = true;
    enum server_errors server_error = SERVER_NO_ERROR;
    int join_status = pthread_join(tcp->server.thread, (void **)&server_error);
    if (join_status != 0)
    {
        fprintf(stderr, "Port : %d Error stopping the server task.\n");
        server_error = SERVER_RUNTIME_ERROR;
    }

    free(tcp->clients);
    return server_error;
}

void *server_thread(void *arg)
{
    struct tcp_server *tcp = (struct tcp_server *)arg;
    const int port = ntohs(tcp->server.address.sin_port);
    FD_ZERO(&tcp->server.fd);
    FD_SET(tcp->server.socket, &tcp->server.fd);
    enum server_errors server_error = SERVER_NO_ERROR;
    printf("Port : %d TCP server started listening.\n", port);

    while (tcp->server.stop == false)
    {
        struct timeval timeout = {0};
        timeout.tv_usec = 1000;
        fd_set temp_fd = tcp->server.fd;

        int socket_count = select(tcp->server.socket + 1, &temp_fd, NULL, NULL, &timeout);
        if (socket_count == SOCKET_ERROR)
        {
            int wsa_error = WSAGetLastError();
            fprintf(stderr, "Port : %d Select failed. Error : %d\n", port, wsa_error);
            server_error = SERVER_RUNTIME_ERROR;
            break;
        }
        if (socket_count == 0)
        {
            continue;
        }

        struct client *new_client = NULL;
        for (size_t i = 0; i < MAX_CLIENT; i++)
        {
            if (tcp->clients[i].init == false)
            {
                new_client = &tcp->clients[i];
                break;
            }
        }

        if (new_client == NULL)
        {
            continue;
        }

        size_t client_addr_len = sizeof(new_client->address);
        new_client->socket = accept(tcp->server.socket, (struct sockaddr *)&new_client->address, (int *)&client_addr_len);

        if (new_client->socket == INVALID_SOCKET)
        {
            fprintf(stderr, "Port : %d Failed to accept client connection. Client socket : %d\n", port, new_client->socket);
            continue;
        }

        printf("Port : %d Client is connected. Client socket : %d\n", port, new_client->socket);

        int thread_status = pthread_create(&new_client->thread, NULL, &client_thread, (void *)new_client);
        if (thread_status != 0)
        {
            fprintf(stderr, "Port : %d Error creating client handle thread. Client socket : %d\n", port, new_client->socket);
            server_error = SERVER_RUNTIME_ERROR;
            break;
        }
    }

    for (size_t i = 0; i < MAX_CLIENT; i++)
    {
        if (tcp->clients[i].init == true)
        {
            tcp->clients[i].stop = true;
            enum client_errors client_error = CLIENT_NO_ERROR;
            int join_status = pthread_join(tcp->clients[i].thread, (void **)&client_error);
            if (join_status != 0)
            {
                fprintf(stderr, "Port : %d Error stopping the client task. Client socket : %d\n", port, tcp->clients[i].socket);
                server_error = SERVER_RUNTIME_ERROR;
            }
            if (client_error != CLIENT_NO_ERROR)
            {
                server_error = SERVER_RUNTIME_ERROR;
            }
        }
    }

    printf("Port : %d TCP server stopped.\n", port);
    closesocket(tcp->server.socket);
    WSACleanup();
    pthread_exit((void *)server_error);
}