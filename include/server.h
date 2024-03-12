#ifndef SERVER_H
#define SERVER_H

enum server_errors
{
    SERVER_NO_ERROR,
    SERVER_INIT_ERROR,
    SERVER_RUNTIME_ERROR
};

extern enum server_errors server_init(const int port);
extern enum server_errors server_run(void);
extern enum server_errors server_stop(void);

#endif