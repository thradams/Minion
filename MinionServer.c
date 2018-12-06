
#include "MinionServer.h"
#include "AppBuild.h"
#include "Board.h"

#define container_of(ptr, type, member) ((type *)((char *)(ptr) - offsetof(type, member)))


/* Push file as a buffer. */
bool fileio_push_file_buffer(duk_context *ctx, const char *filename)
{
  FILE *f = NULL;
  long len;
  void *buf;
  size_t got;

  if (!filename) {
    goto error;
  }

  f = fopen(filename, "rb");
  if (!f) {
    goto error;
  }

  if (fseek(f, 0, SEEK_END) != 0) {
    goto error;
  }

  len = ftell(f);

  if (fseek(f, 0, SEEK_SET) != 0) {
    goto error;
  }

  buf = duk_push_fixed_buffer(ctx, (size_t)len);

  got = fread(buf, 1, len, f);
  if (got != (size_t)len) {
    duk_pop(ctx);
    goto error;
  }

  fclose(f);
  return true;

error:
  if (f) {
    fclose(f);
  }
  //duk_push_undefined(ctx);
  return false;
}

/* Push file as a string. */
bool fileio_push_file_string(duk_context *ctx, const char *filename)
{
  bool b = fileio_push_file_buffer(ctx, filename);
  if (b)
  {
    if (duk_is_buffer(ctx, -1))
    {
      duk_buffer_to_string(ctx, -1);
    }
  }
  return b;
}

struct RunScriptCapture
{
  struct MinionServer* pMinionServer;
};

//Native Print function
static duk_ret_t NativePrint(duk_context *ctx) {
  duk_push_string(ctx, " ");
  duk_insert(ctx, 0);
  duk_join(ctx, duk_get_top(ctx) - 1);
  printf("%s\n", duk_safe_to_string(ctx, -1));
  return 0;
}
static duk_ret_t NCallback(duk_context *ctx)
{
  //Native helper to receive the JS call see Make
  void* p1 = duk_require_pointer(ctx, 0);
  void* p2 = duk_require_pointer(ctx, 1);
  void(*callback)(void*) = (void(*)(void*))p1;
  callback(p2);
  return 0;
}

void SendValueBack(void* pData)
{
  //Send the answer to the client side
  //the client side will run the callback
  struct HttpConnection* pCon = (struct HttpConnection*)pData;
  struct Error error = ERROR_INIT;
  HttpConnection_SendOK(pCon, &error);
  HttpServer_ConnectionSink(pCon);
  
}

