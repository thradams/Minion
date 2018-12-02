#pragma once
#include <stddef.h>

struct Board
{
  struct Node*  pRoot;
};

void AddPost(const wchar_t * fmt, ...);
void Board_Paint();