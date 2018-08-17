#ifndef TASKPOOL_H_
#define TASKPOOL_H_

#include <FreeRTOS.h>
#include <task.h>
#include <queue.h>
#include <stdbool.h>
#include <stdint.h>

#include "spi.h"


#include "../Driver/PixelBase/PixelBase.h"
#include "../PortingLayer/GlobalDefine.h"

#ifndef TaskPoolCounter
#define TaskPoolCounter 3
#endif

#ifndef TaskPoolStackDepth
#define TaskPoolStackDepth (2 * 1024 / 4)
#endif

#ifndef TaskPoolPriority
#define TaskPoolPriority 2
#endif

#ifndef MainTaskPriority
#define MainTaskPriority 3
#endif

#ifndef MainTaskStackDepth
#define MainTaskStackDepth (1024 / 4)
#endif

#ifndef TaskQueueLength
#define TaskQueueLength (PixelBaseCounter * 2)
#endif


#define TaskQueueItemCommandLength ShortCommandBuffSize
typedef PixelBase* TaskItemPoint;





extern QueueHandle_t taskQueueHandle;

typedef struct TagQueueTaskItem
{
	uint8_t command[TaskQueueItemCommandLength];
	TaskItemPoint itemPoint;
} TaskQueueItem;

//extern QueueHandle_t queue;

void createMainTask(void);

void taskPoolCreate(void);
void taskPoolFunction(void * arg);

void mainTaskFunction(void *arg);

#define  taskQueueGetItem(item, ms) (xQueueReceive(taskQueueHandle, (void *)(item), pdMS_TO_TICKS(ms)) == pdPASS)


bool taskQueueAddTrigger(const TaskItemPoint itemPoint, uint32_t ms);
bool taskQueueAddTriggerFromISR(const TaskItemPoint itemPoint, BaseType_t *y);

bool taskQueueAddCommand(const TaskItemPoint itemPoint, const uint8_t *data, uint32_t ms);
bool taskQueueAddCommandResend(const TaskItemPoint itemPoint, uint32_t ms);
bool taskQueueAddCommandResendFromISR(const TaskItemPoint itemPoint, BaseType_t *y);

bool taskQueueItemIsTrigger(const TaskQueueItem *queueItem);
bool taskQueueItemIsResend(const TaskQueueItem *queueItem);
	
void vApplicationTickHook(void);

#endif
