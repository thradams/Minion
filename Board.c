
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

static void GuiAddPost(enum TASK_ACTION action, void* p)
{
    wchar_t *psz = *((wchar_t **)p);
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
void AddPost(const wchar_t * fmt, ...)
{
  

  if (fmt == NULL)
  {
    return;
  }

  //if (s_MobileHttpServer.m_spWhiteboard)
  //{

    va_list args;
    va_start(args, fmt);

    wchar_t *psz = (wchar_t *)malloc(sizeof(wchar_t) * 500);
    //wchar_t buffer[500];
    _vsnwprintf_s(psz, 500, _TRUNCATE, fmt, args);
    va_end(args);

    UIActor_Post(GuiAddPost, &psz, sizeof(void*));

    //std::wstring sKey = buffer;

    //s_MobileHttpServer.m_spWhiteboard->Dispatch([sKey](WhiteBoard& board)
    //{
      //WhiteBoard_Add(&board, sKey.c_str());
    //});
  //}
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
    printf("%ls", (const wchar_t*)p->Key);
  }

  if (p->Text)
  {
#ifdef CONSOLE_CONIO
      c_textcolor(WHITE);
      c_gotoxy(20 + *n * 1, *y);
#endif
    //TextOut(hdc, 200 + *n * 20, *y, p->Text, wcslen(p->Text));
    //TextOut(hdc, 10 + *n * 5, y, p->Key, 0);
    printf(" : %ls", p->Text);
  }
  *y += 1;
  //printf("\n");

  (*n)++;
  for (int i = 0; i < p->Size; i++)
  {
    Node_PrintS( p->pChildNodes[i], y, n);
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
