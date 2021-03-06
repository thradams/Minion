

#include "..\TaskQueue.h"
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
    break;
  case TASK_FULL:
    assert(0);
    break;
  case TASK_RUN:
    assert(0);
    break;
  default:
    assert(0);
    break;
  }

}

void Test1()
{
  struct TaskQueue queue = { .Capacity = 100 };
  assert(queue.Count == 0);
  assert(TaskQueue_Pop(&queue) == NULL);
  assert(queue.Count == 0);

  struct Capture  capture = { .i = 1 };
  TaskQueue_Push(&queue, Test1Function, &capture, sizeof(capture));
  assert(queue.Count == 1);

  /*
    Destroy will call Function with TASK_CANCELED
  */
  TaskQueue_Destroy(&queue);
}

void Test2Function(enum TASK_ACTION action, void* pCaptureData)
{
  struct Capture* pCapture = (struct Capture*) pCaptureData;

  switch (action)
  {
  case TASK_CANCELED:
    break;
  case TASK_FULL:
    assert(0);
    break;
  case TASK_RUN:
    assert(0);
    break;
  default:
    assert(0);
    break;
  }

}


void Test2()
{
  struct TaskQueue queue = { .Capacity = 2147483647 };
  struct Capture  capture = { .i = 1 };

  TaskQueue_Push(&queue, Test2Function, &capture, sizeof(capture));


  TaskQueue_Destroy(&queue);
}

int main()
{
  Test1();
  Test2();
}
