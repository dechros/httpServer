#ifndef CLIENT_H
#define CLIENT_H

enum client_errors
{
    CLIENT_NO_ERROR,
    CLIENT_INIT_ERROR,
    CLIENT_RUNTIME_ERROR,
    CLIENT_DISCONNECTED
};

struct client
{
    bool init;
    bool stop;
    SOCKET socket;
    struct sockaddr_in address;
    pthread_t thread;
    fd_set fd;
    char *request;
    char *response;
    struct timeval timeout;
    enum client_errors error;
};

extern void client_init(struct client *client_data);
extern void client_run(struct client *client_data);
extern void client_stop(struct client *client_data);

#endif