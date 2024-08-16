#include "socket.h"
#include <stdio.h>


int main(int argc, char const *argv[])
{
    SOCKET clientsock;
    create_socket(&clientsock);

    char ip[25];
    unsigned short port;

    printf("IP: ");
    scanf("%s", ip);
    printf("Port: ");
    scanf("%hu", &port);

    connect_to_server(&clientsock, ip, port);
    
    char buffer[100];
    recv(clientsock, buffer, 100, 0);
    puts(buffer);
    return 0;
}
