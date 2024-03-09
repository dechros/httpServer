#ifndef HTTP_CONTROLLER_H
#define HTTP_CONTROLLER_H

#include "socketController.h"

constexpr int PORT = 8888;

class HttpController
{
private:
    SocketController socketController;

public:
    HttpController();
    ~HttpController();
};

#endif