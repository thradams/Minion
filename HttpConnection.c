#include <winsock2.h>
#include <Windows.h>
#include <assert.h>
#include "HttpConnection.h"
#include "board.h"
#include <sys/types.h>
#include <sys/stat.h>

extern char ROOT_PATH[100];

bool HttpConnection_Init(struct HttpConnection* connection, Socket socket, SSL_CTX *ctx, struct Error* error)
{
  bool bIsValid = false;
  connection->pHttpServer = NULL;
  connection->ContentLength = 0;
  connection->bKeepAlive = false;
  connection->uri[0] = 0;
  connection->Method = HTTP_METHOD_NONE;

  connection->ssl = NULL;
  connection->io = NULL;
  connection->ssl_bio = NULL;
  connection->sbio = NULL;

  connection->Socket = socket;

  if (ctx)
  {
    BIO* sbio = BIO_new_socket((int)socket, BIO_NOCLOSE);
    if (sbio != NULL)
    {
      SSL* ssl = SSL_new(ctx);
      if (ssl != NULL)
      {
        SSL_set_bio(ssl, sbio, sbio);

        BIO* io = BIO_new(BIO_f_buffer());
        if (io != NULL)
        {
          BIO* ssl_bio = BIO_new(BIO_f_ssl());
          if (ssl_bio != NULL)
          {
            BIO_set_ssl(ssl_bio, ssl, BIO_CLOSE);
            BIO_push(io, ssl_bio);
            int ssl_error = SSL_accept(ssl);
            if (ssl_error <= 0)
            {
              //https://www.openssl.org/docs/ssl/SSL_get_error.html
              auto reason = SSL_get_error(ssl, ssl_error);
              switch (reason)
              {
              case SSL_ERROR_NONE:
                //The TLS / SSL I / O operation completed.
                //This result code is returned if and only if ret > 0.
                break;
              case SSL_ERROR_ZERO_RETURN:
                Error_Set(error, "The TLS / SSL connection has been closed");
                //The TLS / SSL connection has been closed.
                //If the protocol version is SSL 3.0 or TLS 1.0, this result code
                //is returned only if a closure alert has occurred in the protocol,
                //i.e. if the connection has been closed cleanly.
                //Note that in this case SSL_ERROR_ZERO_RETURN does not necessarily 
                //indicate that the underlying transport has been closed.
                break;

              case SSL_ERROR_WANT_READ:
              case SSL_ERROR_WANT_WRITE:
                Error_Set(error, "SSL_ERROR_WANT_READ/SSL_ERROR_WANT_WRITE");
                //The operation did not complete; 
                //the same TLS / SSL I / O function should be called again later.
                //If, by then, the underlying BIO has data available for 
                //reading(if the result code is SSL_ERROR_WANT_READ) 
                //or allows writing data(SSL_ERROR_WANT_WRITE), 
                //then some TLS / SSL protocol progress will take place, 
                //i.e.at least part of an TLS / SSL record will be read 
                //or written.Note that the retry may again lead to 
                //a SSL_ERROR_WANT_READ or SSL_ERROR_WANT_WRITE condition.
                //There is no fixed upper limit for the number of iterations 
                //that may be necessary until progress becomes visible 
                //at application protocol level.
                break;

              case SSL_ERROR_WANT_CONNECT:
              case SSL_ERROR_WANT_ACCEPT:
                Error_Set(error, "SSL_ERROR_WANT_CONNECT/SSL_ERROR_WANT_ACCEPT");
                //The operation did not complete; 
                //the same TLS / SSL I / O function should be called again later.
                //The underlying BIO was not connected yet to the peer and 
                //the call would block in connect() / accept().
                //The SSL function should be called again when the connection 
                //is established.
                //These messages can only appear with a BIO_s_connect() 
                //or BIO_s_accept() BIO, respectively.
                //In order to find out, when the connection has 
                //been successfully established, on many platforms 
                //select() or poll() for writing on the socket file descriptor 
                //can be used.
                break;

              case SSL_ERROR_WANT_X509_LOOKUP:
                Error_Set(error, "SSL_ERROR_WANT_X509_LOOKUP");
                //The operation did not complete because an application callback
                //set by SSL_CTX_set_client_cert_cb() has asked to be called 
                //again.The TLS / SSL I / O function should be called again later.
                //Details depend on the application.
                break;

              case SSL_ERROR_SYSCALL:
                Error_Set(error, "SSL_ERROR_SYSCALL");
                //Some I / O error occurred.The OpenSSL error queue may contain 
                //more information on the error.If the error queue 
                //is empty(i.e.ERR_get_error() returns 0), ret can be used 
                //to find out more about the error : If ret == 0, an EOF was
                //observed that violates the protocol.If ret == -1, the
                //underlying BIO reported an I / O error(for socket I / O on
                //Unix systems, consult errno for details).
                break;
              case SSL_ERROR_SSL:
                Error_Set(error, "SSL_ERROR_SSL");
                //A failure in the SSL library occurred, usually a protocol error.
                //The OpenSSL error queue contains more information on the error.
                break;
              default:
                Error_Set(error, "SSL_ERROR ?");
                assert(false);
                break;
              }

              //m_bIsValid = false;
            }
            else
            {
              //commit
              bIsValid = true;
              connection->ssl = ssl;
              connection->io = io;
              connection->ssl_bio = ssl_bio;
              connection->sbio = sbio;

              //m_stream.m_bio = m_io;
            }
            //automatico no sslfree
          }
          else
          {
            Error_Set(error, "error creating bio2");
          }

          if (!bIsValid)
          {
            BIO_free(io);
          }
        }
        else
        {
          Error_Set(error, "error creating bio");
        }

        if (!bIsValid)
        {
          SSL_shutdown(ssl);
          SSL_free(ssl);
        }
      }
      else
      {
        Error_Set(error, "error creating ssl");
      }

      if (!bIsValid)
      {
        BIO_free(sbio);
      }
    }
    else
    {
      Error_Set(error, "error creating bio");
    }
  }
  return bIsValid;
}

