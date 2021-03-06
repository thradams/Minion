#pragma once

#include <stdio.h>  
#include <stdarg.h> 
#include <stdbool.h>

struct Error
{
  char Msg[300];
};
#define ERROR_INIT {0}

bool Error_IsEmpty(struct Error* error);

void Error_SetVa(struct Error * p, const char* fmt, va_list va);

void Error_Set(struct Error * p, const char* fmt, ...);
