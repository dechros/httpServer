#include "httpController.h"

HttpController::HttpController(int port) : SocketController(port)
{
}

HttpController::~HttpController()
{
}

std::string HttpController::requestHandler(const std::string &request)
{
    std::cout << "Raw request : " << request << std::endl;
    return "am";
}