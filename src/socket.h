#pragma once
#include "winsock2.h"


// for all
int create_socket( SOCKET *out );

// for client
int connect_to_server(SOCKET *client_socket, const char* ip, unsigned short port);

// for server
int make_server(SOCKET *sock, const char* ip, unsigned short port);
int server_listen(SOCKET *sock, SOCKET *out);