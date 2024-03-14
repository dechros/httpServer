#ifndef CLIENT_H
#define CLIENT_H

enum client_errors
{
    CLIENT_NO_ERROR,
    CLIENT_RECEIVE_ERROR,
    CLIENT_SEND_ERROR
};

struct client
{
    bool init;
    bool stop;
    SOCKET socket;
    struct sockaddr_in address;
    pthread_t thread;
    fd_set fd;
};

void *client_thread(void *arg);

#endif