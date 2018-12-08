#pragma once

#include "Stream.h"
#include "StrBuilder.h"

enum Tokens
{
  TK_NONE,
  TK_BLANKS,
  TK_LEFT_CURLY_BRACKET,
  TK_RIGHT_CURLY_BRACKET,
  TK_LEFT_SQUARE_BRACKET,
  TK_RIGHT_SQUARE_BRACKET,
  TK_SEMICOLON,
  TK_COMMA,
  TK_COLON,
  TK_STRING,
  TK_NUMBER,
  TK_TRUE,
  TK_FALSE,
  TK_NULL,
  TK_EOF,
};
struct JsonScanner
{
  struct Stream Stream;
  enum Tokens Token;
  struct StrBuilder Lexeme;
};

#define JSONSCANNER_INIT {STREAM_INIT, TK_NONE, STRBUILDER_INIT}
void JsonScanner_Destroy(struct JsonScanner* scanner);
void JsonScanner_GetToken(struct JsonScanner* scanner);
void JsonScanner_Match(struct JsonScanner* scanner);
void JsonScanner_Destroy(struct JsonScanner* scanner);

void JsonScanner_CopyString(struct JsonScanner* stream, const char* sinkString);

