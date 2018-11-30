


#include "..\Error.h"
#include "..\test.h"

int main()
{
  struct Error error = ERROR_INIT;
  assert(Error_IsEmpty(&error));
  Error_Set(&error, "Teste %d", 1);
  assert(!Error_IsEmpty(&error));
  assert(strcmp(error.Msg, "Teste 1") == 0);

  const char* bigtext = "abcdefghijlkmnopqrstuvxzabcdefghijlkmnopqrstuvxzabcdefghijlkmnopqrstuvxzabcdefghijlkmnopqrstuvxzabcdefghijlkmnopqrstuvxzabcdefghijlkmnopqrstuvxzabcdefghijlkmnopqrstuvxzabcdefghijlkmnopqrstuvxzabcdefghijlkmnopqrstuvxzabcdefghijlkmnopqrstuvxzabcdefghijlkmnopqrstuvxzabcdefghijlkmnopqrstuvxzabcdefghijlkmnopqrstuvxzabcdefghijlkmnopqrstuvxzabcdefghijlkmnopqrstuvxzabcdefghijlkmnopqrstuvxzabcdefghijlkmnopqrstuvxzabcdefghijlkmnopqrstuvxzabcdefghijlkmnopqrstuvxzabcdefghijlkmnopqrstuvxzabcdefghijlkmnopqrstuvxzabcdefghijlkmnopqrstuvxzabcdefghijlkmnopqrstuvxzabcdefghijlkmnopqrstuvxzabcdefghijlkmnopqrstuvxz";

  Error_Set(&error, bigtext);

}
