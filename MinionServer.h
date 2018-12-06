#pragma once
#include "HttpServer.h"
#include "Error.h"
#include "Actor.h"
#include "duktape.h"

struct MinionServer
{
  struct Actor Actor;
  struct HttpServer HttpServer; 
  duk_context* pDukContext;
  char SOURCE_PATH[200];
  char ROOT_PATH[200];
};

bool MinionServer_Init(struct MinionServer* server, const char* sourcePath,
  const char* rootPath, struct Error* error);
void MinionServer_Destroy(struct MinionServer* server);
