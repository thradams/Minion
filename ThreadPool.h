#pragma once
#include <stdbool.h>
#include "tinycthread.h"
#include "TaskQueue.h"

#define POOL_SIZE 24

struct ThreadPool
{
  struct TaskQueue TaskQueue;
  mtx_t        Mutex;
  thrd_t       Threads[POOL_SIZE];
  bool         Stopping;
  cnd_t        Condition;
};

int ThreadPool_Init(struct ThreadPool* pThreadPool, size_t tasksCapacity);

void ThreadPool_Destroy(struct ThreadPool* pThreadPool);

int ThreadPool_Push(struct ThreadPool * pAsyncTaskQueue,
  TaskFunction pTaskFunction,
  void* pCapture,
  size_t captureSizeBytes);

void ThreadPool_Stop(struct ThreadPool* pThreadPool);
void ThreadPool_Join(struct ThreadPool* pThreadPool);