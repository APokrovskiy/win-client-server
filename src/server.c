#include "socket.h"
#include <stdio.h>
#include <stdlib.h>

int my_getline(FILE* in, char buffer[], int n)
{
    if (!in || !buffer)
    {
        fprintf(stderr, "getline() error: File or buffer has null\n");
        return -1;
    }

    int ch, count = 0;
    
    while((ch = fgetc(in)) != EOF && ch != '\n')
    {
        if (count < n) buffer[count++] = ch;
        else
        {
            fprintf(stderr, "getline() error: too litle buffer\n");
            return -1;
        }
    }
    buffer[count] = '\0';

    return count;
}

int main(int argc, char const *argv[])
{
    SOCKET servsock, clientsock;

    char ip[25];
    unsigned short port;

    printf("IP: ");
    scanf("%s", ip);
    printf("Port: ");
    scanf("%hu", &port);

    create_socket(&servsock);
    make_server(&servsock, ip, port);
    server_listen(&servsock, &clientsock);

    char message[100];

    if (my_getline(stdin, message, 100) <= 0)
    {
        fprintf(stderr,"Uncorrect getline()\n");
        return -1;
    }
    
    send(clientsock, message, strlen(message)+1, 0 );

    return 0;
}
