#pragma once

#include <stddef.h>

enum TASK_ACTION
{
  TASK_CANCELED = 1,
  TASK_FULL = 2,
  TASK_RUN = 3
};

typedef void(*TaskFunction)(enum TASK_ACTION action, void* pData);

struct Task
{
  TaskFunction pTaskFunction;
  void* pCapture[4];
};


struct TaskQueue
{
  struct Task* pTasks;
  int       Capacity;
  int       Count;
  struct Task* pHead;
  struct Task* pTail;
};


void TaskQueue_Init(struct TaskQueue *pTaskQueue, int capacity);

int TaskQueue_Push(struct TaskQueue * pAsyncTaskQueue,
  TaskFunction pTaskFunction,
  void* pCapture,
  int captureSizeBytes);

struct Task* TaskQueue_Pop(struct TaskQueue *pTaskQueue);

void TaskQueue_Destroy(struct TaskQueue *pTaskQueue);
void TaskQueue_Swap(struct TaskQueue *pA, struct TaskQueue *pB);
