#ifndef HTTP_CONTROLLER_H
#define HTTP_CONTROLLER_H

#include <iostream>
#include "socketController.h"

class HttpController : private SocketController
{
private:
    std::string requestHandler(const std::string &request) override;

public:
    HttpController(int port);
    ~HttpController();
};

#endif