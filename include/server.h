#ifndef SERVER_H
#define SERVER_H

#include <stdbool.h>
#include <winsock2.h>

enum server_errors
{
    SERVER_NO_ERROR,
    SERVER_INIT_ERROR,
    SERVER_RUNTIME_ERROR
};

struct tcp_server
{
    bool init_done;
    WSADATA wsa_data;
    SOCKET server_socket;
    SOCKET client_socket;
    struct sockaddr_in server_address;
    struct sockaddr_in client_address;
};

extern enum server_errors server_init(struct tcp_server *tcp_server_data, const int port);
extern enum server_errors server_run(struct tcp_server *tcp_server_data);

#endif