#include "socket.h"

#define BUFFER_SIZE 4096

static WSADATA windowsSocketData = {0};
static SOCKET serverSocket = {0};
static SOCKET clientSocket = {0};

static struct sockaddr_in serverSocketAddress = {0};
static struct sockaddr_in clientSocketAddress = {0};

static int clientSocketAdressLen = 0;
static int initDone = false;

static void *clientThread(void *arg);
static void *mainThread(void *arg);
static char *requestHandler(const char *request);

void socketInit(const int port)
{
    serverSocketAddress.sin_family = AF_INET;         /* IPV4 */
    serverSocketAddress.sin_addr.s_addr = INADDR_ANY; /* Listen on every interface */
    serverSocketAddress.sin_port = htons(port);

    int wsaStatus = WSAStartup(MAKEWORD(2, 2), &windowsSocketData);
    if (wsaStatus != 0)
    {
        fprintf(stderr, "Failed to initialize Windows socket data.\n");
        exit(EXIT_FAILURE);
    }

    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == INVALID_SOCKET)
    {
        fprintf(stderr, "Failed to create server socket.\n");
        exit(EXIT_FAILURE);
    }

    int bindStatus = bind(serverSocket, (struct sockaddr *)&serverSocketAddress, sizeof(serverSocketAddress));
    if (bindStatus == SOCKET_ERROR)
    {
        fprintf(stderr, "Failed to bind server socket to the server address.\n");
        exit(EXIT_FAILURE);
    }

    int listenStatus = listen(serverSocket, SOMAXCONN);
    if (listenStatus == SOCKET_ERROR)
    {
        fprintf(stderr, "Failed to listen for incoming connections.\n");
        exit(EXIT_FAILURE);
    }

    pthread_t thread;
    if (pthread_create(&thread, NULL, mainThread, NULL) != 0)
    {
        fprintf(stderr, "Error creating main socket thread\n");
        exit(EXIT_FAILURE);
    }
}

void socketDestroy(void)
{
    closesocket(serverSocket);
    closesocket(clientSocket);
    WSACleanup();
}

void *mainThread(void *arg)
{
    printf("Server started listening on port 8888.\n");
    while (true)
    {
        clientSocketAdressLen = sizeof(clientSocketAddress);
        clientSocket = accept(serverSocket, (struct sockaddr *)&clientSocketAddress, &clientSocketAdressLen);

        if (clientSocket == INVALID_SOCKET)
        {
            fprintf(stderr, "Failed to accept incoming client connection. Socket : %d\n", clientSocket);
            continue;
        }

        printf("Client is connected. Socket : %d\n", clientSocket);

        pthread_t thread;
        if (pthread_create(&thread, NULL, clientThread, (void *)clientSocket) != 0)
        {
            fprintf(stderr, "Error creating client socket thread : %d\n");
            exit(EXIT_FAILURE);
        }
    }
}

void *clientThread(void *arg)
{
    SOCKET socket = (SOCKET)arg;
    char *buffer = malloc(sizeof(char) * BUFFER_SIZE);
    if (buffer == NULL)
    {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    while (true)
    {
        int bytesReceived = recv(socket, buffer, BUFFER_SIZE - 1, 0);
        if (bytesReceived == SOCKET_ERROR || bytesReceived == 0)
        {
            fprintf(stderr, "Message receive error. Closing the connection on : %d\n", socket);
            closesocket(socket);
            break;
        }

        buffer[bytesReceived] = '\0';
        printf("Bytes received : %d\n", bytesReceived);

        char *response = requestHandler(buffer);
        int sendStatus = send(socket, response, strlen(response), 0);
        free(response);
        if (sendStatus == 0)
        {
            fprintf(stderr, "Message send error. Closing the connection on : %d\n", socket);
            closesocket(socket);
            break;
        }
    }

    free(buffer);
    return NULL;
}

static char *requestHandler(const char *request)
{
    printf("request: %s\n", request);
    size_t requestLength = strlen(request);
    char *response = malloc(sizeof(char) * BUFFER_SIZE);
    if (response == NULL)
    {
        fprintf(stderr, "Memory allocation failed.\n");
        exit(EXIT_FAILURE);
    }
    strcpy(response, request);
    response[requestLength] = '\0';
    printf("response: %s\n", response);
    return response;
}