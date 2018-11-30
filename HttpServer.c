
#include <assert.h>
#include <stdbool.h>
#include "Socket.h"
#include "ThreadPool.h"
#include "HttpServer.h"
#include "HttpConnection.h"
#include "Board.h"
#include "AppBuild.h"

extern char ROOT_PATH[100];
extern char SOURCE_PATH[100];

int CaseInsensitiveCompare(char const *a, char const *b)
{
  for (;; a++, b++) {
    int d = tolower(*a) - tolower(*b);
    if (d != 0 || !*a)
      return d;
  }
}

bool ReadUntil(struct HttpConnection* pCon,
  char ch,
  char * dest,
  size_t destLen,
  struct Error* error)
{
  if (!Error_IsEmpty(error))
  {
    return false;
  }

  int i = 0;
  char local;
  do
  {
    if (HttpConnection_GetChar(pCon, &local, error))
    {
      if (local == ch)
      {
        break;
      }
      else
      {
        if (i < (int)destLen - 1)
        {
          *dest = tolower(local);
          dest++;
          i++;
        }
        else
        {
          Error_Set(error, "insufficient space");
        }
      }
    }
  } while (Error_IsEmpty(error));

  *dest = 0;
  return Error_IsEmpty(error);
}


bool ReadLine(struct HttpConnection* pCon,
  char* bufferLeft,
  char* bufferRight,
  size_t buffersize,
  struct Error* error)
{
  if (!Error_IsEmpty(error))
  {
    return false;
  }

  *bufferLeft = 0;
  *bufferRight = 0;

  bool result = true;
  char* p = bufferLeft;
  char ch;
  bool found = false;
  while (Error_IsEmpty(error))
  {
    if (HttpConnection_GetChar(pCon, &ch, error))
    {
      if (ch == '\r')
      {
        if (HttpConnection_GetChar(pCon, &ch, error))
        {
          if (ch == '\n')
          {
            //result = false;
            break;
          }
        }
      }
      else if (ch == ':' && !found)
      {
        *p = 0;

        p = bufferRight;
        found = true;
        while (HttpConnection_GetChar(pCon, &ch, error) && ch == ' ')
        {
        }
        if (ch != ' ')
        {
          *p = tolower(ch);
          p++;
        }
      }
      else
      {
        *p = tolower(ch);
        p++;
      }
    }
  }
  *p = 0;
  if (!Error_IsEmpty(error))
  {
    result = false;
  }
  return result;
}

static void HandleConnection(enum TASK_ACTION action, void* pData);

bool Call(struct HttpConnection* pCon,
  int funcID,
  struct Error* error)
{
  AddPost(L"Thread/%d Call", (int)GetCurrentThreadId());
  switch (funcID)
  {
  case 1:
    BuildApp(SOURCE_PATH, ROOT_PATH);
    break;
  }


  if (Error_IsEmpty(error))
  {
    if (pCon->bKeepAlive)
    {
      //Reseta o header e reaproveita a conex�o
      pCon->bKeepAlive = false;
      pCon->uri[0] = 0;
      pCon->Method = HTTP_METHOD_NONE;

      Socket_SetTimeout(pCon->Socket, 3000);
      HandleConnection(TASK_RUN, (void*)&pCon);
    }
    else
    {
      //ok pode fechar
      HttpConnection_Delete(pCon);
    }
  }
  else
  {
    //enviar error
    HttpConnection_Delete(pCon);
  }
  return true;
}

static void SaveFile(struct HttpConnection* pCon,
  struct Error* error)
{
  if (pCon->ContentLength > 0)
  {
    char buffer[MAX_PATH];
    char* strings[10];
    int stringsCount = 0;
    strcpy(buffer, pCon->uri);


    char* p = buffer;
    strings[stringsCount] = p;
    stringsCount++;

    while (*p)
    {
      if (*p == '/')
      {
        strings[stringsCount] = p + 1;
        *p = 0;
        stringsCount++;
      }
      p++;
    }

    char buffer2[MAX_PATH];
    strcpy(buffer2, SOURCE_PATH);
    strcat(buffer2, "source/");
    strcat(buffer2, strings[stringsCount - 1]);

    FILE * out = fopen(buffer2, "w");
    if (out)
    {
      int bytesRead = 0;
      char ch;
      while (Error_IsEmpty(error))
      {
        if (HttpConnection_GetChar(pCon, &ch, error))
        {
          fputc(ch, out);
          bytesRead++;
          if (bytesRead == pCon->ContentLength)
          {
            break;
          }
        }
      }

      fclose(out);
    }
  }

}

