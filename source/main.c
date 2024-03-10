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

#include <stdio.h>
#include "socket.h"

#define PORT 8888

int main(void)
{
    socketInit(PORT);
    while (true)
    {
        char userInput;
        scanf(&userInput);
    }
    exit(EXIT_SUCCESS);
}