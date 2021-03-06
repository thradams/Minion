
#include "..\ThreadPool.h"
#include "..\test.h"


struct Capture {
  int i;
};
static_assert(sizeof(struct Capture) <= sizeof((struct Task*)0)->pCapture, "");

void Test1Function(enum TASK_ACTION action, void* pCaptureData)
{
  struct Capture* pCapture = (struct Capture*) pCaptureData;

  switch (action)
  {
  case TASK_CANCELED:
    assert(0);
    break;
  case TASK_FULL:
    assert(0);
    break;
  case TASK_RUN:

    break;
  default:
    assert(0);
    break;
  }

}

void Test1()
{
  struct ThreadPool threadPool;
  ThreadPool_Init(&threadPool, 100);

  struct Capture  capture = { .i = 1 };
  ThreadPool_Push(&threadPool, Test1Function, &capture, sizeof(capture));
  
  ThreadPool_Stop(&threadPool);
  
  ThreadPool_Push(&threadPool, Test1Function, &capture, sizeof(capture));

  ThreadPool_Join(&threadPool);
  ThreadPool_Destroy(&threadPool);
}

int main()
{
  Test1();
}

