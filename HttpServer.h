#pragma once



#include "Error.h"
#include "Socket.h"
#include "HttpConnection.h"
#include <openssl/ssl.h>

enum SECURITY
{
  SECURITY_VERSION_NONE = 0,
  SECURITY_VERSION_SSL2 = 1,
  SECURITY_VERSION_SSL3 = 2,
  SECURITY_VERSION_TLS1 = 2,
  SECURITY_VERSION_TLS11 = 3,
  SECURITY_VERSION_TLS12 = 4,
};

typedef void(*HandleFunction)(struct HttpConnection* connection);

struct HttpServer
{
  Socket listenSocket;
  SSL_CTX *m_ctx;
  HandleFunction HandleFunction;
};

bool HttpServer_Run(struct HttpServer* httpServer);

bool HttpServer_Init(struct HttpServer* httpServer,
  enum SECURITY securityVersion,
  HandleFunction handleFunction,
  const char* port,
  const char* strsslCertificate,
  const char* strsslPrivateKey,
  struct Error* error);
