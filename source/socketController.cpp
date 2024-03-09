#include "socketController.h"

SocketController::SocketController(const int port)
{
    serverSocketAddress.sin_family = AF_INET;         /* IPV4 */
    serverSocketAddress.sin_addr.s_addr = INADDR_ANY; /* Listen every interface */
    serverSocketAddress.sin_port = htons(port);

    int wsaStatus = WSAStartup(MAKEWORD(2, 2), &windowsSocketData);
    if (wsaStatus != 0)
    {
        std::cerr << "Failed to initialize Windows socket data." << std::endl;
        throw;
    }
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == INVALID_SOCKET)
    {
        std::cerr << "Failed to create server socket." << std::endl;
        throw;
    }

    int bindStatus = bind(serverSocket, (struct sockaddr *)&serverSocketAddress, sizeof(serverSocketAddress));
    if (bindStatus == SOCKET_ERROR)
    {
        std::cerr << "Failed to bind server socket into the server adress." << std::endl;
        throw;
    }

    int listenStatus = listen(serverSocket, SOMAXCONN);
    if (listenStatus == SOCKET_ERROR)
    {
        std::cerr << "Failed to listen incoming connections." << std::endl;
        throw;
    }

    std::thread t([this]()
                  { mainThread(); });
    t.detach();
}

SocketController::~SocketController(void)
{
    closesocket(serverSocket);
    closesocket(clientSocket);
    WSACleanup();
}

void SocketController::mainThread(void)
{
    std::cout << "Server started listening on port 8888." << std::endl;

    while (true)
    {
        clientSocketAdressLen = sizeof(clientSocketAddress);
        clientSocket = accept(serverSocket, reinterpret_cast<struct sockaddr *>(&clientSocketAddress), &clientSocketAdressLen);

        if (clientSocket == INVALID_SOCKET)
        {
            std::cerr << "Failed to accept incoming client connection. Socket : " << clientSocket << std::endl;
            continue;
        }

        std::cout << "Client is connected. Socket : " << clientSocket << std::endl;

        std::thread t([this](SOCKET socket)
                      { this->clientHandleThread(clientSocket); },
                      clientSocket);
        t.detach();

        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}

void SocketController::clientHandleThread(SOCKET socket)
{
    while (true)
    {
        std::array<char, BUFFER_SIZE> buffer;
        int bytesReceived = recv(socket, reinterpret_cast<char *>(&buffer), sizeof(buffer), 0);
        if (bytesReceived == SOCKET_ERROR || bytesReceived == 0)
        {
            std::cerr << "Closing the connection on : " << socket << std::endl;
            closesocket(socket);
            break;
        }

        std::cout << "Bytes received : " << bytesReceived << " Incoming raw data : " << buffer.data() << std::endl;
    }
}