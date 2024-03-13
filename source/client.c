#include <stdio.h>
#include <stdbool.h>
#include <winsock2.h>
#include "client.h"

#define BUFFER_SIZE 4096

enum request_types
{
    HTTP,
    FTP,
    SMTP,
    OTHER
};

void *handle_client(void *arg)
{
    SOCKET client_socket = (SOCKET)arg;
    char request_buffer[BUFFER_SIZE] = {0};
    char response_buffer[BUFFER_SIZE] = {0};
    enum client_errors ret_val = CLIENT_NO_ERROR;

    printf("Client thread is created. Socket : %d\n", client_socket);

    while (ret_val == CLIENT_NO_ERROR)
    {
        int bytes_received = recv(client_socket, request_buffer, BUFFER_SIZE - 1, 0);
        if (bytes_received == SOCKET_ERROR || bytes_received == 0)
        {
            fprintf(stderr, "Message receive error. Socket : %d\n", client_socket);
            ret_val = CLIENT_RECEIVE_ERROR;
        }
        else
        {
            request_buffer[bytes_received] = '\0';
            printf("Bytes received : %d\n", bytes_received);
            printf("Message received : %s\n", request_buffer);

            strcpy(response_buffer, request_buffer);
            int send_status = send(client_socket, response_buffer, strlen(response_buffer), 0);
            if (send_status == 0)
            {
                fprintf(stderr, "Message send error. Socket : %d\n", client_socket);
                ret_val = CLIENT_SEND_ERROR;
            }
            else
            {
                printf("Message sent : %s\n", response_buffer);
                ret_val = CLIENT_NO_ERROR;
            }
        }
    }

    printf("Client thread is closed. Socket : %d\n", client_socket);
    closesocket(client_socket);
}

enum request_types detect_request_type()
{
}