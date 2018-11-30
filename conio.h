
#ifndef _CONIO_H_
#define _CONIO_H_

#ifdef _WIN32
#include <conio.h> //from window
#endif

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

#define cprintf printf

enum COLORS
{
  BLACK = 0,
  BLUE = 1,
  GREEN = 2,
  CYAN = 3,
  RED = 4,
  MAGENTA = 5,
  BROWN = 6,
  LIGHTGRAY = 7,
  DARKGRAY = 8,
  LIGHTBLUE = 9,
  LIGHTGREEN = 10,
  LIGHTCYAN = 11,
  LIGHTRED = 12,
  LIGHTMAGENTA = 13,
  YELLOW = 14,
  WHITE = 15,
  BLINK = 128
};

enum CURSORTYPE
{
  _NOCURSOR,//     (turns off the cursor
  _SOLIDCURSOR,//  (solid block cursor
  _NORMALCURSOR // (normal underscore cursor
};


#ifndef _WIN32
char * cgets(char * _Buffer);
int cputs(const char *str);
int getch(void);
int getche(void);
/*
Checks for currently available keystrokes.
kbhit checks to see if a keystroke is currently available.
Any available keystrokes can be retrieved with getch or getche.
*/
int kbhit(void);
int putch(int c);
int ungetch(int ch);
#else
/*
Gets character from keyboard, does not echo to screen.
getch reads a single character directly from the keyboard, without echoing to the screen.
*/
#define getch _getch

#define putch _putch
#define kbhit _kbhit
#define cputs _cputs
#define ungetch _ungetch
#define getche _getche
#define putch _putch
#define cputs _cputs
#endif

/*
 Clears the text-mode window.
 clrscr clears the current text window and places the cursor in 
 the upper left corner (at position 1,1).
*/
void clrscr();

/*
 Positions cursor in text window.
 gotoxy moves the cursor to the given position in the current text window. 
 If the coordinates are in any way invalid the call to gotoxy is ignored.
 An example of this is a call to gotoxy(40,30) when (35,25) is the bottom right position 
 in the window. Neither argument to gotoxy can be zero.
*/
void gotoxy(int x, int y);

/*Selects cursor appearance.

Sets the cursor type to

_NOCURSOR
 Turns off the cursor
_NORMALCURSOR
  Normal underscore cursor
_SOLIDCURSOR
  Solid block cursor
*/
void _setcursortype(int cur_t);

/*
Selects new text background color.

BLACK 0
BLUE 1
GREEN 2
CYAN 3
RED 4
MAGENTA 5
BROWN 6
LIGHTGRAY 7
*/
void textbackground(int newcolor);

/*
Selects new character color in text mode.

BLACK 0
BLUE 1
GREEN 2
CYAN 3
RED 4
MAGENTA 5
BROWN 6
LIGHTGRAY 7
DARKGRAY 8
LIGHTBLUE 9
LIGHTGREEN 10
LIGHTCYAN 11
LIGHTRED 12
LIGHTMAGENTA 13
YELLOW 14
WHITE 15
BLINK 128
*/
void textcolor(int newcolor);

#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif //CONIO_H

