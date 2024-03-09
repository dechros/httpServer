#ifndef SOCKET_CONTROLLER_H
#define SOCKET_CONTROLLER_H

#include <iostream>
#include <array>
#include <vector>
#include <thread>
#include <stdexcept>
#include <winsock2.h>

constexpr int BUFFER_SIZE = 1024;
constexpr int PORT = 8888;

class SocketController
{
private:
    typedef struct sockaddr_in SocketAddress;

    WSADATA windowsSocketData;
    SOCKET serverSocket;
    SOCKET clientSocket;

    SocketAddress serverSocketAddress;
    SocketAddress clientSocketAddress;
    int clientSocketAdressLen;

    void clientHandle(SOCKET clientSocket);

public:
    SocketController(void);
    ~SocketController(void);
    void run(void);
};

#endif