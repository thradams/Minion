#include "..\HttpServer.h"
#include "..\ThreadPool.h"
#define PORT_NUMBER "8080"

void MinionServerConnectionHandler(struct HttpConnection* connection)
{

}

int main()
{
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    ThreadPool_Init(NULL, 100);

    struct HttpServer httpServer;
    struct Error error = ERROR_INIT;

    if (HttpServer_Init(&httpServer,
        SECURITY_VERSION_NONE,
        MinionServerConnectionHandler,
        0,
        PORT_NUMBER,
        "my_test_cert.crt",
        "my_test_privkey.key",
        &error))
    {

    }
    else
    {
    }


    ThreadPool_Stop(NULL);
    ThreadPool_Join(NULL);
 
    WSACleanup();



    return 0;
}
