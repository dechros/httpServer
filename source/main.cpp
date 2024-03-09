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

int main(void)
{
    HttpController httpController;
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