/**
 * @file main.c
 * @author Halit Cetin (halitcetin@live.com)
 * @brief HTTP Server
 * @version 0.1
 * @date 09-03-2024
 *
 * @copyright Copyright (c) 2024
 *
 */

#include <stdlib.h>
#include "server.h"

#define PORT 8888

static struct tcp_server server;

int main(void)
{
    enum server_errors error;
    error = server_init(&server, PORT);
    if (error != SERVER_NO_ERROR)
    {
        return EXIT_FAILURE;
    }

    error = server_run(&server);
    if (error != SERVER_NO_ERROR)
    {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}