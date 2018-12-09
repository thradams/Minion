#pragma once
#include <stddef.h>
#include <stdbool.h>

struct Node
{
    const wchar_t*  Key;
    const wchar_t*  Text;
    struct Node** pChildNodes;
    int Size;
    int Capacity;
    bool SortedFlag;
};

void Node_Sort(struct Node* p);

struct Board
{
    struct Node*  pRoot;
};

void Board_Init(struct Board* p);
void Board_Destroy(struct Board* p);
void Board_Add(struct Board * p, const wchar_t * key);
void Board_Print(struct Board * p);