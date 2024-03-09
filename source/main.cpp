/**
 * @file main.cpp
 * @author Halit Cetin (halitcetin@live.com)
 * @brief HTTP Server
 * @version 0.1
 * @date 09-03-2024
 *
 * @copyright Copyright (c) 2024
 * 
 */

#include <iostream>
#include "socketController.h"
#include "serverController.h"

int main(void)
{
    SocketController socketController;
    socketController.run();
    return 0;
}