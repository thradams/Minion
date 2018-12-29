
//#include <winsock2.h>
//#include <Windows.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
//#include <wtypes.h>
#include <stdarg.h>
#include <wchar.h>
#include <assert.h>

#include "Board.h"
#include "UITask.h"
#include "conio.h"
#include "BoardCore.h"

struct Board Board = { 0 };

static void GuiPrint(enum TASK_ACTION action, void* p)
{
    char *psz = *((char **)p);
    if (action == TASK_RUN)
    {
        printf(psz);
    }
    else
    {
        //in case of full the call thread
        //can be diferent
        //
        // assert(false);
    }
    free(psz);
}

void Print(const char * fmt, ...)
{
    if (fmt == NULL)
    {
        return;
    }

    va_list args;
    va_start(args, fmt);

    char *psz = (char *)malloc(sizeof(char) * 500);
    psz[0] = 0;
    vsnprintf(psz, 500, fmt, args);
    va_end(args);

    UIActor_Post(GuiPrint, &psz, sizeof(void*));
}

static void GuiAddPost(enum TASK_ACTION action, void* p)
{
    char *psz = *((char **)p);
    if (action == TASK_RUN)
    {
        Board_Add(&Board, psz);
    }
    else
    {
        //in case of full the call thread
        //can be diferent
        //
        // assert(false);
    }
    free(psz);
}

#ifdef UIACTOR
void AddPost(const char * fmt, ...)
{

    if (fmt == NULL)
    {
        return;
    }

    va_list args;
    va_start(args, fmt);

    char *psz = (char *)malloc(sizeof(char) * 500);
    psz[0] = 0;
    vsnprintf(psz, 500, fmt, args);
    va_end(args);

    UIActor_Post(GuiAddPost, &psz, sizeof(void*));
}
#endif


void Node_PrintS(struct Node* p, int* y, int* n)
{
    Node_Sort(p);

    //for (int k = 0; k < *n; k++)
  //    printf(" ");


    if (p->Key)
    {
#ifdef CONSOLE_CONIO
        c_textcolor(CYAN);
        c_gotoxy(1 + *n * 1, *y);
#endif
        //TextOut(hdc, 10 + *n * 20, *y, p->Key, wcslen(p->Key));
        printf("%s", (const char*)p->Key);
  }

    if (p->Text)
    {
#ifdef CONSOLE_CONIO
        c_textcolor(WHITE);
        c_gotoxy(20 + *n * 1, *y);
#endif
        //TextOut(hdc, 200 + *n * 20, *y, p->Text, wcslen(p->Text));
        //TextOut(hdc, 10 + *n * 5, y, p->Key, 0);
        printf(" : %s", p->Text);
    }
    *y += 1;
    //printf("\n");

    (*n)++;
    for (int i = 0; i < p->Size; i++)
    {
        Node_PrintS(p->pChildNodes[i], y, n);
    }
    (*n)--;
}

void Board_Paint()
{
#ifdef CONSOLE_CONIO
    c_setcursortype(_NOCURSOR);
#endif
    if (Board.pRoot)
    {
        int n = 0;
        int y = 1;
        for (int i = 0; i < Board.pRoot->Size; i++)
        {
            Node_PrintS(Board.pRoot->pChildNodes[i], &y, &n);
        }
#ifdef CONSOLE_CONIO
        c_gotoxy(1, y);
#endif
    }
#ifdef CONSOLE_CONIO
    c_setcursortype(_SOLIDCURSOR);
#endif

    }