static void HandleConnection(enum TASK_ACTION action, void* pData)
{
  AddPost(L"Thread/%d HandleConnection", (int)GetCurrentThreadId());

  struct HttpConnection* pCon = *((struct HttpConnection**) pData);
  if (pCon)
  {
    struct Error error = ERROR_INIT;

    //https://www.w3.org/Protocols/rfc2616/rfc2616-sec5.html#sec5.1
    char bufferLeft[200];
    char bufferRight[200];
    //Method SP
    ReadUntil(pCon, ' ', bufferLeft, sizeof(bufferLeft), &error);
    if (strcmp(bufferLeft, "get") == 0)
    {
      pCon->Method = HTTP_METHOD_GET;
    }
    else if (strcmp(bufferLeft, "post") == 0)
    {
      pCon->Method = HTTP_METHOD_POST;
    }

    //Request-URI SP
    ReadUntil(pCon, ' ', pCon->uri, sizeof(pCon->uri), &error);

    //HTTP-Version CRLF
    ReadUntil(pCon, '\r', bufferLeft, sizeof(bufferLeft), &error);
    ReadUntil(pCon, '\n', bufferLeft, sizeof(bufferLeft), &error);


    //https://www.w3.org/Protocols/rfc2616/rfc2616-sec4.html#sec4.2

    //Le os headers
    while (Error_IsEmpty(&error))
    {
      if (!ReadLine(pCon, bufferLeft, bufferRight, 1000, &error))
      {
        break;
      }

      if (strcmp(bufferLeft, "connection") == 0)
      {
        //https://tools.ietf.org/html/rfc7230#section-6.1
        if (strcmp(bufferRight, "keep-alive") == 0)
        {
          pCon->bKeepAlive = true;
        }
        else if (strcmp(bufferRight, "close") == 0)
        {
          pCon->bKeepAlive = false;
        }
      }
      else if (strcmp(bufferLeft, "content-length") == 0)
      {
        pCon->ContentLength = atoi(bufferRight);
      }

      if (bufferLeft[0] == 0)
      {
        //empty line
        break;
      }
    }

    if (Error_IsEmpty(&error))
    {
      if (pCon->Method == HTTP_METHOD_GET &&
        strcmp(pCon->uri, "/edit/func/1") == 0)
      {
        AddPost(L"Thread/%d %s", (int)GetCurrentThreadId(), pCon->uri);
        Call(pCon, 1, &error);
      }
      else if (pCon->Method == HTTP_METHOD_GET)
      {
        AddPost(L"Thread/%d send file %s", (int)GetCurrentThreadId(), pCon->uri);
        
        if (strncmp(pCon->uri, "/html", 5) == 0)
        {
          //aqui vou ver se eh edit ou srce
          HttpConnection_SendFile(pCon, pCon->uri, SOURCE_PATH, &error);
        }
        else if (strncmp(pCon->uri, "/source", 5) == 0)
        {
          //aqui vou ver se eh edit ou srce
          HttpConnection_SendFile(pCon, pCon->uri, SOURCE_PATH, &error);
        }
        else if (strncmp(pCon->uri, "/edit", 5) == 0)
        {
          //aqui vou ver se eh edit ou srce
          HttpConnection_SendFile(pCon, pCon->uri, ROOT_PATH, &error);
        }
        

        if (Error_IsEmpty(&error))
        {
          if (pCon->bKeepAlive)
          {
            //Reseta o header e reaproveita a conex�o
            pCon->bKeepAlive = false;
            pCon->uri[0] = 0;
            pCon->Method = HTTP_METHOD_NONE;

            Socket_SetTimeout(pCon->Socket, 3000);
            HandleConnection(action, pData);
          }
          else
          {
            //ok pode fechar
            HttpConnection_Delete(pCon);
          }
        }
        else
        {
          //enviar error
          HttpConnection_Delete(pCon);
        }

      }

      else if (pCon->Method == HTTP_METHOD_POST)
      {
        SaveFile(pCon, &error);
        HttpConnection_Delete(pCon);
      }


    }
    else
    {
      //ok pode fechar
      HttpConnection_Delete(pCon);
    }
  }
}

static void HttpServer_Loop(enum TASK_ACTION action, void* pData)
{
  if (action == TASK_RUN)
  {
    struct HttpServer* pHttpServer = (*(struct HttpServer**) pData);

    //for (;;)
    {
      AddPost(L"Thread/%d Socket_IsReadyToReceive", (int)GetCurrentThreadId());
      if (Socket_IsReadyToReceive(pHttpServer->listenSocket, 10000))
      {
        Socket socket = Socket_Accept(pHttpServer->listenSocket, NULL, NULL);
        if (socket == INVALID_SOCKET)
        {
          //socketerror = WSAGetLastError();
          //ASSERT(false);
        }
        else
        {
          int on = 1;
          setsockopt(socket,
            SOL_SOCKET,
            SO_KEEPALIVE,
            (const char*)&on,
            sizeof(on));

          Socket_SetTimeout(socket, 3000);
          struct Error error = ERROR_INIT;
          struct HttpConnection* pCon = HttpConnection_Create(socket, pHttpServer->m_ctx, &error);
          if (pCon)
          {
            ThreadPool_Push(NULL, HandleConnection, &pCon, sizeof(pCon));
          }
        }
      }
    }
    AddPost(L"Thread/%d Sleep(10)", (int)GetCurrentThreadId());
    Sleep(10);
    //Socket_Close(listenSocket);
    ThreadPool_Push(NULL, HttpServer_Loop, pData, sizeof(void*));
  }
}




