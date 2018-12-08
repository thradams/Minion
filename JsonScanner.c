
#include "JsonScanner.h"
#include <assert.h>
void JsonScanner_CopyString(struct JsonScanner* scanner, const char* sinkString)
{
  Stream_CopyString(&scanner->Stream, sinkString);
  scanner->Token = TK_NONE;
  StrBuilder_Clear(&scanner->Lexeme);
}

void JsonScanner_Destroy(struct JsonScanner* scanner)
{
  Stream_Destroy(&scanner->Stream);
}

void JsonScanner_GetToken(struct JsonScanner* scanner)
{
}

wchar_t JsonScanner_MatchChar(struct JsonScanner* scanner)
{
  wchar_t ch = Stream_GetChar(&scanner->Stream);
  StrBuilder_AppendWChar(&scanner->Lexeme, ch);
  Stream_Match(&scanner->Stream);
  ch = Stream_GetChar(&scanner->Stream);
  return ch;
}

wchar_t JsonScanner_MatchCharWith(struct JsonScanner* scanner, wchar_t wch)
{
  wchar_t ch = Stream_GetChar(&scanner->Stream);
  if (ch == wch)
  {
    StrBuilder_AppendWChar(&scanner->Lexeme, ch);
    Stream_Match(&scanner->Stream);
    ch = Stream_GetChar(&scanner->Stream);    
  }
  else
  {
    //unexpected
  }
  return ch;
}

void JsonScanner_Match(struct JsonScanner* scanner)
{
  StrBuilder_Clear(&scanner->Lexeme);
  scanner->Token = TK_NONE;

  wchar_t ch = Stream_GetChar(&scanner->Stream);

  if (ch == '{')
  {
    scanner->Token = TK_LEFT_CURLY_BRACKET;
    JsonScanner_MatchChar(scanner);    
  }
  else if (ch == '}')
  {
    scanner->Token = TK_RIGHT_CURLY_BRACKET;
    JsonScanner_MatchChar(scanner);    
  }
  else if (ch == '[')
  {
    scanner->Token = TK_LEFT_SQUARE_BRACKET;
    JsonScanner_MatchChar(scanner);    
  }
  else if (ch == ']')
  {
    scanner->Token = TK_RIGHT_SQUARE_BRACKET;
    JsonScanner_MatchChar(scanner);    
  }
  else if (ch == 'f')//false
  {
    scanner->Token = TK_FALSE;
    JsonScanner_MatchCharWith(scanner, L'f');
    JsonScanner_MatchCharWith(scanner, L'a');
    JsonScanner_MatchCharWith(scanner, L'l');
    JsonScanner_MatchCharWith(scanner, L's');
    JsonScanner_MatchCharWith(scanner, L'e');
  }
  else if (ch == 't')//true
  {
    scanner->Token = TK_TRUE;
    JsonScanner_MatchCharWith(scanner, L't');
    JsonScanner_MatchCharWith(scanner, L'r');
    JsonScanner_MatchCharWith(scanner, L'u');
    JsonScanner_MatchCharWith(scanner, L'e');    
  }
  else if (ch == 'n')//null
  {
    scanner->Token = TK_NULL;
    JsonScanner_MatchCharWith(scanner, L'n');
    JsonScanner_MatchCharWith(scanner, L'u');
    JsonScanner_MatchCharWith(scanner, L'l');
    JsonScanner_MatchCharWith(scanner, L'l');
  }
  else if (ch == '"')
  {
    scanner->Token = TK_STRING;
    ch = JsonScanner_MatchChar(scanner);
    for (;;)
    {
      if (ch == L'\\')
      {
        //scape
        ch = JsonScanner_MatchChar(scanner);

        switch (ch)
        {
        case '"':
        case '/':
        case '\\':
        case 'b':
        case 'f':
        case 'n':
        case 'r':
        case 't':
          ch = JsonScanner_MatchChar(scanner);
          break;
        default:
          break;
        }
      }
      else if (ch == '"')
      {
        ch = JsonScanner_MatchChar(scanner);
        break;
      }
      else
      {
        //qualquer coisa  
        ch = JsonScanner_MatchChar(scanner);
      }
    }
  }
  else if (ch == ' ' || ch == '\n' || ch == '\r' || ch == '\t')
  {
    while (ch == ' ' || ch == '\n' || ch == '\r' || ch == '\t')
    {
      ch = JsonScanner_MatchChar(scanner);
    }
    scanner->Token = TK_BLANKS;
  }
  else if (ch == ':')
  {
    ch = JsonScanner_MatchChar(scanner);
    scanner->Token = TK_COLON;
  }
  else if (ch == ',')
  {
    scanner->Token = TK_COMMA;
    ch = JsonScanner_MatchChar(scanner);    
  }
  else if (ch == ';')
  {
    scanner->Token = TK_SEMICOLON;
    ch = JsonScanner_MatchChar(scanner);    
  }
  else if (ch == '-' || (ch >= '0' && ch <= '9'))
  {
    scanner->Token = TK_NUMBER;

    ch = JsonScanner_MatchChar(scanner);


    while (ch >= L'0' && ch <= L'9')
    {
      ch = JsonScanner_MatchChar(scanner);
    }

    if (ch == L'.')
    {
      ch = JsonScanner_MatchChar(scanner);

      while (ch >= L'0' && ch <= L'9')
      {
        ch = JsonScanner_MatchChar(scanner);
      }
    }

    if (ch == L'E' || ch == L'e')
    {
      ch = JsonScanner_MatchChar(scanner);

      if (ch == L'+' || ch == L'-')
      {
        ch = JsonScanner_MatchChar(scanner);
      }

      while (ch >= L'0' && ch <= L'9')
      {
        ch = JsonScanner_MatchChar(scanner);
      }
    }

    
  }
}



