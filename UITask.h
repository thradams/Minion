#pragma once
#include "TaskQueue.h"
#include "tinycthread.h"




int UIActor_Init();
void UIActor_Destroy();
void UIActor_Post(TaskFunction taskFunction, void* pCapture, size_t captureSizeBytes);
void UIActorProcess();