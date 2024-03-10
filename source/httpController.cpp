#include "httpController.h"

HttpController::HttpController(int port) : SocketController(port)
{
}

HttpController::~HttpController()
{
}

std::string HttpController::requestHandler(const std::string &request)
{
    return "am";
}