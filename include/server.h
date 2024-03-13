#ifndef SERVER_H
#define SERVER_H

#include <winsock2.h>
#include <windows.h>
#include <stdbool.h>
#include "client.h"

enum server_errors
{
    SERVER_NO_ERROR,
    SERVER_INIT_ERROR,
    SERVER_RUNTIME_ERROR
};

struct server
{
    bool init;
    bool stop;
    SOCKET socket;
    struct sockaddr_in address;
    pthread_t thread;
    fd_set fd;
    WSADATA wsa;
};

struct tcp_server
{
    struct server server;
    struct client *clients;
};

extern enum server_errors server_init(struct tcp_server *tcp, const int port);
extern enum server_errors server_run(struct tcp_server *tcp);
extern enum server_errors server_stop(struct tcp_server *tcp);

#endif