
#include "TaskQueue.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>



void TaskQueue_Init(struct TaskQueue *pTaskQueue, int queueCapacity)
{
    pTaskQueue->pTasks = NULL;
    pTaskQueue->Capacity = queueCapacity;
    pTaskQueue->Count = 0;
    pTaskQueue->pHead = NULL;
    pTaskQueue->pTail = NULL;
}

int TaskQueue_Push(struct TaskQueue * pAsyncTaskQueue,
  TaskFunction pTaskFunction,
  void* pCapture,
  int captureSizeBytes)
{
  //Retorna 1 se colocou na fila ou 0 se esta cheio.
  int result = 0;

  if (pAsyncTaskQueue->pTasks == NULL && 
      pAsyncTaskQueue->Capacity > 0)
  {    
    pAsyncTaskQueue->pTasks = 
      (struct Task*) malloc(sizeof(struct Task) * pAsyncTaskQueue->Capacity);    
    
    if (pAsyncTaskQueue->pTasks)
    {
      pAsyncTaskQueue->pHead = pAsyncTaskQueue->pTasks;
      pAsyncTaskQueue->pTail = pAsyncTaskQueue->pTasks;
    }
  }

  if (pAsyncTaskQueue->pTasks != NULL)
  {
    if (pAsyncTaskQueue->Count < pAsyncTaskQueue->Capacity)
    {
      pAsyncTaskQueue->pHead->pTaskFunction = pTaskFunction;

      //copia
      if (pCapture != NULL)
      {
        assert(captureSizeBytes <= sizeof(pAsyncTaskQueue->pHead->pCapture));
        memcpy(pAsyncTaskQueue->pHead->pCapture, pCapture, captureSizeBytes);
      }

      pAsyncTaskQueue->pHead++;

      if (pAsyncTaskQueue->pHead == (pAsyncTaskQueue->pTasks + pAsyncTaskQueue->Capacity))
      {
        pAsyncTaskQueue->pHead = pAsyncTaskQueue->pTasks;
      }
      pAsyncTaskQueue->Count++;
      result = 1;
    }
    else
    {
      pTaskFunction(TASK_FULL, pCapture);
    }
  }
  else
  {
    pTaskFunction(TASK_FULL, pCapture);
  }


  return result;
}

struct Task* TaskQueue_Pop(struct TaskQueue *pTaskQueue)
{
  struct Task* pTask = NULL;
  if (pTaskQueue->Count > 0)
  {
    pTask = pTaskQueue->pTail;
    pTaskQueue->pTail++;
    if (pTaskQueue->pTail == (pTaskQueue->pTasks + pTaskQueue->Capacity))
    {
      pTaskQueue->pTail = pTaskQueue->pTasks;
    }
    pTaskQueue->Count--;
  }
  return pTask;
}


void TaskQueue_Destroy(struct TaskQueue *pTaskQueue)
{
  if (pTaskQueue->Count > 0)
  {
    struct Task* pTask = TaskQueue_Pop(pTaskQueue);
    while (pTask)
    {
      pTask->pTaskFunction(TASK_CANCELED, pTask->pCapture);
#ifdef _DEBUG
     
#endif
      pTask = TaskQueue_Pop(pTaskQueue);
    }
  }
  free(pTaskQueue->pTasks);
}

void TaskQueue_Swap(struct TaskQueue *pA, struct TaskQueue *pB)
{
  struct TaskQueue temp = *pA;
  *pA = *pB;
  *pB = temp;
}



#ifdef TEST
#include <stdio.h>
int main()
{
  printf("test");
}
#endif
