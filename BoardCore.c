#include <stdlib.h>
#include <string.h>
#include "BoardCore.h"
#include <stdio.h>
#include <wchar.h>

#ifdef _WIN32
#define strdup _strdup 
#else
#define _strdup strdup
#endif

struct Node* Node_Create()
{
    struct Node *p = (struct Node*)malloc(sizeof * p);
    if (p != NULL)
    {
        p->Key = NULL;
        p->Text = NULL;
        p->pChildNodes = NULL;
        p->Size = 0;
        p->Capacity = 0;
        p->SortedFlag = false;
    }
    return p;
}

void Node_Delete(struct Node* p)
{
    if (p != NULL)
    {
        free((void*)p->Key);
        free((void*)p->Text);
        free(p);
    }
}

//Retorna o indice do array aonde se encontra a Key searchItem
int BinarySearch(struct Node *pNode,
    const char* searchItem,
    int searchItemLen)
{
    int mid;
    int c = 0;
    int l = 0;
    int u = pNode->Size - 1;

    while (l <= u)
    {
        mid = (l + u) / 2;

        int cmp = strncmp(searchItem, pNode->pChildNodes[mid]->Key, searchItemLen);

        if (cmp == 0)
        {
            c = 1;
            break;
        }
        else if (cmp < 0)
        {
            u = mid - 1;
        }
        else
        {
            l = mid + 1;
        }
    }

    return c == 0 ? -1 : mid;
}

//Ordena por Key
void QuickSort(struct Node *pNode,
    int first_index,
    int last_index)
{
    int pivot, j, i;
    struct Node* tempNode;

    if (first_index < last_index)
    {
        pivot = first_index;
        i = first_index;
        j = last_index;

        while (i < j)
        {

            while (i < last_index &&
                   strcmp(pNode->pChildNodes[i]->Key, pNode->pChildNodes[pivot]->Key) <= 0)
            {
                i++;
            }

            while (strcmp(pNode->pChildNodes[j]->Key, pNode->pChildNodes[pivot]->Key) > 0)
            {
                j--;
            }

            if (i < j)
            {
                /*swap*/
                tempNode = pNode->pChildNodes[i];
                pNode->pChildNodes[i] = pNode->pChildNodes[j];
                pNode->pChildNodes[j] = tempNode;
            }
        }

        /*swap*/
        tempNode = pNode->pChildNodes[pivot];
        pNode->pChildNodes[pivot] = pNode->pChildNodes[j];
        pNode->pChildNodes[j] = tempNode;

        QuickSort(pNode, first_index, j - 1);
        QuickSort(pNode, j + 1, last_index);
    }
}

void Node_Reserve(struct Node* p, int nelements)
{
    if (nelements > p->Capacity)
    {
        struct Node** pnew = p->pChildNodes;
        pnew = (struct Node**)realloc(pnew, nelements * sizeof(struct Node*));
        if (pnew)
        {
            p->pChildNodes = pnew;
            p->Capacity = nelements;
        }
    }
}

void Node_Grow(struct Node* p)
{
    int minCapacity = p->Size + 1;

    if (minCapacity > p->Capacity)
    {
        int newCapacity = p->Capacity + p->Capacity / 2;
        if (newCapacity < minCapacity)
        {
            newCapacity = minCapacity;
        }
        Node_Reserve(p, newCapacity);
    }
}

void Node_RemoveAt(struct Node* p, int index)
{
    if (index == p->Size - 1)
    {
        //[0][1][2][3][4][5] size = 6
        //                ^        
    }
    else
    {
        //[0][1][2][3][4][5] size = 6
        //    ^                    
        int nItemsMoved = p->Size - index - 1;
        memmove(&p->pChildNodes[index],
            &p->pChildNodes[index + 1],
            nItemsMoved * sizeof(struct Node*));
    }
    p->Size--;
}

void Node_Sort(struct Node* p)
{
    if (!p->SortedFlag)
    {
        if (p->Size > 1)
        {
            QuickSort(p, 0, p->Size - 1);
        }
        p->SortedFlag = true;
    }
}

int Node_Find(struct Node* p, const char* key, int keylen)
{
    if (p == NULL)
        return -1;

    Node_Sort(p);
    return  BinarySearch(p, key, keylen);
}

void Node_RemoveKey(struct Node* p, const char* key)
{
    if (key != NULL)
    {
        int idx = Node_Find(p, key, (int)strlen(key));
        if (idx != -1)
        {
            Node_RemoveAt(p, idx);
        }
    }
}

