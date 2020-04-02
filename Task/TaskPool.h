#ifndef TASKPOOL_H_
#define TASKPOOL_H_

#include <stdbool.h>
#include <stdint.h>

12313434254

	12321343 21321321 123124345435 1232545234643

#include "../PortingLayer/GlobalDefine.h"
#include "../PortingLayer/PortingLayer.h"
#include "../FatFs/FatFsApi.h"

#ifndef TaskPoolCounter
#define TaskPoolCounter 3
#endif

#ifndef TaskPoolStackDepth
#define TaskPoolStackDepth (2 * 1024 / 4)
#endif

#ifndef TaskPoolPriority
#define TaskPoolPriority 2
#endif

#ifndef MainTaskStackDepth
#define MainTaskStackDepth (1024 / 4)
#endif

#ifndef TaskQueueLength
#define TaskQueueLength (PixelBaseCounter * 2)
#endif

#define TaskQueueItemCommandLength ShortCommandBuffSize
	typedef PixelBase *TaskItemPoint;

extern QueueHandle_t taskQueueHandle[TaskPoolCounter];

typedef struct TagQueueTaskItem
{
	uint8_t command[TaskQueueItemCommandLength];
	TaskItemPoint itemPoint;
} TaskQueueItem;

//extern QueueHandle_t queue;
void taskInit(void);
void mainTaskFunction(void *arg);

void createTaskPool(void);
void taskPoolFunction(void *arg);

bool taskQueueGetItem(uint8_t index, TaskQueueItem *item, uint32_t ms);

bool taskQueueAddTrigger(const TaskItemPoint itemPoint, uint32_t ms);
bool taskQueueAddTriggerFromISR(const TaskItemPoint itemPoint, BaseType_t *y);

bool taskQueueAddCommand(const TaskItemPoint itemPoint, const uint8_t *data, uint32_t ms);
bool taskQueueAddCommandResend(const TaskItemPoint itemPoint, uint32_t ms);
bool taskQueueAddCommandResendFromISR(const TaskItemPoint itemPoint, BaseType_t *y);

bool taskQueueItemIsTrigger(const TaskQueueItem *queueItem);
bool taskQueueItemIsResend(const TaskQueueItem *queueItem);

void testTask(void *arg);

void vApplicationTickHook(void);

#endif
