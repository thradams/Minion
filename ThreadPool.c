
#include "ThreadPool.h"

#ifdef BOARD
#include "Board.h"
#endif

struct ThreadPool MainThreadPool;

int Thread_N_Loop(void* pData)
{
  struct ThreadPool *pTaskQueue = (struct ThreadPool *)pData;
  
  for (;;)
  {
#ifdef BOARD
    AddPost(L"Thread/%d Waiting", (int)GetCurrentThreadId());
#endif
    mtx_lock(&pTaskQueue->Mutex);

    while (!pTaskQueue->Stopping && pTaskQueue->TaskQueue.Count == 0)
    {
      /*
      Atomically unlocks the mutex pointed to by mutex and blocks on
      the condition variable pointed to by cond until the thread is signalled
      by cnd_signal or cnd_broadcast. The mutex is locked again before the
      function returns.
      The behavior is undefined if the mutex is not already locked by the
      calling thread.
      */
      cnd_wait(&pTaskQueue->Condition, &pTaskQueue->Mutex);
    }

    

    if (pTaskQueue->Stopping)
    {
      mtx_unlock(&pTaskQueue->Mutex);
      break;
    }
    else
    {
      struct Task *p = TaskQueue_Pop(&pTaskQueue->TaskQueue);
      struct Task copy;
      
      if (p)
      {
        copy = *p;
      }
#ifdef BOARD
      AddPost(L"Thread %d/%d", pTaskQueue->TaskQueue.Count, pTaskQueue->TaskQueue.Capacity);
#endif
      mtx_unlock(&pTaskQueue->Mutex);
      if (p)
      {
#ifdef BOARD
        AddPost(L"Thread/%d Working", (int)GetCurrentThreadId());
#endif
        (copy.pTaskFunction)(TASK_RUN, copy.pCapture);
      }
    }
  }

  return 0;
}

int ThreadPool_Init(struct ThreadPool* pThreadPool, size_t tasksCapacity)
{
  //NULL is the MainThreadPool
  if (pThreadPool == NULL)
    pThreadPool = &MainThreadPool;

  TaskQueue_Init(&pThreadPool->TaskQueue, (int)tasksCapacity);

  pThreadPool->Stopping = false;

  int r = mtx_init(&pThreadPool->Mutex, mtx_plain);
  if (r == thrd_success)
  {
    r = cnd_init(&pThreadPool->Condition);
    if (r == thrd_success)
    {
      for (int i = 0; i < POOL_SIZE; i++)
      {
        r = thrd_create(&pThreadPool->Threads[i], &Thread_N_Loop, (void*)pThreadPool);
        if (r != thrd_success)
        {
          break;
        }
      }
    }
  }
  return r == thrd_success ? 0 : 1;
}

void ThreadPool_Destroy(struct ThreadPool* pThreadPool)
{
  //NULL is the MainThreadPool
  if (pThreadPool == NULL)
    pThreadPool = &MainThreadPool;

  TaskQueue_Destroy(&pThreadPool->TaskQueue);
}

int ThreadPool_Push(struct ThreadPool * pThreadPool,
  TaskFunction pTaskFunction,
  void* pCapture,
  size_t captureSizeBytes)
{
  if (pThreadPool == NULL)
    pThreadPool = &MainThreadPool;

  mtx_lock(&pThreadPool->Mutex);
  int result = 1;

  if (pThreadPool->Stopping)
  {
    result = 2;
    mtx_unlock(&pThreadPool->Mutex);
    pTaskFunction(TASK_CANCELED, pCapture);
  }
  else
  {
    TaskQueue_Push(&pThreadPool->TaskQueue, pTaskFunction, pCapture, (int)captureSizeBytes);
#ifdef BOARD
    AddPost(L"Thread %d/%d", pThreadPool->TaskQueue.Count, pThreadPool->TaskQueue.Capacity);
#endif
    mtx_unlock(&pThreadPool->Mutex);

    /*Unblocks one thread that currently waits on condition variable pointed to by cond.
    If no threads are blocked, does nothing and returns thrd_success.*/

    cnd_signal(&pThreadPool->Condition);
  }
  return result;
}

void ThreadPool_Stop(struct ThreadPool* pThreadPool)
{
  //NULL is the MainThreadPool
  if (pThreadPool == NULL)
    pThreadPool = &MainThreadPool;

  mtx_lock(&pThreadPool->Mutex);

  //assert(!pThreadPool->Stopping);
  pThreadPool->Stopping = true;

  mtx_unlock(&pThreadPool->Mutex);


  /*
  Unblocks all thread that currently wait on condition variable pointed to by cond.
  If no threads are blocked, does nothing and returns thrd_success.
  */
  cnd_broadcast(&pThreadPool->Condition);
}
void ThreadPool_Join(struct ThreadPool* pThreadPool)
{
  //NULL is the MainThreadPool
  if (pThreadPool == NULL)
    pThreadPool = &MainThreadPool;

  mtx_lock(&pThreadPool->Mutex);
  //assert(pTaskQueue->Stopping);
  mtx_unlock(&pThreadPool->Mutex);

  for (size_t i = 0; i < POOL_SIZE; ++i)
  {
    int res;
    /*int r =*/ thrd_join(pThreadPool->Threads[i], &res);
   // assert(r == thrd_success);
  }

  mtx_lock(&pThreadPool->Mutex);
  //assert(pTaskQueue->Count == 0);
  mtx_unlock(&pThreadPool->Mutex);

  mtx_destroy(&pThreadPool->Mutex);
  cnd_destroy(&pThreadPool->Condition);
}
