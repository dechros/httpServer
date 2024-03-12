#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <winsock2.h>
#include <pthread.h>
#include <string.h>
#include <winuser.h>
#include "server.h"
#include "client.h"

struct tcp_server
{
    bool init_done;
    WSADATA wsa_data;
    SOCKET server_socket;
    SOCKET client_socket;
    struct sockaddr_in server_address;
    struct sockaddr_in client_address;
};

static struct tcp_server tcp_server_data;

enum server_errors server_init(const int port)
{
    enum server_errors ret_val = SERVER_INIT_ERROR;

    tcp_server_data.server_address.sin_family = AF_INET;         /* IPV4 */
    tcp_server_data.server_address.sin_addr.s_addr = INADDR_ANY; /* Listen on every interface */
    tcp_server_data.server_address.sin_port = htons(port);

    int wsa_status = WSAStartup(MAKEWORD(2, 2), &tcp_server_data.wsa_data);
    if (wsa_status != 0)
    {
        ret_val = SERVER_INIT_ERROR;
        int wsa_error = WSAGetLastError();
        fprintf(stderr, "Failed to initialize Windows socket data. Error : \n", wsa_error);
    }
    else
    {
        tcp_server_data.server_socket = socket(AF_INET, SOCK_STREAM, 0);
        if (tcp_server_data.server_socket == INVALID_SOCKET)
        {
            ret_val = SERVER_INIT_ERROR;
            int wsa_error = WSAGetLastError();
            fprintf(stderr, "Failed to create tcp server socket. Error : \n", wsa_error);
        }
        else
        {
            int bind_status = bind(tcp_server_data.server_socket, (struct sockaddr *)&tcp_server_data.server_address, sizeof(tcp_server_data.server_address));
            if (bind_status == SOCKET_ERROR)
            {
                ret_val = SERVER_INIT_ERROR;
                int wsa_error = WSAGetLastError();
                fprintf(stderr, "Failed to bind server socket to the server address. Error : \n", wsa_error);
            }
            else
            {
                int listen_status = listen(tcp_server_data.server_socket, SOMAXCONN);
                if (listen_status == SOCKET_ERROR)
                {
                    ret_val = SERVER_INIT_ERROR;
                    int wsa_error = WSAGetLastError();
                    fprintf(stderr, "Failed to listen for incoming connections. Error : \n", wsa_error);
                }
                else
                {
                    ret_val = SERVER_NO_ERROR;
                    tcp_server_data.init_done = true;
                    printf("TCP server socket initialization is succesfull.\n");
                }
            }
        }
    }

    return ret_val;
}

enum server_errors server_run(void)
{
    enum server_errors ret_val = SERVER_RUNTIME_ERROR;

    if (tcp_server_data.init_done == false)
    {
        ret_val = SERVER_RUNTIME_ERROR;
        printf("TCP server is not initialized.\n");
    }
    else
    {
        ret_val = SERVER_NO_ERROR;
        printf("TCP server started listening on port 8888.\n");

        while (ret_val == SERVER_NO_ERROR)
        {
            size_t client_sock_addr_len = sizeof(tcp_server_data.client_address);
            tcp_server_data.client_socket = accept(tcp_server_data.server_socket, (struct sockaddr *)&tcp_server_data.client_address, (int *)&client_sock_addr_len);

            if (tcp_server_data.client_socket == INVALID_SOCKET)
            {
                fprintf(stderr, "Failed to accept client connection. Client socket : %d\n", tcp_server_data.client_socket);
            }
            else
            {
                printf("Client is connected. Client socket : %d\n", tcp_server_data.client_socket);
                pthread_t thread;
                int thread_status = pthread_create(&thread, NULL, &handle_client, (void *)tcp_server_data.client_socket);
                if (thread_status != 0)
                {
                    ret_val = SERVER_RUNTIME_ERROR;
                    fprintf(stderr, "Error creating client handle thread. Client socket : %d\n", tcp_server_data.client_socket);
                }
                else
                {
                    printf("New thread is created. Client socket : %d\n", tcp_server_data.client_socket);
                }
            }
        }

        closesocket(tcp_server_data.server_socket);
        WSACleanup();
    }

    return ret_val;
}