void RunScript(enum TASK_ACTION action, void* pData)
{
  struct HttpConnection* pCon = *((struct HttpConnection**)pData);
  struct MinionServer* pMinionServer = container_of(pCon->pHttpServer, struct MinionServer, HttpServer);

  if (action == TASK_RUN)
  {
    if (duk_get_global_string(pMinionServer->pDukContext, "func")) 
    {
      //name the function pushed
      duk_push_string(pMinionServer->pDukContext, "teste"); //arguments are push ..

    //now we create the javascript closure
    //that will call the native callback that
    //sends the answer to the client
      if (duk_get_global_string(pMinionServer->pDukContext, "Make"))
      {
        //https://wiki.duktape.org/HowtoFinalization.html
        duk_push_pointer(pMinionServer->pDukContext, (void*)SendValueBack);
        duk_push_pointer(pMinionServer->pDukContext, (void*)pCon);
        duk_call(pMinionServer->pDukContext, 2);
      }
      //call the function.
      duk_call(pMinionServer->pDukContext, 2);
    }
    //duk_eval_string(p->pMinionServer->pDukContext, "1+2");
    //int res = duk_get_int(p->pMinionServer->pDukContext, -1);
  }
}
void MinionServerConnectionHandler(struct HttpConnection* pCon)
{
  struct MinionServer* pMinionServer = container_of(pCon->pHttpServer, struct MinionServer, HttpServer);

  struct Error error = ERROR_INIT;
  if (pCon->Method == HTTP_METHOD_GET &&
    strcmp(pCon->uri, "/edit/func/1") == 0)
  {
    Actor_Post(&pMinionServer->Actor, RunScript, &pCon, sizeof(pCon));
    //BuildApp(pMinionServer->SOURCE_PATH, pMinionServer->ROOT_PATH);
    //AddPost(L"Thread/%d %s", (int)GetCurrentThreadId(), pCon->uri);
    //Call(pCon, 1, &error);
    //HttpConnection_SendOK(pCon, &error);

    //HttpServer_ConnectionSink(pCon);
  }
  else if (pCon->Method == HTTP_METHOD_GET)
  {
#ifdef BOARD
    AddPost(L"Thread/%d send file %s", (int)GetCurrentThreadId(), pCon->uri);
#endif
    if (strcmp(pCon->uri, "/html/index.html") == 0)
    {
      //Build completo
      BuildApp(pMinionServer->SOURCE_PATH, pMinionServer->ROOT_PATH);

      HttpConnection_SendFile(pCon, pCon->uri, pMinionServer->SOURCE_PATH, &error);
      HttpServer_ConnectionSink(pCon);
    }
    else if (strncmp(pCon->uri, "/html", 5) == 0)
    {
      //BuildApp(pMinionServer->SOURCE_PATH, pMinionServer->ROOT_PATH);

      //aqui vou ver se eh edit ou srce
      HttpConnection_SendFile(pCon, pCon->uri, pMinionServer->SOURCE_PATH, &error);
      HttpServer_ConnectionSink(pCon);
    }
    else if (strncmp(pCon->uri, "/source", 5) == 0)
    {
      //aqui vou ver se eh edit ou srce
      HttpConnection_SendFile(pCon, pCon->uri, pMinionServer->SOURCE_PATH, &error);
      HttpServer_ConnectionSink(pCon);
    }
    else if (strncmp(pCon->uri, "/edit", 5) == 0)
    {
      //aqui vou ver se eh edit ou srce
      HttpConnection_SendFile(pCon, pCon->uri, pMinionServer->ROOT_PATH, &error);
      HttpServer_ConnectionSink(pCon);
    }
  }
  else if (pCon->Method == HTTP_METHOD_POST)
  {


    SaveFile(pCon, pMinionServer->SOURCE_PATH, &error);
    HttpConnection_SendOK(pCon, &error);
    HttpServer_ConnectionSink(pCon);
    //HttpConnection_Delete(pCon);
  }
}


bool MinionServer_Init(struct MinionServer* server,
                       const char* sourcePath,
                       const char* rootPath, 
                       struct Error* error)
{
  server->ROOT_PATH[0] = 0;
  strcat(server->ROOT_PATH, rootPath);
  server->SOURCE_PATH[0] = 0;
  strcat(server->SOURCE_PATH, sourcePath);

  Actor_Init(&server->Actor, 100);

  server->pDukContext = duk_create_heap_default();

  duk_push_c_function(server->pDukContext, NativePrint, DUK_VARARGS);
  duk_put_global_string(server->pDukContext, "print");

  duk_push_c_function(server->pDukContext, NCallback, DUK_VARARGS);
  duk_put_global_string(server->pDukContext, "NCallback");

  

  duk_eval_string(server->pDukContext, "function Make(a, b) { return function() { var a1 = a; var b1 = b; NCallback(a1, b1);}}");

  char sjs[200] = { 0 };
  strcat(sjs, server->SOURCE_PATH);
  strcat(sjs, "/server/server.js");

  if (fileio_push_file_string(server->pDukContext, sjs))
  {
    if (duk_peval(server->pDukContext) != 0)
    {
      //falhou
      const char* errors = duk_safe_to_string(server->pDukContext, -1);
    }
    else
    {
  
    }
    duk_pop(server->pDukContext);
  }
  else
  {
  
  }


  if (HttpServer_Init(&server->HttpServer,
    SECURITY_VERSION_NONE,
    MinionServerConnectionHandler,
    "8080",
    "my_test_cert.crt",
    "my_test_privkey.key",
    error))
  {

  }
  else
  {
  }
  return true;
}

void MinionServer_Destroy(struct MinionServer* server)
{
  duk_destroy_heap(server->pDukContext);
}

