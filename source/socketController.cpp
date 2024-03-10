#include "socketController.h"

SocketController::SocketController(const int port)
{
    try
    {
        serverSocketAddress.sin_family = AF_INET;         /* IPV4 */
        serverSocketAddress.sin_addr.s_addr = INADDR_ANY; /* Listen on every interface */
        serverSocketAddress.sin_port = htons(port);

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

        int bindStatus = bind(serverSocket, reinterpret_cast<struct sockaddr *>(&serverSocketAddress), sizeof(serverSocketAddress));
        if (bindStatus == SOCKET_ERROR)
        {
            throw std::runtime_error("Failed to bind server socket to the server address.");
        }

        int listenStatus = listen(serverSocket, SOMAXCONN);
        if (listenStatus == SOCKET_ERROR)
        {
            throw std::runtime_error("Failed to listen for incoming connections.");
        }

        std::thread t(mainThread, this);
        t.detach();
    }
    catch (const std::exception &e)
    {
        closesocket(serverSocket);
        WSACleanup();
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

        std::thread t(clientHandleThread, this, clientSocket);
        t.detach();

        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}

void SocketController::clientHandleThread(const SOCKET socket)
{
    while (true)
    {
        std::array<char, BUFFER_SIZE> buffer;
        int bytesReceived = recv(socket, reinterpret_cast<char *>(&buffer), sizeof(buffer), 0);
        if (bytesReceived == SOCKET_ERROR || bytesReceived == 0)
        {
            std::cerr << "Message receive error. Closing the connection on : " << socket << std::endl;
            closesocket(socket);
            break;
        }

        std::cout << "Bytes received : " << bytesReceived << " Incoming raw data : " << buffer.data() << std::endl;

        std::string request;
        std::copy(buffer.begin(), buffer.end(), request.begin());
        std::string response = requestHandler(request);

        int sendStatus = send(socket, response.c_str(), sizeof(response.c_str()), 0);
        if (sendStatus == 0)
        {
            std::cerr << "Message send error. Closing the connection on : " << socket << std::endl;
            closesocket(socket);
            break;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}