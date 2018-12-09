#include "..\test.h"
#include "..\BoardCore.h"


void Test1()
{
    struct Board Board = { 0 };
    Board_Print(&Board);
    Board_Destroy(&Board);
}

void Test2()
{
    struct Board Board = { 0 };
    Board_Add(&Board, L"teste");
    Board_Print(&Board);
    Board_Destroy(&Board);
}

void Test3()
{
    struct Board Board = { 0 };
    Board_Add(&Board, L"teste1");
    Board_Add(&Board, L"teste1/teste2");
    Board_Print(&Board);
    Board_Destroy(&Board);
}

int main()
{
    Test1();
    Test2();
    Test3();

}
