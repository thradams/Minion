#pragma once
#include <stddef.h>
#include <WinSock2.h>
#include <Windows.h>

struct Board
{
  struct Node*  pRoot;
};

void AddPost(const wchar_t * fmt, ...);
void Board_Paint();