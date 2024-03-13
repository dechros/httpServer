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

static struct tcp_server server1;
static struct tcp_server server2;

int main(void)
{
    enum server_errors error;
    error = server_init(&server1, PORT);
    if (error != SERVER_NO_ERROR)
    {
        return EXIT_FAILURE;
    }

    error = server_init(&server2, 3152);
    if (error != SERVER_NO_ERROR)
    {
        return EXIT_FAILURE;
    }

    error = server_run(&server1);
    if (error != SERVER_NO_ERROR)
    {
        return EXIT_FAILURE;
    }

    error = server_run(&server2);
    if (error != SERVER_NO_ERROR)
    {
        return EXIT_FAILURE;
    }

    while (true)
    {
    }

    return EXIT_SUCCESS;
}