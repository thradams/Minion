
#pragma once
#include <stdbool.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <stdlib.h>   // Needed for _wtoi
#include <stddef.h>
#include <Windows.h>
//#include <stdexcept>
//#include <chrono>
//#include <iostream>
//#include <thread>
#pragma comment (lib, "Ws2_32.lib")


typedef SOCKET Socket;
#define SOCKET_INIT -1

void Socket_Close(Socket socket);
Socket Socket_Create(int af,
  int type,
  int protocol
);

int Socket_Bind(Socket socket,
  const struct sockaddr * name,
  int namelen
);

int Socket_Listen(Socket socket, int backlog);

Socket Socket_Accept(Socket socket, struct sockaddr * addr, int *addrlen);


bool Socket_IsReadyToReceive(Socket sock, int intervalMs);
void Socket_CloseGracefully(Socket socket);


inline int SetNonBlockingMode(SOCKET sock)
{
    unsigned long on = 1;
    return ioctlsocket(sock, FIONBIO, &on);
}

const char* GetSocketErrorA(int error);
const wchar_t* GetSocketErrorW(int error);

int Socket_SetTimeout(Socket sock, int milliseconds);

/*


inline std::string GetPeername(SOCKET socket)
{
    struct sockaddr_in name;
    socklen_t len = sizeof(name);

    std::string s;
    if (getpeername(socket, (struct sockaddr *)&name, &len) == 0)
    {
        char buf[INET_ADDRSTRLEN] = {0};
        strncpy_s(buf, inet_ntoa(name.sin_addr), _TRUNCATE);
        s = buf;
    }

    return s;    
}

inline int Recv(SOCKET s, char *buf, int len, int flags, int& socketError)
{
    socketError = 0; //out
    int result = recv(s, buf, len, flags);

    if (result > 0)
    {
        //ok
    }

    if (result == 0)
    {
        //closed
    }

    if (result == SOCKET_ERROR)
    { 
        socketError = WSAGetLastError();
        if (socketError == 0)
        {
            ASSERT(false);        
            socketError = 1;
        }
    }

    return result;
}
*/




size_t Socket_PushBytes(Socket socket,
  const char* bytes,
  size_t len);


int SetSockTimeout(Socket sock, int milliseconds);