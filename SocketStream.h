#pragma once
#include <openssl/ssl.h>

struct SocketStream
{
  BIO *bio; //nao é dono
};


void SocketStream_Init(struct SocketStream* s);