void Node_AddNode(struct Node* p, struct  Node *pNewNode)
{
    //garante que tem espaço para + 1
    Node_Grow(p);
    p->pChildNodes[p->Size] = pNewNode;
    p->Size++;
    p->SortedFlag = (p->Size == 1);
}

void Node_Add(struct Node* p, const char* key, const char* text)
{
    struct Node* pNewNode = Node_Create();
#ifdef _WIN32
    pNewNode->Key = _strdup(key);
    pNewNode->Text = _strdup(text);
#else
    pNewNode->Key = strdup(key);
    pNewNode->Text = strdup(text);
#endif
    Node_AddNode(p, pNewNode);
}

void Board_Remove(struct Board * p, const char * key)
{
    if (key == NULL)
    {
        return;
    }

    struct Node* pCurrent = p->pRoot;
    struct Node* pParent = NULL;
    int childIndex = -1;
    const char *tail = key;
    const char *front = key;
    while (*tail)
    {
        if (*tail == '/' || *(tail + 1) == 0)
        {
            const char* key2 = front;
            int key2size;
            if (*(tail + 1) == 0)
            {
                key2size = (tail - front) + 1;
            }
            else
            {
                key2size = tail - front;
            }
            int idx = Node_Find(pCurrent, key2, key2size);
            if (idx == -1)
            {
                //nao achou
                pParent = NULL;
                childIndex = -1;
                break;
            }
            else
            {
                //ja existe so aponta para ele
                pParent = pCurrent;
                pCurrent = pCurrent->pChildNodes[idx];
                childIndex = idx;
            }
            front = tail + 1;
        }
        tail++;
    }

    if (pParent != NULL && childIndex != -1)
    {
        Node_RemoveAt(pParent, childIndex);
    }
}
#ifdef _WIN32
static char * strndup(const char *s, size_t n)
{
    size_t len = n > 0 ? strnlen(s, n) : 0;
    char *newt = (char *)malloc(sizeof(char) * len + 1);

    if (newt == NULL)
        return NULL;

    newt[len] = '\0';
    return (char *)memcpy(newt, s, sizeof(char) * len);
}
#endif

void Board_Add(struct Board * p, const char * key)
{
    //  01234567
    //  AAAAAAAA/BBB/CCCC
    //  ^       ^
    //  |       |
    // front    |       
    //         tail

    if (p->pRoot == NULL)
    {
        struct Node* pNew = Node_Create();
        pNew->Key = strdup("root");
        p->pRoot = pNew;
    }

    struct Node* pCurrent = p->pRoot;
    const char *tail = key;
    const char *front = key;
    while (*tail)
    {
        if (*tail == '/' || *tail == ' ' || *(tail + 1) == 0)
        {
            const char* key2 = front;
            int key2size;
            if (*(tail + 1) == 0)
            {
                key2size = (tail - front) + 1;
            }
            else
            {
                key2size = tail - front;
            }

            int idx = Node_Find(pCurrent, key2, key2size);
            if (idx == -1)
            {
                //o que ele nao awchar_t vai criando..
                struct Node* pNew = Node_Create();
                pNew->Key = strndup(key2, (size_t)key2size);

                Node_AddNode(pCurrent, pNew);
                pCurrent = pNew;
            }
            else
            {
                //ja existe so aponta para ele
                pCurrent = pCurrent->pChildNodes[idx];
            }

            if (*tail == ' ')
            {
                free((void*)pCurrent->Text);

                pCurrent->Text = strdup(tail + 1);

                break;
            }

            front = tail + 1;
        }

        if (*tail == '\0')
        {
            break;
        }
        tail++;
    }
}


void Board_Init(struct Board* p)
{
    p->pRoot = NULL;
}

void Board_Destroy(struct Board* p)
{
    //TODO
    //Node_Delete(p->pRoot);
}


static void Node_PrintS(struct Node* p, int* y, int* n)
{
    Node_Sort(p);

    for (int k = 0; k < *n; k++)
      printf(" ");


    if (p->Key)
    {
        printf("%s", (const char*)p->Key);
    }

    if (p->Text)
    {
        printf(" : %s\n", p->Text);
    }
    
    printf("\n");

    (*n)++;
    for (int i = 0; i < p->Size; i++)
    {
        Node_PrintS(p->pChildNodes[i], y, n);
    }
    (*n)--;
}

void Board_Print(struct Board* board)
{
    if (board->pRoot)
    {
        int n = 0;
        int y = 1;
        for (int i = 0; i < board->pRoot->Size; i++)
        {
            Node_PrintS(board->pRoot->pChildNodes[i], &y, &n);
        }
    }

}

