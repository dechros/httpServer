#include "socketController.h"

SocketController::SocketController(void)
{
    try
    {
        serverSocketAddress.sin_family = AF_INET;         /* IPV4 */
        serverSocketAddress.sin_addr.s_addr = INADDR_ANY; /* Listen every interface */
        serverSocketAddress.sin_port = htons(PORT);       /* Port */

        int wsaStatus = WSAStartup(MAKEWORD(2, 2), &windowsSocketData);
        if (wsaStatus != 0)
        {
            throw std::runtime_error("Failed to initialize Windows socket data.");
        }

        serverSocket = socket(AF_INET, SOCK_STREAM, 0);
        if (serverSocket == INVALID_SOCKET)
        {
            throw std::runtime_error("Failed to create server socket.");
        }

        int bindStatus = bind(serverSocket, (struct sockaddr *)&serverSocketAddress, sizeof(serverSocketAddress));
        if (bindStatus == SOCKET_ERROR)
        {
            throw std::runtime_error("Failed to bind server socket into the server adress.");
        }

        int listenStatus = listen(serverSocket, SOMAXCONN);
        if (listenStatus == SOCKET_ERROR)
        {
            throw std::runtime_error("Failed to listen incoming connections.");
        }

        std::cout << "Server started listening on port 8888." << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        throw;
    }
}

SocketController::~SocketController(void)
{
    closesocket(serverSocket);
    closesocket(clientSocket);
    WSACleanup();
}

void SocketController::run(void)
{
    while (true)
    {
        try
        {
            clientSocketAdressLen = sizeof(clientSocketAddress);
            clientSocket = accept(serverSocket, reinterpret_cast<struct sockaddr *>(&clientSocketAddress), &clientSocketAdressLen);

            if (clientSocket == INVALID_SOCKET)
            {
                throw std::runtime_error("Failed to accept incoming client connection.");
            }

            std::cout << "Client is connected." << std::endl;

            std::array<char, BUFFER_SIZE> buffer;
            int bytesReceived = recv(clientSocket, reinterpret_cast<char *>(&buffer), sizeof(buffer), 0);
            if (bytesReceived == SOCKET_ERROR)
            {
                throw std::runtime_error("Failed to receive incoming data.");
            }

            std::cout << "Incoming data : " << buffer.data() << std::endl;
        }
        catch (const std::exception &e)
        {
            std::cerr << e.what() << std::endl;
            throw;
        }
    }
}

void SocketController::clientHandle(SOCKET clientSocket)
{

}