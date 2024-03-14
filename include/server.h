#ifndef SERVER_H
#define SERVER_H

#include <winsock2.h>
#include <windows.h>
#include <stdbool.h>
#include "client.h"

#define MAX_CLIENT 2

enum server_errors
{
    SERVER_NO_ERROR,
    SERVER_INIT_ERROR,
    SERVER_RUNTIME_ERROR
};

struct server_data
{
    bool init;
    bool stop;
    SOCKET socket;
    struct sockaddr_in address;
    pthread_t thread;
    fd_set fd;
    WSADATA wsa;
    struct timeval timeout;
    enum server_errors error;
};

struct tcp_data
{
    struct server_data server;
    struct client_data clients[MAX_CLIENT];
};

extern void server_init(struct tcp_data *tcp, const int port);
extern void server_run(struct tcp_data *tcp);
extern void server_stop(struct tcp_data *tcp);

#endif