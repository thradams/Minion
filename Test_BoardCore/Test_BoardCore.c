#include "..\test.h"
#include "..\BoardCore.h"

int main()
{
    struct Board board;
    Board_Init(&board);

    Board_Destroy(&board);
}
