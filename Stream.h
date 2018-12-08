#pragma once
#include  <wctype.h> 
#include <stdbool.h>

struct Stream
{
  /*
  Text, Stream is the owner
  */
  const char* Text;
  
  /*Current linear position*/
  int Position;
    
  int Line;    
  int Col;
};

#define STREAM_INIT {0, 0, 0, 0}


void Stream_Destroy(struct Stream* stream);
wchar_t Stream_GetChar(struct Stream* stream);
wchar_t Stream_LookAhead(struct Stream* stream);
bool Stream_MatchChar(struct Stream* stream, wchar_t ch);
bool Stream_Match(struct Stream* stream);
void Stream_BuyString(struct Stream* stream, const char* sinkString);
void Stream_CopyString(struct Stream* stream, const char* sinkString);
