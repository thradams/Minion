
#include "UITask.h"
#include "TaskQueue.h"



#define WM_MYMESSAGE 1


struct UIActor
{
  struct TaskQueue TaskQueue;
  mtx_t Mutex;
};


struct UIActor UIActor;



int UIActor_Init()
{
  TaskQueue_Init(&UIActor.TaskQueue, 100);
  int r = mtx_init(&UIActor.Mutex, mtx_plain);
  return r;
}

void UIActor_Destroy()
{
  mtx_destroy(&UIActor.Mutex);
}


void UIActorProcess()
{
    struct TaskQueue queue;
    TaskQueue_Init(&queue, 0);

    mtx_lock(&UIActor.Mutex);
    TaskQueue_Swap(&queue, &UIActor.TaskQueue);
    mtx_unlock(&UIActor.Mutex);

    struct Task* pTask = TaskQueue_Pop(&queue);
    while (pTask)
    {
      pTask->pTaskFunction(TASK_RUN, pTask->pCapture);
      pTask = TaskQueue_Pop(&queue);
    }
    //InvalidateRect(s_Hwnd, NULL, TRUE);

    TaskQueue_Destroy(&queue);
}
extern int s_screen_1_dirty;

void UIActor_Post(TaskFunction taskFunction,
  void* pCapture,
  size_t captureSizeBytes)
{
  mtx_lock(&UIActor.Mutex);
  TaskQueue_Push(&UIActor.TaskQueue, taskFunction, pCapture, (int)captureSizeBytes);
  s_screen_1_dirty = 1;
  mtx_unlock(&UIActor.Mutex);

  //if (!PostMessage(s_Hwnd, WM_USER + WM_MYMESSAGE, 0, 0))
  //{
    //DWORD dw = GetLastError();
    //dw = 0;
  //}

  
}
