
#include "Stream.h"

#include <stdlib.h>
#include <string.h>

void Stream_Destroy(struct Stream* stream)
{
  free((void*)stream->Text);
}

wchar_t Stream_GetChar(struct Stream* stream)
{
  wchar_t ch = 0;
  if (stream->Text != NULL)
  {
    ch = stream->Text[stream->Position];
  }
  return ch;
}

wchar_t Stream_LookAhead(struct Stream* stream)
{
  wchar_t ch = 0;
  if (stream->Text != NULL &&
    stream->Text[stream->Position] != 0)
  {
    ch = stream->Text[stream->Position + 1];
  }

  return ch;
}


bool Stream_MatchChar(struct Stream* stream, wchar_t ch)
{
  bool b = false;
  if (stream->Text != NULL)
  {
    b = Stream_GetChar(stream) == ch;
    if (b)
    {
      b = Stream_Match(stream);
    }
  }
  return b;
}

bool Stream_Match(struct Stream* stream)
{
  bool b = false;
  if (stream->Text != NULL &&
      stream->Text[stream->Position] != '\0')
  {        
    if (stream->Text[stream->Position] == '\n')
    {
      stream->Line++;
      stream->Col = 1;
    }    
    else if (stream->Text[stream->Position] == '\r')
    {
      //don't move Col
    }
    else
    {
      stream->Col++;
    }
    stream->Position++;
    b = true;
  }
  return b;
}

void Stream_BuyString(struct Stream* stream, const char* sinkString)
{
  Stream_Destroy(stream);
  
  if (sinkString != NULL &&
      sinkString[0] != '\0')
  {
    stream->Text = sinkString;
    stream->Line = 1;
    stream->Col = 1;
    stream->Position = 0;
  }
  else
  {
    stream->Text = NULL;
    stream->Line = 0;
    stream->Col = 0;
    stream->Position = 0;
  }
}


void Stream_CopyString(struct Stream* stream, const char* s)
{
  char* copy = _strdup(s);
  Stream_BuyString(stream, copy);
}

