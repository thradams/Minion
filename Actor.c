
#include "Actor.h"
#ifdef BOARD
#include "Board.h"
#endif

int Actor_Init(struct Actor* pActor, int queueCapacity)
{
  TaskQueue_Init(&pActor->TaskQueue, queueCapacity);
  pActor->state = ACTOR_STATE_NONE;
  int r = mtx_init(&pActor->Mutex, mtx_plain);
#ifdef BOARD  
  AddPost(L"Actor/%p/State NONE", (void*)pActor);
  AddPost(L"Actor/%p/Tasks %d/%d %s", (void*)pActor, pActor->TaskQueue.Count, pActor->TaskQueue.Capacity, pActor->TaskQueue.pTasks == NULL ? L"null" : L"");
  AddPost(L"Actor/%p/Active -", (void*)pActor);
#endif
  return r;
}

void Actor_Destroy(struct Actor* actor) /*default*/
{
}

void ActorProcess(enum TASK_ACTION state, void* pdata)
{
  struct Actor* actor = *((struct Actor**) pdata);

  struct TaskQueue queue;
  TaskQueue_Init(&queue, actor->TaskQueue.Capacity);

  for (;;)
  {
    mtx_lock(&actor->Mutex);

    if (actor->TaskQueue.Count == 0)
    {
      //Nao tem nenhuma tarefa
      //sair do loop
      TaskQueue_Swap(&queue, &actor->TaskQueue);
#ifdef BOARD
      AddPost(L"Actor/%p/Tasks %d/%d %s", (void*)actor, actor->TaskQueue.Count, actor->TaskQueue.Capacity, actor->TaskQueue.pTasks == NULL ? L"null" : L"");
      AddPost(L"Actor/%p/State NONE", (void*)actor);
#endif
      actor->state = ACTOR_STATE_NONE;
      mtx_unlock(&actor->Mutex);
      break;
    }

    actor->state = ACTOR_STATE_RUNNING;
#ifdef BOARD
    AddPost(L"Actor/%p/State RUNNING", (void*)actor);
#endif
    
    TaskQueue_Swap(&queue, &actor->TaskQueue);

#ifdef BOARD
    AddPost(L"Actor/%p/Tasks %d/%d %s", (void*)actor, actor->TaskQueue.Count, actor->TaskQueue.Capacity, actor->TaskQueue.pTasks == NULL ? L"null" : L"");
#endif
    mtx_unlock(&actor->Mutex);

    

    //Roda todas as tasks deste actor
    struct Task* pTask = TaskQueue_Pop(&queue);
#ifdef BOARD
    AddPost(L"Actor/%p/Active %d/%d", (void*)actor, queue.Count, queue.Capacity);
#endif
    while (pTask)
    {
      pTask->pTaskFunction(TASK_RUN, pTask->pCapture);
      pTask = TaskQueue_Pop(&queue);
#ifdef BOARD
      AddPost(L"Actor/%p/Active %d/%d", (void*)actor, queue.Count, queue.Capacity);
#endif
    }
    
#ifdef BOARD
    AddPost(L"Actor/%p/Tasks %d/%d %s", (void*)actor, actor->TaskQueue.Count, actor->TaskQueue.Capacity, actor->TaskQueue.pTasks == NULL ? L"null" : L"");
#endif
    //Reset (reciclagem)
    queue.Count = 0;
    queue.pHead = queue.pTasks;
    queue.pTail = queue.pTasks;    
  }
#ifdef BOARD
  AddPost(L"Actor/%p/Active -", (void*)actor);
#endif
  TaskQueue_Destroy(&queue);
}

extern struct ThreadPool MainThreadPool;

void Actor_Post(struct Actor* actor,
  TaskFunction taskFunction,
  void* pCapture,
  size_t captureSizeBytes)
{
  mtx_lock(&actor->Mutex);

  TaskQueue_Push(&actor->TaskQueue, taskFunction, pCapture, captureSizeBytes);
#ifdef BOARD
  AddPost(L"Actor/%p/Tasks %d/%d %s", (void*)actor, actor->TaskQueue.Count, actor->TaskQueue.Capacity, actor->TaskQueue.pTasks == NULL ? L"null" : L"");
#endif

  switch (actor->state)
  {
  case ACTOR_STATE_NONE:
#ifdef BOARD
    AddPost(L"Actor/%p/State ONQUEUE", (void*)actor);
#endif
    actor->state = ACTOR_STATE_ONQUEUE;
    ThreadPool_Push(&MainThreadPool, ActorProcess, &actor, sizeof(void*));
    break;

  case ACTOR_STATE_ONQUEUE:
  case ACTOR_STATE_RUNNING:
    break;

  default:
    //assert(false);
    break;
  }
  mtx_unlock(&actor->Mutex);
  //return result;
}
