#include "Error.h"

bool Error_IsEmpty(struct Error* error)
{
    return error->Msg[0] == '\0';
}

void Error_SetVa(struct Error * p, const char* fmt, va_list va)
{
  vsnprintf(p->Msg, sizeof(p->Msg), fmt, va);
}

void Error_Set(struct Error * p, const char* fmt, ...)
{
  va_list args;
  va_start(args, fmt);
  vsnprintf(p->Msg, sizeof(p->Msg), fmt, args);
  va_end(args);
}
