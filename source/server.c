#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include "server.h"
#include "client.h"

static void *server_task(void *arg);

enum server_errors server_init(struct tcp_server *tcp_server_data, const int port)
{
    tcp_server_data->server_address.sin_family = AF_INET;         /* IPV4 */
    tcp_server_data->server_address.sin_addr.s_addr = INADDR_ANY; /* Listen on every interface */
    tcp_server_data->server_address.sin_port = htons(port);

    int wsa_status = WSAStartup(MAKEWORD(2, 2), &tcp_server_data->wsa_data);
    if (wsa_status != 0)
    {
        int wsa_error = WSAGetLastError();
        fprintf(stderr, "Port : %d Failed to initialize Windows socket data. Error : %d\n", ntohs(tcp_server_data->server_address.sin_port), wsa_error);
        return SERVER_INIT_ERROR;
    }

    tcp_server_data->server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (tcp_server_data->server_socket == INVALID_SOCKET)
    {
        int wsa_error = WSAGetLastError();
        fprintf(stderr, "Port : %d Failed to create server socket. Error : %d\n", ntohs(tcp_server_data->server_address.sin_port), wsa_error);
        return SERVER_INIT_ERROR;
    }

    int bind_status = bind(tcp_server_data->server_socket, (struct sockaddr *)&tcp_server_data->server_address, sizeof(tcp_server_data->server_address));
    if (bind_status == SOCKET_ERROR)
    {
        int wsa_error = WSAGetLastError();
        fprintf(stderr, "Port : %d Failed to bind server socket to the server address. Error : %d\n", ntohs(tcp_server_data->server_address.sin_port), wsa_error);
        return SERVER_INIT_ERROR;
    }

    int listen_status = listen(tcp_server_data->server_socket, SOMAXCONN);
    if (listen_status == SOCKET_ERROR)
    {
        int wsa_error = WSAGetLastError();
        fprintf(stderr, "Port : %d Failed to listen for incoming connections. Error : %d\n", ntohs(tcp_server_data->server_address.sin_port), wsa_error);
        return SERVER_INIT_ERROR;
    }

    tcp_server_data->init_done = true;
    printf("Port : %d TCP server socket initialization is succesfull.\n", ntohs(tcp_server_data->server_address.sin_port));
    return SERVER_NO_ERROR;
}

enum server_errors server_run(struct tcp_server *tcp_server_data)
{
    if (tcp_server_data->init_done == false)
    {
        printf("Port : %d TCP server is not initialized.\n", ntohs(tcp_server_data->server_address.sin_port));
        return SERVER_RUNTIME_ERROR;
    }

    pthread_t thread;
    int thread_status = pthread_create(&thread, NULL, &server_task, (void *)tcp_server_data);
    if (thread_status != 0)
    {
        fprintf(stderr, "Error creating server thread.");
        return SERVER_RUNTIME_ERROR;
    }

    return SERVER_NO_ERROR;
}

void *server_task(void *arg)
{
    struct tcp_server *tcp_server_data = (struct tcp_server *)arg;

    printf("TCP server started listening on port : %d.\n", ntohs(tcp_server_data->server_address.sin_port));

    while (true)
    {
        size_t client_sock_addr_len = sizeof(tcp_server_data->client_address);
        tcp_server_data->client_socket = accept(tcp_server_data->server_socket, (struct sockaddr *)&tcp_server_data->client_address, (int *)&client_sock_addr_len);

        if (tcp_server_data->client_socket == INVALID_SOCKET)
        {
            fprintf(stderr, "Failed to accept client connection. Client socket : %d\n", tcp_server_data->client_socket);
        }
        else
        {
            printf("Client is connected. Client socket : %d\n", tcp_server_data->client_socket);
            pthread_t thread;
            int thread_status = pthread_create(&thread, NULL, &handle_client, (void *)tcp_server_data->client_socket);
            if (thread_status != 0)
            {
                fprintf(stderr, "Error creating client handle thread. Client socket : %d\n", tcp_server_data->client_socket);
                break;
            }
            else
            {
                printf("New thread is created. Client socket : %d\n", tcp_server_data->client_socket);
            }
        }
    }

    closesocket(tcp_server_data->server_socket);
    WSACleanup();
}