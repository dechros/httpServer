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

int main(void)
{
    int ret_val = EXIT_FAILURE;
    enum server_errors error;

    error = server_init(PORT);
    if (error != SERVER_NO_ERROR)
    {
        ret_val = EXIT_FAILURE;
    }
    else
    {
        error = server_run();
        if (error != SERVER_NO_ERROR)
        {
            ret_val = EXIT_FAILURE;
        }
        else
        {
            error = server_stop();
            if (error != SERVER_NO_ERROR)
            {
                ret_val = EXIT_FAILURE;
            }
            else
            {
                ret_val = EXIT_SUCCESS;
            }
        }
    }
    return ret_val;
}