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
#include "duktape.h"
#include "MinionServer.h"

//#include <Windows.h>

int s_screen_1_dirty = 0;
int s_screen_0_dirty = 0;

char rootDirectory[500] = { 0 };
#define PORT_NUMBER "8080"

char ROOT_PATH[100] = { 0 };
char SOURCE_PATH[100] = { 0 };



void RunApp(const char* appName)
{
    UIActor_Init();
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);


    ThreadPool_Init(NULL, 100);

    struct MinionServer minionServer;

    struct Error error = ERROR_INIT;

    if (MinionServer_Init(&minionServer, appName, "./", &error))
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
                //printf("building..\n");
                //BuildApp(SOURCE_PATH, ROOT_PATH);
                //printf("complete\n");
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

    MinionServer_Destroy(&minionServer);
}


int main(int argc, char *argv[])
{
    printf("Minions Castle\n");

    if (argc < 2)
    {
        printf("please inform the name of the working folder");
        return 1;
    }

    if (argc == 4 && (strcmp(argv[1], "create") == 0))
    {
        const char* templateName = argv[2];
        const char* appName = argv[3];
    }
    else if (argc == 3 && (strcmp(argv[1], "build") == 0))
    {
        const char* appName = argv[2];
        BuildApp(appName, "\.");
    }
    else if (argc == 3 && (strcmp(argv[1], "run") == 0))
    {
        const char* appName = argv[2];
        RunApp(appName);
    }
    else
    {
        printf("usage:\n"
            " create TemplateName AppName\n"
            " build AppName\n"
            " run AppName\n"
        );
    }



    return 0;
}

