#ifndef SERVER_H
#define SERVER_H

#include <winsock2.h>
#include <windows.h>
#include <stdbool.h>

enum server_errors
{
    SERVER_NO_ERROR,
    SERVER_INIT_ERROR,
    SERVER_RUNTIME_ERROR
};

struct tcp_server
{
    bool init_done;
    bool stop_signal;
    WSADATA wsa_data;
    SOCKET server_socket;
    SOCKET client_socket;
    struct sockaddr_in server_address;
    struct sockaddr_in client_address;
    pthread_t thread;
};

extern enum server_errors server_init(struct tcp_server *tcp_server_data, const int port);
extern enum server_errors server_run(struct tcp_server *tcp_server_data);
extern enum server_errors server_stop(struct tcp_server *tcp_server_data);

#endif