#include <stdio.h>
#include <stdlib.h>
#include "conio.h"

#include <stdio.h>
#include "Socket.h"
#include "TaskQueue.h"
#include "ThreadPool.h"
#include "Actor.h"
#include "UITask.h"
#include "Board.h"

#include "HttpServer.h"
#include "AppBuild.h"
//#include <Windows.h>

int s_screen_1_dirty = 0;
int s_screen_0_dirty = 0;

char rootDirectory[500] = { 0 };
#define PORT_NUMBER "8080"

char ROOT_PATH[100] = { 0 };
char SOURCE_PATH[100] = { 0 };


void Function(struct HttpConnection* pCon)
{
    struct Error error = ERROR_INIT;
    if (pCon->Method == HTTP_METHOD_GET &&
        strcmp(pCon->uri, "/edit/func/1") == 0)
    {
        BuildApp(SOURCE_PATH, ROOT_PATH);
        //AddPost(L"Thread/%d %s", (int)GetCurrentThreadId(), pCon->uri);
        //Call(pCon, 1, &error);
    }
    else if (pCon->Method == HTTP_METHOD_GET)
    {
#ifdef BOARD
        AddPost(L"Thread/%d send file %s", (int)GetCurrentThreadId(), pCon->uri);
#endif
        if (strcmp(pCon->uri, "/html/index.html") == 0)
        {
            //Build completo
            BuildApp(SOURCE_PATH, ROOT_PATH);

            HttpConnection_SendFile(pCon, pCon->uri, SOURCE_PATH, &error);
        }
        else if (strncmp(pCon->uri, "/html", 5) == 0)
        {
            BuildApp(SOURCE_PATH, ROOT_PATH);

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
    }
    else if (pCon->Method == HTTP_METHOD_POST)
    {
        SaveFile(pCon, SOURCE_PATH, &error);
        //HttpConnection_Delete(pCon);
    }
}

int main(int argc, char *argv[])
{

    if (argc < 2)
    {
        printf("please inform the name of the working folder");
        return 1;
    }

    strcat(SOURCE_PATH, argv[1]);
    strcat(ROOT_PATH, "./");

    char drive[_MAX_DRIVE];
    //char dir[_MAX_DIR];
    char fname[_MAX_FNAME];
    char ext[_MAX_EXT];

    _splitpath(argv[0], drive, rootDirectory, fname, ext);

    UIActor_Init();
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);


    ThreadPool_Init(NULL, 100);

    struct HttpServer httpServer;
    struct Error error = ERROR_INIT;

    if (HttpServer_Init(&httpServer,
        SECURITY_VERSION_NONE,
        Function,
        PORT_NUMBER,
        "my_test_cert.crt",
        "my_test_privkey.key",
        &error))
    {

    }
    else
    {
    }


    s_screen_0_dirty = 1;
    int screen_number = 0;
    for (;;)
    {
        UIActorProcess();


        switch (screen_number)
        {
        case 0:
            if (s_screen_0_dirty)
            {
                clrscr();
                printf("http server running " PORT_NUMBER " - press ESC to exit\n");
                printf("directory : %s\n", rootDirectory);
                printf("\n");
                printf("Edit: http://localhost:8080/edit/edit.html?name=tela1 \n");
                printf("Runtime: http://localhost:8080/html/index.html \n");
                printf("press b to build all\n");


                s_screen_0_dirty = false;
            }
            //s_screen_1_dirty = 0;
            break;
        case 1:
            if (s_screen_1_dirty)
            {
                clrscr();
                Board_Paint();
                s_screen_1_dirty = false;
            }
            break;
        }


        if (kbhit())
        {
            int k = getch();
            if (k == 'v')
            {
                screen_number = screen_number == 1 ? 0 : 1;
                s_screen_0_dirty = true;
                s_screen_1_dirty = true;
            }
            else if (k == 'b')
            {
                printf("building..\n");
                BuildApp(SOURCE_PATH, ROOT_PATH);
                printf("complete\n");
            }

            if (k == 27)
                break;

        }

        Sleep(500);
    }

    ThreadPool_Stop(NULL);
    ThreadPool_Join(NULL);
    UIActor_Destroy();

    WSACleanup();



    return 0;
}

