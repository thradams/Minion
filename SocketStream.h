#pragma once
#include <openssl/ssl.h>

struct SocketStream
{
  BIO *bio; //nao � dono
};


void SocketStream_Init(struct SocketStream* s);