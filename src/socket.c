#include "socket.h"

#include <winsock2.h>
#include <ws2tcpip.h>
#include <ws2def.h>
#include <stdio.h>

int create_socket( SOCKET *out )
{
    WSADATA wsData; // Структура с информацией о версии сокетов
    // Минимальная версия сокета(2) в старшем байте слова, Максимальная версия тоже 2 в младшем байте слова
    if ( WSAStartup(MAKEWORD(2,2), &wsData) != 0)
    {
        fprintf(stderr, "%d: WinSock Version Init error\n", WSAGetLastError());
        goto error;
    }

    if ( (*out = socket(AF_INET, SOCK_STREAM, 0) ) == INVALID_SOCKET)
    {
        fprintf(stderr, "%d: Socket init error\n", WSAGetLastError());
        goto error;
    }

    return 0;

error:
    fprintf(stderr, "error: SOCKET_CREATE_ERROR\n");
    closesocket(*out);
    WSACleanup();
    return 1;
}
int connect_to_server(SOCKET *client_socket, const char* ip, unsigned short port)
{
    SOCKADDR_IN clientInfo;
    ZeroMemory(&clientInfo, sizeof(clientInfo));

    clientInfo.sin_family = AF_INET;
    clientInfo.sin_port = htons(port);
    if (inet_pton(AF_INET, ip , &clientInfo.sin_addr) < 0)
    {
        fprintf(stderr, "%d: IP Translation to special numeric format Error\n", WSAGetLastError());
        goto error;
    }
    if (connect(*client_socket,(SOCKADDR*)&clientInfo, sizeof(clientInfo)) != 0)
    {
        fprintf(stderr, "%d Connect Error\n", WSAGetLastError());
        goto error;
    }

    return 0;

error:
    fprintf(stderr, "error: SERVER_CONNECT_ERROR\n");
    closesocket(*client_socket);
    WSACleanup();
    return 1;
}

int make_server(SOCKET *sock, const char* ip, unsigned short port)
{
    // Структура для привязывания сокету внешней информации для идендификации другими процессами
    SOCKADDR_IN servInfo; 
    // Обнуление памяти занимаемой структурой (Очистка структуры от раннее хранимых данных)
    ZeroMemory(&servInfo, sizeof(servInfo)); 
    servInfo.sin_family = AF_INET;
    servInfo.sin_port = htons(port);
    if (inet_pton(AF_INET, ip, &servInfo.sin_addr) < 0)
    {
        fprintf(stderr, "%d: IP Translation to special numeric format Error\n", WSAGetLastError());
        goto error;
    } 

    if (bind(*sock,(SOCKADDR*)&servInfo, sizeof(servInfo)) != 0)
    {
        fprintf(stderr, "%d: Socket Binding error\n", WSAGetLastError());
        closesocket(*sock);
        WSACleanup();
        goto error;
    }

    return 0;

error:
    fprintf(stderr, "error: MAKE_SERVER_ERROR\n");
    closesocket(*sock);
    WSACleanup();
    return 1;
}

int server_listen(SOCKET *sock, SOCKET *out)
{
    // Сокет для Сервера готов
    // Программа будет приостановлена пока клиент не подключится к серверу
    if (listen(*sock, SOMAXCONN) != 0)
    {
        fprintf(stderr, "%d: listen start error\n", WSAGetLastError());
        goto error;
    } 

    SOCKADDR_IN clientInfo;

    ZeroMemory(&clientInfo, sizeof(clientInfo));

    int clientInfoSize = sizeof(clientInfo);

    *out = accept(*sock, (SOCKADDR*)&clientInfo, &clientInfoSize);

    if (*out == INVALID_SOCKET)
    {
        fprintf(stderr, "%d: Client detected, but can't connect\n", WSAGetLastError());
        goto error;
    }
    
    return 0;

error:
    fprintf(stderr, "CLIENT_CONNECT_ERROR\n");
    closesocket(*sock);
    closesocket(*out);
    return 1;
}