bool HttpServer_Run(struct HttpServer* httpServer)
{
  return true;
}

static void SetSecurity(SSL_CTX *ctx, enum SECURITY securityVersion)
{
  //https://en.wikipedia.org/wiki/Template:TLS/SSL_support_history_of_web_browsers
  if (securityVersion == SECURITY_VERSION_TLS12)
  {
    SSL_CTX_set_options(ctx, SSL_OP_NO_SSLv2 |
      SSL_OP_NO_SSLv3 |
      SSL_OP_NO_TLSv1 |
      SSL_OP_NO_TLSv1_1);
  }
  else if (securityVersion == SECURITY_VERSION_TLS11)
  {
    SSL_CTX_set_options(ctx, SSL_OP_NO_SSLv2 |
      SSL_OP_NO_SSLv3 |
      SSL_OP_NO_TLSv1);
  }
  else if (securityVersion == SECURITY_VERSION_TLS1)
  {
    SSL_CTX_set_options(ctx, SSL_OP_NO_SSLv2 |
      SSL_OP_NO_SSLv3);
  }
  else if (securityVersion == SECURITY_VERSION_SSL3)
  {
    SSL_CTX_set_options(ctx, SSL_OP_NO_SSLv2);
  }
}

bool HttpServer_Init(struct HttpServer* httpServer,
  enum SECURITY securityVersion,
  const char* port,
  const char* strsslCertificate,
  const char* strsslPrivateKey,
  struct Error* error)
{

  httpServer->m_ctx = NULL;

  if (securityVersion != SECURITY_VERSION_NONE)
  {
    SSL_library_init();
    SSL_load_error_strings();

    //https://www.openssl.org/docs/ssl/SSL_CTX_new.html
    SSL_METHOD* meth = (SSL_METHOD*)SSLv23_server_method();
    SSL_CTX* ctx = SSL_CTX_new(meth);

    if (ctx == NULL)
    {
      Error_Set(error, "Error creating sslcontext");
      return false;
    }

    SetSecurity(ctx, securityVersion);

    if (!(SSL_CTX_use_certificate_chain_file(ctx, strsslCertificate)))
    {
      Error_Set(error, "Can't read certificate file");
      return false;
    }

    if (!(SSL_CTX_use_PrivateKey_file(ctx, strsslPrivateKey, SSL_FILETYPE_PEM)))
    {
      Error_Set(error, "Can't read key file");
      return false;
    }

    httpServer->m_ctx = ctx;
  }


  struct addrinfo* ai = NULL;

  struct addrinfo hints;
  ZeroMemory(&hints, sizeof(hints));
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_protocol = IPPROTO_TCP;
  hints.ai_flags = AI_PASSIVE;


  //The getaddrinfo function provides protocol-independent translation 
  //from an ANSI host name to an address.
  int iRes = getaddrinfo(
    NULL,
    port,
    &hints,
    &ai);

  //Success returns zero.
  //Failure returns a nonzero Windows Sockets error code, as found in 
  //the Windows Sockets Error Codes.
  if (iRes != 0)
  {
    //ja tem o erro direto
  }

  httpServer->listenSocket = Socket_Create(ai->ai_family,
    ai->ai_socktype,
    ai->ai_protocol);

  if (!httpServer->listenSocket)
  {
    //erro
  }

  int i = Socket_Bind(httpServer->listenSocket, ai->ai_addr, ai->ai_addrlen);
  if (i == SOCKET_ERROR)
  {
    //socketerror = WSAGetLastError();
  }
  int socketerror = 0;
  iRes = Socket_Listen(httpServer->listenSocket, SOMAXCONN);
  if (iRes == SOCKET_ERROR)
  {
    //  If the listen function is called on an already listening socket,
    //  it will return success without changing the value for the
    //  backlog parameter. Setting the backlog parameter to 0 in a
    //  subsequent call to listen on a listening socket is not
    //  considered a proper reset, especially if there are
    //  connections on the socket.

    //ASSERT(false);
    //std::wstring strError = GetSocketErrorW(socketerror);
    //SendHttpServerError(strError);
    //return;
  }

  ThreadPool_Push(NULL, HttpServer_Loop, &httpServer, sizeof(void*));
  //HttpServer_Loop

  return true;

}