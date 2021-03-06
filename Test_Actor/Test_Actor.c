#include "..\Actor.h"
#include "..\test.h"

struct Capture {
  int i;
};
static_assert(sizeof(struct Capture) <= sizeof(((struct Task*)0)->pCapture), "");

void Test1Function(enum TASK_ACTION action, void* pCaptureData)
{
  struct Capture* pCapture = (struct Capture*) pCaptureData;

  switch (action)
  {
  case TASK_CANCELED:
    
    break;
  case TASK_FULL:
    
    break;
  case TASK_RUN:

    break;
  default:
    assert(0);
    break;
  }

}


int main()
{
  /*
  Actor is using the main thread pool
  */
  ThreadPool_Init(NULL, 100);


  struct Actor actor;
  Actor_Init(&actor, 100);

  struct Capture  capture = { .i = 1 };

  Actor_Post(&actor, Test1Function, &capture, sizeof(capture));

  
  Actor_Destroy(&actor);

  ThreadPool_Stop(NULL);
  ThreadPool_Join(NULL);
  ThreadPool_Destroy(NULL);
}