struct HttpConnection* HttpConnection_Create(Socket socket, SSL_CTX *ssl, struct Error* error)
{
  struct HttpConnection *p = (struct HttpConnection *) malloc(sizeof * p);
  if (p)
  {
    HttpConnection_Init(p, socket, ssl, error);
  }
  else
  {
    Error_Set(error, "out of memory");
  }
  return p;
}

void HttpConnection_CloseGracefully(struct HttpConnection* connection)
{
  if (connection->sbio != NULL)
  {
    BIO_free(connection->sbio);
    connection->sbio = NULL;
  }

  if (connection->io != NULL)
  {
    BIO_free(connection->io);
    connection->io = NULL;
  }

  if (connection->ssl != NULL)
  {
    int r = SSL_shutdown(connection->ssl);
    if (!r)
    {
      // If we called SSL_shutdown() first then
      // we always get return value of '0'. In
      //this case, try again, but first send a
      //TCP FIN to trigger the other side's
      // close_notify
      shutdown(connection->Socket, 1);
      r = SSL_shutdown(connection->ssl);
    }

    SSL_free(connection->ssl);
    //limpa tb o bio
    connection->ssl = NULL;
  }
  Socket_CloseGracefully(connection->Socket);
  //m_socket.CloseSocketGracefully();
}

void HttpConnection_Delete(struct HttpConnection* connection) /*@default*/
{
  if (connection)
  {
    HttpConnection_CloseGracefully(connection);
    free((void*)connection);
  }
}

bool HttpConnection_GetChar(struct HttpConnection* connection, char *ch, struct Error* error)
{
  if (!Error_IsEmpty(error))
  {
    return false;
  }

  int result = 0;

  if (connection->sbio)
  {
    result = BIO_read(connection->io, ch, 1);
    if (result != 1)
      Error_Set(error, "error");
  }
  else
  {
    result = recv(connection->Socket, ch, 1, 0);
    if (result > 0)
    {
      //ok
    }

    if (result == 0)
    {
      //closed
      //result = 0;
      Error_Set(error, "socket closed");
    }

    if (result == SOCKET_ERROR)
    {
      int socketError = WSAGetLastError();
      Error_Set(error, "%s", GetSocketErrorA(socketError));
      //if (socketError == 0)
      //{
        //ASSERT(false);
        //socketError = 1;
      //}
    }
  }
  return Error_IsEmpty(error);
}

size_t HttpConnection_PushBytes(struct HttpConnection* connection, const char* bytes, size_t len, struct Error* error)
{
  size_t r = 0;
  if (connection->io != NULL)
  {
    r = BIO_write(connection->io, bytes, (int)len);
    if (r > 0)
    {
      int r2 = BIO_flush(connection->io);
      if (r2 != 1)
      {
        //returns 1 for success and 0 or -1 for failure.
        r = 0;
      }
    }
    return r;
  }
  else
  {
    r = Socket_PushBytes(connection->Socket, bytes, len);
  }
  return r;
}

