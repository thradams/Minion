
#include "..\JsonScanner.h"
#include "..\test.h"

void Scanner_Test()
{
    struct JsonScanner scanner = JSONSCANNER_INIT;
    //JsonScanner_se
    JsonScanner_CopyString(&scanner, "[\"f\",  true, false, 1, -2, 1.1, null]");

    JsonScanner_Match(&scanner);
    assert(scanner.Token == TK_LEFT_SQUARE_BRACKET);
    assert(strcmp(scanner.Lexeme.c_str, "[") == 0);

    JsonScanner_Match(&scanner);
    assert(scanner.Token == TK_STRING);
    assert(strcmp(scanner.Lexeme.c_str, "\"f\"") == 0);

    JsonScanner_Match(&scanner);
    assert(scanner.Token == TK_COMMA);
    assert(strcmp(scanner.Lexeme.c_str, ",") == 0);

    JsonScanner_Match(&scanner);
    assert(scanner.Token == TK_TRUE);
    assert(strcmp(scanner.Lexeme.c_str, "true") == 0);

    JsonScanner_Match(&scanner);
    assert(scanner.Token == TK_COMMA);
    assert(strcmp(scanner.Lexeme.c_str, ",") == 0);

    JsonScanner_Match(&scanner);
    assert(scanner.Token == TK_FALSE);
    assert(strcmp(scanner.Lexeme.c_str, "false") == 0);

    JsonScanner_Match(&scanner);
    assert(scanner.Token == TK_COMMA);
    assert(strcmp(scanner.Lexeme.c_str, ",") == 0);

    JsonScanner_Match(&scanner);
    assert(scanner.Token == TK_NUMBER);
    assert(strcmp(scanner.Lexeme.c_str, "1") == 0);

    JsonScanner_Match(&scanner);
    assert(scanner.Token == TK_COMMA);
    assert(strcmp(scanner.Lexeme.c_str, ",") == 0);

    JsonScanner_Match(&scanner);
    assert(scanner.Token == TK_NUMBER);
    assert(strcmp(scanner.Lexeme.c_str, "-2") == 0);

    JsonScanner_Match(&scanner);
    assert(scanner.Token == TK_COMMA);
    assert(strcmp(scanner.Lexeme.c_str, ",") == 0);

    JsonScanner_Match(&scanner);
    assert(scanner.Token == TK_NUMBER);
    assert(strcmp(scanner.Lexeme.c_str, "1.1") == 0);

    JsonScanner_Match(&scanner);
    assert(scanner.Token == TK_COMMA);
    assert(strcmp(scanner.Lexeme.c_str, ",") == 0);

    JsonScanner_Match(&scanner);
    assert(scanner.Token == TK_NULL);
    assert(strcmp(scanner.Lexeme.c_str, "null") == 0);

    JsonScanner_Match(&scanner);
    assert(scanner.Token == TK_RIGHT_SQUARE_BRACKET);
    assert(strcmp(scanner.Lexeme.c_str, "]") == 0);

    JsonScanner_Match(&scanner);
    assert(scanner.Token == TK_EOF);
    assert(strcmp(scanner.Lexeme.c_str, "") == 0);

    JsonScanner_Destroy(&scanner);
}

int main()
{
    Scanner_Test();
}
