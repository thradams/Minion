
#pragma once
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>   // Needed for _wtoi
#include <stddef.h>

#include "Error.h"

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#include <Windows.h>
typedef SOCKET Socket;
#define SOCKET_INIT -1
#pragma comment (lib, "Ws2_32.lib")
#else
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/ioctl.h>
#include <netdb.h>
#include <sys/select.h>

typedef int Socket;
#define SOCKET_INIT -1
#define INVALID_SOCKET -1
//send select..
#define SOCKET_ERROR -1


#endif


void Socket_Close(Socket socket);
Socket Socket_Create(int af,
                     int type,
                     int protocol,
                     struct Error* error);

bool Socket_Bind(Socket socket,
    const struct sockaddr * name,
    int namelen
    , struct Error* error);

bool Socket_Listen(Socket socket, int backlog, struct Error* error);

Socket Socket_Accept(Socket socket, struct sockaddr * addr, int *addrlen, struct Error* error);


bool Socket_IsReadyToReceive(Socket sock, int intervalMs);
void Socket_CloseGracefully(Socket socket);


int SetNonBlockingMode(Socket sock);

const char* GetSocketErrorA(int error);


int Socket_Recv(Socket socket, void *buf, size_t n, int flags, struct Error* error);


int Socket_SetTimeout(Socket sock, int milliseconds, struct Error* error);


size_t Socket_PushBytes(Socket socket,
    const char* bytes,
    size_t len, struct Error* error);


void SocketStaticInit();
void SocketStaticDestroy();
