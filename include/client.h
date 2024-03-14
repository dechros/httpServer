#ifndef CLIENT_H
#define CLIENT_H

#define BUFFER_SIZE 4096

enum client_errors
{
    CLIENT_NO_ERROR,
    CLIENT_INIT_ERROR,
    CLIENT_RUNTIME_ERROR,
    CLIENT_DISCONNECTED
};

struct client_data
{
    bool init;
    bool stop;
    SOCKET socket;
    struct sockaddr_in address;
    pthread_t thread;
    fd_set fd;
    char request[BUFFER_SIZE];
    char response[BUFFER_SIZE];
    struct timeval timeout;
    enum client_errors error;
};

extern void client_init(struct client_data *client);
extern void client_run(struct client_data *client);
extern void client_stop(struct client_data *client);

#endif