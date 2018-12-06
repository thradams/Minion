
#pragma once

#include "Socket.h"
#include <openssl/ssl.h>
#include "Error.h"

enum HTTP_METHOD
{
  HTTP_METHOD_NONE,
  HTTP_METHOD_OPTIONS,
  HTTP_METHOD_GET,
  HTTP_METHOD_HEAD,
  HTTP_METHOD_POST,
  HTTP_METHOD_PUT,
  HTTP_METHOD_DELETE,
  HTTP_METHOD_TRACE,
  HTTP_METHOD_CONNECT,
  HTTP_METHOD_EXTENSION
};

struct HttpServer;
/*
  HttpConnection abstrai o conceito de socket de tal forma
  que nao insteressa se eh socket normal ou se eh conexao segura
  via openss. O proprio openssl eh abstraido de forma que possa
  ser substituido por outra coisa.
*/
struct HttpConnection
{
  Socket Socket;

  //SSL  
  SSL *ssl;
  BIO *io;
  BIO *ssl_bio;
  BIO *sbio;  

  enum HTTP_METHOD Method;
  //vou fazer conter o heaader da request
  bool bKeepAlive;
  char uri[200];
  int ContentLength;

  struct HttpServer *pHttpServer;
};

struct HttpConnection* HttpConnection_Create(Socket socket, SSL_CTX *ssl, struct Error* error);
bool HttpConnection_Init(struct HttpConnection* connection, Socket socket, SSL_CTX *ctx, struct Error* error);
void HttpConnection_Delete(struct HttpConnection* h);

size_t HttpConnection_PushBytes(struct HttpConnection* connection, const char* bytes, size_t len, struct Error* error);
bool HttpConnection_GetChar(struct HttpConnection* connection, char *ch, struct Error* error);

bool HttpConnection_SendFile(struct HttpConnection* connection,
  const char* fileName,
  const char* ROOT_PATH,
  struct Error* error);

void SaveFile(struct HttpConnection* pCon,
    const char* SOURCE_PATH,
    struct Error* error);

bool HttpConnection_SendOK(struct HttpConnection* connection, struct Error* error);
