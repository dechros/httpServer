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
#include "httpController.h"

constexpr int PORT = 8888;

int main(void)
{
    HttpController httpController(PORT);
    while (true)
    {
        std::string userInput;
        std::getline(std::cin, userInput);
        if (userInput == "exit")
        {
            break;
        }
    }
    return 0;
}