const char* GetMimeType(const char* name)
{
  char* ext = strrchr((char*)name, '.');

  if (!ext)
    return "";

  if (strcmp(ext, ".html") == 0 || strcmp(ext, ".htm") == 0)
    return "text/html; charset=UTF-8";

  if (strcmp(ext, ".jpg") == 0 || strcmp(ext, ".jpeg") == 0)
    return "image/jpeg";

  if (strcmp(ext, ".gif") == 0)
    return "image/gif";

  if (strcmp(ext, ".png") == 0)
    return "image/png";

  if (strcmp(ext, ".ico") == 0)
    return "image/ico";

  if (strcmp(ext, ".css") == 0)
    return "text/css; charset=UTF-8";

  if (strcmp(ext, ".json") == 0)
    return "text/json; charset=utf-8";

  if (strcmp(ext, ".au") == 0)
    return "audio/basic";

  if (strcmp(ext, ".wav") == 0)
    return "audio/wav";

  if (strcmp(ext, ".avi") == 0)
    return "video/x-msvideo";

  if (strcmp(ext, ".mpeg") == 0 || strcmp(ext, ".mpg") == 0)
    return "video/mpeg";

  if (strcmp(ext, ".mp3") == 0)
    return "audio/mpeg";

  if (strcmp(ext, ".js") == 0)
    return "application/javascript; charset=UTF-8";

  if (strcmp(ext, ".map") == 0)
    return "application/x-navimap";

  if (strcmp(ext, ".ts") == 0)
    return "text/x.typescript; charset=UTF-8";

  if (strcmp(ext, ".svg") == 0)
    return "image/svg+xml; charset=UTF-8";

  if (strcmp(ext, ".woff") == 0)
    return "application/x-font-woff";

  if (strcmp(ext, ".eot") == 0)
    return "application/vnd.ms-fontobject";

  if (strcmp(ext, ".ttf") == 0)
    return "application/octet-stream";

  if (strcmp(ext, ".otf") == 0)
    return "application/x-font-opentype";

  if (strcmp(ext, ".txt") == 0)
    return "text/plain; charset=UTF-8";

  if (strcmp(ext, ".cer") == 0)
    return "application/x-x509-ca-cert";

  if (strcmp(ext, ".crt") == 0)
    return "application/x-x509-ca-cert";

  if (strcmp(ext, ".cab") == 0)
    return "application/cab, application/x-cabinet";

  if (strcmp(ext, ".exe") == 0)
    return "application/x-msdownload";

  if (strcmp(ext, ".zip") == 0)
    return "application/zip";

  //http://reference.sitepoint.com/html/mime-types-full
  //ASSERT(false);
  return "";
}

bool HttpConnection_SendJson(struct HttpConnection* connection, const char* jsonString, struct Error* error)
{
    int len = strlen(jsonString);
    char buffer[500];
    int bufferSize = 500;
    int number_characters_written = sprintf_s(buffer,
        bufferSize,
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/json;charset=utf-8\r\n"
        "Content-Length: %d\r\n"
        "Connection: %s\r\n"
        "\r\n",
        len,
        (connection->bKeepAlive ? "keep-alive" : "close"));
    
    HttpConnection_PushBytes(connection, buffer, number_characters_written, error);
    HttpConnection_PushBytes(connection, jsonString, strlen(jsonString), error);
}
bool HttpConnection_SendOK(struct HttpConnection* connection, struct Error* error)
{  
    if (connection->bKeepAlive)
    {
        const char m[] = "HTTP/1.1 204 OK\r\n"
            "Connection: keep-alive\r\n\r\n";
        HttpConnection_PushBytes(connection, m, sizeof(m), error);
    }
    else
    {
        const char m[] = "HTTP/1.1 204 OK\r\n"
            "Connection: close\r\n\r\n";
        HttpConnection_PushBytes(connection, m, sizeof(m), error);
    }
  return Error_IsEmpty(error);
}

bool HttpConnection_SendFile(struct HttpConnection* connection,
  const char* fileName0,
  const char* ROOT_PATH,
  struct Error* error)
{

#ifdef BOARD
  AddPost(L"Thread/%d HttpConnection_SendFile", (int)GetCurrentThreadId());
#endif

  if (!Error_IsEmpty(error))
  {
    return false;
  }
  char fileName[500] = { 0 };
  strcpy(fileName, ROOT_PATH);
  const char* p = fileName0;
  while (*p && *p != '?')
  {
    strncat(fileName, p, 1);
    p++;
  }


  struct _stat info;

