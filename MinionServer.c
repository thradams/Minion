
#include "MinionServer.h"
#include "AppBuild.h"
#include "Board.h"
#include "JsonScanner.h"
#include <assert.h>
#include <stdlib.h>

#ifdef _WIN32
#else
#include <pthread.h>
#define GetCurrentThreadId pthread_self
#endif

#ifdef _WIN32
#else

char * _itoa( int value, char *buffer, int radix )
{
    snprintf(buffer, 100, "%d", value);
    return buffer;
}

#endif


#define container_of(ptr, type, member) ((type *)((char *)(ptr) - offsetof(type, member)))

typedef void(*NativeCallback)(void*);

void NativeLogin(const char* user,
                 const char* password, 
                 NativeCallback callback,
                 void* pData)
{
    callback(pData);
}

struct NativeLoginHandlerCallbackCapture
{
    const char CallbackName[50];
    duk_context *ctx;
};

void NativeLoginHandlerCallback(void* pData)
{
    struct NativeLoginHandlerCallbackCapture* pCapture =
        (struct NativeLoginHandlerCallbackCapture*) pData;

    duk_context *ctx = pCapture->ctx;

    
    char buffer[500];
    int bufferSize = 500;
    int number_characters_written = snprintf(buffer,
        bufferSize,
        "%s('', '\"NativeOK\"'); delete %s;",
        pCapture->CallbackName,
        pCapture->CallbackName);

    free(pData);
    duk_eval_string(ctx, buffer);
}

static duk_ret_t NativeLoginHandler(duk_context *ctx)
{
    //Native helper to receive the JS call see Make
    char* user = duk_require_string(ctx, 0);
    char* password = duk_require_string(ctx, 1);
    
    struct NativeLoginHandlerCallbackCapture* pCapture =
        malloc(sizeof * pCapture);

    strcpy(pCapture->CallbackName, "_callbackvar");
    static int count = 0;
    count++;
    
    _itoa(count, pCapture->CallbackName + sizeof("_callbackvar") - 1, 10);
    pCapture->ctx = ctx;
    duk_require_function(ctx, 2);
    duk_dup(ctx, 2);
    duk_put_global_string(ctx, pCapture->CallbackName);

    NativeLogin(user, password, NativeLoginHandlerCallback, (void*)pCapture);

    
    return 0;
}


