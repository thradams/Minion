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
#include "fs.h"
#include <time.h>

#ifdef _WIN32
#else
#include <unistd.h>

void Sleep(int t)
{
    usleep(t);
}
#endif

int s_screen_1_dirty = 0;
int s_screen_0_dirty = 0;

char rootDirectory[500] = { 0 };
#define PORT_NUMBER "8080"

char ROOT_PATH[100] = { 0 };
char SOURCE_PATH[100] = { 0 };


///sudo service ssh start

bool RunApp(const char* rootPath, const char* appName, struct Error* error)
{
    //rootPath is the path were we can find the sources
    //that are unchanged like runtime.js

    UIActor_Init();
    SocketStaticInit();


    ThreadPool_Init(NULL, 100);

    struct MinionServer minionServer;



    if (MinionServer_Init(&minionServer, appName, rootPath, error))
    {
        clock_t  t;
        t = clock();

        s_screen_0_dirty = 1;
        s_screen_1_dirty = 1;
        int screen_number = 0;

        for (;;)
        {
            UIActorProcess();


            switch (screen_number)
            {
            case 0:
                if (s_screen_0_dirty)
                {
                    c_clrscr();
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

                    clock_t  t2;
                    t2 = clock();
                    if (( (t2 - t)) > CLOCKS_PER_SEC/5)
                    {
                        t = t2;
                        c_clrscr();
                        Board_Paint();
                        s_screen_1_dirty = false;
                    }

                }
                break;
            }


            if (c_kbhit())
            {
                int k = c_getch();
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
        }

    }
    else
    {

    }


    ThreadPool_Stop(NULL);
    ThreadPool_Join(NULL);
    UIActor_Destroy();

    SocketStaticDestroy();



    MinionServer_Destroy(&minionServer);
    return Error_IsEmpty(error);
}

void PrintHelp()
{
    printf("usage:\n"
        " create TemplateName App_Path\n"
        " build App_Path\n"
        " run App_Path\n"
    );
}
int main(int argc, char *argv[])
{
    char Drive[256];
    char Directory[256];
    char Filename[256];
    char Extension[256];

    fs_path_split(argv[0],
        Drive,
        Directory,
        Filename,
        Extension);

    printf("Minions Castle running at %s\n", Directory);

    if (argc < 2)
    {
        PrintHelp();
        return 1;
    }

    if (argc == 4 && (strcmp(argv[1], "create") == 0))
    {
        const char* templateName = argv[2];
        const char* appPath = argv[3];
        CreateApp(Directory, appPath);
    }
    else if (argc == 3 && (strcmp(argv[1], "build") == 0))
    {
        const char* appName = argv[2];
        BuildApp(appName, ".");
    }
    else if (argc == 3 && (strcmp(argv[1], "run") == 0))
    {
        struct Error error = ERROR_INIT;


        char currentPath[256] = { 0 };
        fs_current_path(currentPath);
        printf("Current Path:%s\n", currentPath);

#ifdef _WIN32
        const char* appName = argv[2];
        if (!RunApp("./", appName, &error))
        {

            printf("Error : %s", error.Msg);

        }
#else
        //home/tra/projects/MinionLinux/bin/x64/Debug/
        const char* appName = argv[2];
        if (!RunApp("../../../", appName, &error))
        {
            printf("Error : %s", error.Msg);
        }
#endif


    }
    else
    {
        PrintHelp();
    }



    return 0;
}

