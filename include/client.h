#ifndef CLIENT_H
#define CLIENT_h

enum client_errors
{
    CLIENT_NO_ERROR,
    CLIENT_RECEIVE_ERROR,
    CLIENT_SEND_ERROR
};

void *handle_client(void *arg);

#endif