#pragma once
#include <stddef.h>


void AddPost(const char * fmt, ...);

//thread safe printf
void Print(const char * fmt, ...);

void Board_Paint();