/* Push file as a buffer. */
bool fileio_push_file_buffer(duk_context *ctx, const char *filename)
{
    FILE *f = NULL;
    long len;
    void *buf;
    size_t got;

    if (!filename)
    {
        goto error;
    }

    f = fopen(filename, "rb");
    if (!f)
    {
        goto error;
    }

    if (fseek(f, 0, SEEK_END) != 0)
    {
        goto error;
    }

    len = ftell(f);

    if (fseek(f, 0, SEEK_SET) != 0)
    {
        goto error;
    }

    buf = duk_push_fixed_buffer(ctx, (size_t)len);

    got = fread(buf, 1, len, f);
    if (got != (size_t)len)
    {
        duk_pop(ctx);
        goto error;
    }

    fclose(f);
    return true;

error:
    if (f)
    {
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
static duk_ret_t NativePrint(duk_context *ctx)
{
    duk_push_string(ctx, " ");
    duk_insert(ctx, 0);
    duk_join(ctx, duk_get_top(ctx) - 1);
    printf("%s\n", duk_safe_to_string(ctx, -1));
    return 0;
}
static duk_ret_t NCallback(duk_context *ctx)
{
    //Native helper to receive the JS call see Make
    char* errorString = duk_require_string(ctx, 0);
    const char* resultJsonString = duk_require_string(ctx, 1);    
    void* p1 = duk_require_pointer(ctx, 2);
    void* p2 = duk_require_pointer(ctx, 3);
    void(*callback)(const char* error, const char* json, void*) = 
        (void(*)(const char*, const char*, void*))p1;
    callback(errorString, resultJsonString, p2);
    return 0;
}

void SendValueBack(const char* errorString, const char* json, void* pData)
{
    //Send the answer to the client side
    //the client side will run the callback
    struct HttpConnection* pCon = (struct HttpConnection*)pData;
    struct Error error = ERROR_INIT;
    //HttpConnection_SendOK(pCon, &error);
    HttpConnection_SendJson(pCon, json, &error);
    HttpServer_ConnectionSink(pCon);

}

char* GetContent(struct HttpConnection* pCon, struct Error* error)
{
    char* ss = malloc(sizeof(char) * pCon->ContentLength + 1);
    char* s = ss;
    *s = 0;
    for (int i = 0; i < pCon->ContentLength; i++)
    {
        char ch = 0;
        HttpConnection_GetChar(pCon, &ch, error);
        *s = ch;
        s++;
        if (!Error_IsEmpty(error))
            break;
    }
    *s = 0;
    return ss;
}

void RunScript(enum TASK_ACTION action, void* pData)
{
    struct HttpConnection* pCon = *((struct HttpConnection**)pData);
    struct MinionServer* pMinionServer = container_of(pCon->pHttpServer, struct MinionServer, HttpServer);
    struct Error error = ERROR_INIT;

    if (action == TASK_RUN)
    {
        char* s = GetContent(pCon, &error);
        struct JsonScanner scanner = JSONSCANNER_INIT;
        JsonScanner_BuyString(&scanner, s);
        JsonScanner_Match(&scanner);//[

        int nArguments = 0;
        JsonScanner_Match(&scanner);//string
        if (duk_get_global_string(pMinionServer->pDukContext, scanner.Lexeme.c_str))
        {
            JsonScanner_Match(&scanner);//string

            while (scanner.Token == TK_COMMA)
            {
                JsonScanner_Match(&scanner);//,
                if (scanner.Token == TK_NUMBER)
                {
                    double dVal = atof(scanner.Lexeme.c_str);
                    duk_push_number(pMinionServer->pDukContext, dVal); //arguments are push ..
                    JsonScanner_Match(&scanner);
                    nArguments++;
                }
                else if (scanner.Token == TK_STRING)
                {
                    duk_push_string(pMinionServer->pDukContext, scanner.Lexeme.c_str); //arguments are push ..
                    JsonScanner_Match(&scanner);
                    nArguments++;
                }
                else
                {
                    //TODO
                    assert(false);
                }
            }            
        }

        if (Error_IsEmpty(&error))
        {          
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
            duk_call(pMinionServer->pDukContext, nArguments + 1);
        }
      
        JsonScanner_Destroy(&scanner);
    }
}


void MinionServerConnectionHandler(struct HttpConnection* pCon)
{
    struct MinionServer* pMinionServer = container_of(pCon->pHttpServer, struct MinionServer, HttpServer);

    struct Error error = ERROR_INIT;
    if (pCon->Method == HTTP_METHOD_POST &&
        strcmp(pCon->uri, "/call") == 0)
    {
        Actor_Post(&pMinionServer->Actor, RunScript, &pCon, sizeof(pCon));
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

    duk_push_c_function(server->pDukContext, NCallback, 4);
    duk_put_global_string(server->pDukContext, "NCallback");

    duk_push_c_function(server->pDukContext, NativeLoginHandler, 3);
    duk_put_global_string(server->pDukContext, "LoginNative");
    

    duk_eval_string(server->pDukContext, "function Make(a, b) { return function(error, json) { var a1 = a; var b1 = b; NCallback(error, json, a1, b1);}}");

    char sjs[200] = { 0 };
    strcat(sjs, server->SOURCE_PATH);
    strcat(sjs, "/server/server.js");

    if (fileio_push_file_string(server->pDukContext, sjs))
    {
        if (duk_peval(server->pDukContext) != 0)
        {
            //falhou
            //const char* errors = duk_safe_to_string(server->pDukContext, -1);
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
    return Error_IsEmpty(error);
}

void MinionServer_Destroy(struct MinionServer* server)
{
    duk_destroy_heap(server->pDukContext);
}

