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
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include "server.h"

#define VER "0.1"
#define PORT 8888

static struct tcp_server server1 = {0};
static struct tcp_server server2 = {0};

int main(void)
{
    printf("Welcome to the HTTP server Version : %s \nPress ESC to exit from the application.\n", VER);

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
        if (GetAsyncKeyState(VK_ESCAPE) & 0x8000)
        {
            printf("ESC key pressed. Exiting from the application.\n");
            break;
        }
        Sleep(1);
    }

    error = server_stop(&server1);
    error = server_stop(&server2);

    return EXIT_SUCCESS;
}