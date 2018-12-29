#pragma once
#include <stddef.h>
#include <stdbool.h>

struct Node
{
    const char*  Key;
    const char*  Text;
    struct Node** pChildNodes;
    int Size;
    int Capacity;
    bool SortedFlag;
};


struct Board
{
    struct Node*  pRoot;
};

void Board_Init(struct Board* p);
void Board_Destroy(struct Board* p);
void Board_Add(struct Board * p, const char * key);
void Board_Print(struct Board * p);

void Node_Sort(struct Node* p);