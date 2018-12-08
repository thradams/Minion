
#include "..\JsonScanner.h"
#include "..\test.h"

void Scanner_Test()
{
    struct JsonScanner scanner = JSONSCANNER_INIT;
    //JsonScanner_se
    JsonScanner_CopyString(&scanner, "[\"f\", true, 1, null]");

    JsonScanner_Match(&scanner);
    assert(scanner.Token == TK_LEFT_SQUARE_BRACKET);
    assert(strcmp(scanner.Lexeme.c_str, "[") == 0);

    JsonScanner_Match(&scanner);
    assert(scanner.Token == TK_STRING);
    assert(strcmp(scanner.Lexeme.c_str, "\"f\"") == 0);

    JsonScanner_Destroy(&scanner);
}

int main()
{
  
}