  if (fileName != NULL)
  {

    time_t curtime = time(NULL);
    char currentTimeUTC[200];
    struct tm s1;
    gmtime_s(&s1, &curtime);
    strftime(currentTimeUTC, 200, "%a, %d %b %Y %H:%M:%S GMT", &s1);


    char lastModified[200];
    if (_stat(fileName, &info) == 0)
    {
      struct tm s;
      gmtime_s(&s, &info.st_mtime);
      strftime(lastModified, 200, "%a, %d %b %Y %H:%M:%S GMT", &s);

      if (fileName[0] == '/')
      {
        //fileName++; //skip this char
      }

      FILE* requested_file = fopen(fileName, "rb");

      if (requested_file != NULL)
      {
        if (fseek(requested_file, 0, SEEK_END) == 0)
        {
          size_t fileLength = ftell(requested_file);
          rewind(requested_file);

          size_t maxheaderlen = 500;
          size_t bufferSize = fileLength + maxheaderlen;
          char* sendbuf = malloc(sizeof(char)* bufferSize);
          if (sendbuf != NULL)
          {
            int number_characters_written = 0;


            if (lastModified[0] = 0)
            {
              number_characters_written = sprintf_s(sendbuf,
                bufferSize,
                "HTTP/1.1 200 OK\r\n"
                "Date: %s\r\n"
                "Cache-Control: max-age=0\r\n"
                "Content-Type: %s\r\n"
                "Content-Length: %d\r\n"
                "Access-Control-Allow-Origin: *\r\n"
                "Access-Control-Allow-Methods: GET\r\n"
                "Connection: %s\r\n"
                "\r\n",
                currentTimeUTC,
                (const char*)GetMimeType(fileName),
                (int)fileLength,
                (connection->bKeepAlive ? "keep-alive" : "close"));
            }
            else
            {
              number_characters_written = sprintf_s(sendbuf,
                bufferSize,
                "HTTP/1.1 200 OK\r\n"
                "Date: %s\r\n"
                "Last-Modified: %s\r\n"
                "Cache-Control: max-age=0\r\n"
                "Content-Type: %s\r\n"
                "Content-Length: %d\r\n"
                "Access-Control-Allow-Origin: *\r\n"
                "Access-Control-Allow-Methods: GET\r\n"
                "Connection: %s\r\n"
                "\r\n",
                currentTimeUTC,
                lastModified,
                (const char*)GetMimeType(fileName),
                (int)fileLength,
                (connection->bKeepAlive ? "keep-alive" : "close"));
            }

            if (number_characters_written != -1)
            {
              const size_t headerlen = number_characters_written;

              //ASSERT(bufferSize > fileLength + headerlen);

              size_t result = fread(sendbuf + headerlen,
                sizeof(char),
                fileLength,
                requested_file);

              if (result >= fileLength)
              {
                size_t sendLen = fileLength + headerlen;
                HttpConnection_PushBytes(connection, sendbuf, sendLen, error);
              }
              else
              {
                Error_Set(error, "fread");
              }
            }
            else
            {
              Error_Set(error, "no space");
            }
            free(sendbuf);
          }
          else
          {
            Error_Set(error, "malloc fail");
          }
        }
        else
        {
          Error_Set(error, "seek error");
        }
        fclose(requested_file);
      }
      else
      {
        Error_Set(error, "file not open");
      }
    }
    else
    {
      Error_Set(error, "not found?");
    }
  }
  else
  {
    Error_Set(error, "invalid file name");
  }

  return Error_IsEmpty(error);
}


bool HttpConnection_SendFileIfModified(struct HttpConnection* connection,
  const char* fileName,
  const char* ROOT_PATH,
  time_t ifmodifiedtime,
  bool bKeepAlive,
  struct Error* error)
{
  if (fileName != NULL)
  {
    if (fileName[0] == '/')
    {
      //fileName++; //skip this char
    }

    struct _stat info;

    _stat(fileName, &info);

    time_t curtime = time(NULL);
    char currentTimeUTC[200];
    struct tm s1;
    gmtime_s(&s1, &curtime);
    strftime(currentTimeUTC, 200, "%a, %d %b %Y %H:%M:%S GMT", &s1);

    if (info.st_mtime <= ifmodifiedtime)
    {
      char buffer[200];
      int number_characters_written = sprintf_s(buffer,
        "HTTP/1.1 304 Not Modified\r\n"
        "Date: %s\r\n"
        "Connection: %s\r\n"
        "\r\n",
        currentTimeUTC,
        (bKeepAlive ? "keep-alive" : "close"));

      if (number_characters_written != -1)
      {
        HttpConnection_PushBytes(connection, buffer, number_characters_written, error);
      }
      else
      {
        HttpConnection_SendFile(connection, fileName, ROOT_PATH, error);
      }
    }
    else
    {
      Error_Set(error, "e");
    }
  }
  else
  {
    Error_Set(error, "e");
  }
  return Error_IsEmpty(error);
}


void SaveFile(struct HttpConnection* pCon,
    const char* SOURCE_PATH,
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
        strcat(buffer2, "/source/");
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
