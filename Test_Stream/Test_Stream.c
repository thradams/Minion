


#include "..\Stream.h"
#include "..\test.h"

void Test_Empty()
{
    struct Stream ss = STREAM_INIT;
    assert(ss.Col == 0);
    assert(ss.Line == 0);
    assert(ss.Text == 0);
    assert(ss.Position == 0);

    assert(Stream_GetChar(&ss) == 0);
    assert(Stream_LookAhead(&ss) == 0);
    assert(Stream_Match(&ss) == 0);
    assert(Stream_MatchChar(&ss, 0) == 0);

    Stream_Destroy(&ss);
}

void Test_Empty2()
{
    struct Stream ss = STREAM_INIT;
    Stream_CopyString(&ss, "");
    assert(ss.Position == 0);
    assert(ss.Col == 0);
    assert(ss.Line == 0);

    assert(Stream_GetChar(&ss) == '\0');

    assert(Stream_LookAhead(&ss) == '\0');
    assert(Stream_Match(&ss) == false);


    assert(Stream_MatchChar(&ss, 0) == false);

    Stream_Destroy(&ss);
}

void Test_One()
{
    struct Stream ss = STREAM_INIT;

    assert(ss.Position == 0);
    assert(ss.Col == 0);
    assert(ss.Line == 0);

    Stream_CopyString(&ss, "A");

    assert(ss.Position == 0);
    assert(ss.Col == 1);
    assert(ss.Line == 1);


    assert(Stream_GetChar(&ss) == 'A');
    assert(Stream_LookAhead(&ss) == '\0');


    assert(Stream_Match(&ss) == true);

    assert(ss.Position == 1);
    assert(ss.Col == 2);
    assert(ss.Line == 1);


    assert(Stream_GetChar(&ss) == '\0');
    assert(Stream_LookAhead(&ss) == '\0');

    //assert(Stream_MatchChar(&ss, 0) == 0);

    Stream_Destroy(&ss);
}


void Test_1()
{
    struct Stream ss = STREAM_INIT;
    Stream_CopyString(&ss, "\n");
    assert(Stream_GetChar(&ss) == '\n');
    assert(Stream_LookAhead(&ss) == '\0');

    assert(ss.Position == 0);
    assert(ss.Col == 1);
    assert(ss.Line == 1);


    assert(Stream_Match(&ss) == true);

    assert(ss.Position == 1);
    assert(ss.Col == 1);
    assert(ss.Line == 2);


    assert(Stream_GetChar(&ss) == '\0');
    assert(Stream_LookAhead(&ss) == '\0');

    //assert(Stream_MatchChar(&ss, 0) == 0);

    Stream_Destroy(&ss);
}


void Test_2()
{
    struct Stream ss = STREAM_INIT;
    Stream_CopyString(&ss, "\r\n");
    assert(Stream_GetChar(&ss) == '\r');
    assert(Stream_LookAhead(&ss) == '\n');

    assert(ss.Position == 0);
    assert(ss.Col == 1);
    assert(ss.Line == 1);


    assert(Stream_Match(&ss) == true);

    assert(ss.Position == 1);
    assert(ss.Col == 1);
    assert(ss.Line == 1);

    assert(Stream_Match(&ss) == true);

    assert(ss.Position == 2);
    assert(ss.Col == 1);
    assert(ss.Line == 2);


    assert(Stream_GetChar(&ss) == '\0');
    assert(Stream_LookAhead(&ss) == '\0');

    //assert(Stream_MatchChar(&ss, 0) == 0);

    Stream_Destroy(&ss);
}


int main()
{
    Test_Empty();
    Test_Empty2();
    Test_One();
    Test_1();
    Test_2();
   
}

