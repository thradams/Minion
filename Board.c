
#include <winsock2.h>
#include <Windows.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <wtypes.h>
#include <stdarg.h>
#include <wchar.h>


#include "Board.h"
#include "UITask.h"
#include "conio.h"

struct Node
{
  const wchar_t*  Key;
  const wchar_t*  Text;
  struct Node** pChildNodes;
  int Size;
  int Capacity;
  bool SortedFlag;
};

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
  const wchar_t* searchItem,
  int searchItemLen)
{
  int mid;
  int c = 0;
  int l = 0;
  int u = pNode->Size - 1;

  while (l <= u)
  {
    mid = (l + u) / 2;

    int cmp = wcsncmp(searchItem, pNode->pChildNodes[mid]->Key, searchItemLen);

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
      while (wcscmp(pNode->pChildNodes[i]->Key, pNode->pChildNodes[pivot]->Key) <= 0 &&
        i < last_index)
      {
        i++;
      }

      while (wcscmp(pNode->pChildNodes[j]->Key, pNode->pChildNodes[pivot]->Key) > 0)
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
    QuickSort(p, 0, p->Size - 1);
    p->SortedFlag = true;
  }
}

int Node_Find(struct Node* p, const wchar_t* key, int keylen)
{
  if (p == NULL)
    return -1;

  Node_Sort(p);
  return  BinarySearch(p, key, keylen);
}

void Node_RemoveKey(struct Node* p, const wchar_t* key)
{
  if (key != NULL)
  {
    int idx = Node_Find(p, key, (int)wcslen(key));
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

void Node_Add(struct Node* p, const wchar_t* key, const wchar_t* text)
{
  struct Node* pNewNode = Node_Create();
  pNewNode->Key = _wcsdup(key);
  pNewNode->Text = _wcsdup(text);
  Node_AddNode(p, pNewNode);
}

void Board_Remove(struct Board * p, const wchar_t * key)
{
  if (key == NULL)
  {
    return;
  }

  struct Node* pCurrent = p->pRoot;
  struct Node* pParent = NULL;
  int childIndex = -1;
  const wchar_t *tail = key;
  const wchar_t *front = key;
  while (*tail)
  {
    if (*tail == '/' || *(tail + 1) == 0)
    {
      const wchar_t* key2 = front;
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

static wchar_t * wstrndup(const wchar_t *s, size_t n)
{
  size_t len = n > 0 ? wcsnlen(s, n) : 0;
  wchar_t *newt = (wchar_t *)malloc(sizeof(wchar_t) * len + 1);

  if (newt == NULL)
    return NULL;

  newt[len] = '\0';
  return (wchar_t *)memcpy(newt, s, sizeof(wchar_t) * len);
}

void Board_Add(struct Board * p, const wchar_t * key)
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
    pNew->Key = _wcsdup(L"root");
    p->pRoot = pNew;
  }

  struct Node* pCurrent = p->pRoot;
  const wchar_t *tail = key;
  const wchar_t *front = key;
  while (*tail)
  {
    if (*tail == '/' || *tail == ' ' || *(tail + 1) == 0)
    {
      const wchar_t* key2 = front;
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
        pNew->Key = wstrndup(key2, key2size);

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

        pCurrent->Text = _wcsdup(tail + 1);

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

struct Board Board = { 0 };

static void GuiAddPost(enum TASK_ACTION action, void* p)
{
  wchar_t *psz = *((wchar_t **)p);
  Board_Add(&Board, psz);
  free(psz);
}

void AddPost(const wchar_t * fmt, ...)
{
  

  if (fmt == NULL)
  {
    return;
  }

  //if (s_MobileHttpServer.m_spWhiteboard)
  //{

    va_list args;
    va_start(args, fmt);

    wchar_t *psz = (wchar_t *)malloc(sizeof(wchar_t) * 500);
    //wchar_t buffer[500];
    _vsnwprintf_s(psz, 500, _TRUNCATE, fmt, args);
    va_end(args);

    UIActor_Post(GuiAddPost, &psz, sizeof(void*));
    //std::wstring sKey = buffer;

    //s_MobileHttpServer.m_spWhiteboard->Dispatch([sKey](WhiteBoard& board)
    //{
      //WhiteBoard_Add(&board, sKey.c_str());
    //});
  //}
}



void Node_PrintS(struct Node* p, int* y, int* n)
{
  Node_Sort(p);

  //for (int k = 0; k < *n; k++)
//    printf(" ");

  
  if (p->Key)
  {
    textcolor(CYAN);
    gotoxy(1 + *n * 1, *y);
    //TextOut(hdc, 10 + *n * 20, *y, p->Key, wcslen(p->Key));
    printf("%ls", (const wchar_t*)p->Key);
  }

  if (p->Text)
  {
    textcolor(WHITE);
    gotoxy(20 + *n * 1, *y);
    //TextOut(hdc, 200 + *n * 20, *y, p->Text, wcslen(p->Text));
    //TextOut(hdc, 10 + *n * 5, y, p->Key, 0);
    printf(" : %ls", p->Text);
  }
  *y += 1;
  //printf("\n");

  (*n)++;
  for (int i = 0; i < p->Size; i++)
  {
    Node_PrintS( p->pChildNodes[i], y, n);
  }
  (*n)--;
}

void Board_Paint()
{
  _setcursortype(_NOCURSOR);

  if (Board.pRoot)
  {
    int n = 0;
    int y = 1;
    for (int i = 0; i < Board.pRoot->Size; i++)
    {
      Node_PrintS(Board.pRoot->pChildNodes[i], &y, &n);
    }
    gotoxy(1, y);
  }
  _setcursortype(_SOLIDCURSOR);

}
