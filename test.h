#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef assert
#undef assert
#endif
#define assert(x) do {if (!(x)) { printf("TEST FAILED\n%d: assert(%s);\n", __LINE__ , #x);  exit(1); } } while(0)

