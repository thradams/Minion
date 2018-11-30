#pragma once
#include "tinycthread.h"
#include "TaskQueue.h"
#include "ThreadPool.h"

enum ACTOR_STATE
{
  ACTOR_STATE_NONE,
  ACTOR_STATE_RUNNING,
  ACTOR_STATE_ONQUEUE
};

struct Actor
{
  enum ACTOR_STATE state;
  struct TaskQueue TaskQueue;
  mtx_t Mutex;
};

int Actor_Init(struct Actor * pActor, int queueCapacity);
void Actor_Destroy(struct Actor * actor);
void Actor_Post(struct Actor* actor, TaskFunction taskFunction, void* pCapture, size_t captureSizeBytes);


/*

struct X
{
  int i;
};

void X_Init(struct X* p)
{
  p->i = 0;
}

void X_Write(struct X* p, const char* tag, double value)
{
  AddPost(L"Thread/%d Working X_Write", (int)GetCurrentThreadId());
  p->i++;
  Sleep(500);
}

struct X_Actor;
extern struct X_Actor MyActor;

void X_Actor_Write(struct X_Actor* actor, const char* tag, double value);

struct X_Actor
{
  struct Actor Actor;
  struct X X;
};

void X_Actor_Init(struct X_Actor* actor)
{
  Actor_Init(&actor->Actor);
  X_Init(&actor->X);
}

struct Capture {
  struct X_Actor* actor;
  const char* tag;
  double value;
};


static void lambda(enum TASK_ACTION state, void* pdata)
{
  struct Capture* pCapture = (struct Capture*) pdata;
  X_Write(&pCapture->actor->X, pCapture->tag, pCapture->value);
}

void X_Actor_Write(struct X_Actor* actor, const char* tag, double value)
{
  struct Capture capture = { actor, tag, value };
  Actor_Post(&actor->Actor, lambda, &capture, sizeof(capture));
}

struct X_Actor MyActor;

*/
