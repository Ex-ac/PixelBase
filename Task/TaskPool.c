#include "TaskPool.h"

QueueHandle_t taskQueueHandle;

static uint8_t taskIsTriggerDeal(TaskItemPoint pixelBase, uint8_t *data)
{
	uint8_t errorCode;
	errorCode = PixelBase_GetAnswer(pixelBase, data);
	if (errorCode == ErrorCode_NoneError)
	{
		switch (PixelBase_PackDataAnswerCommand(pixelBase))
		{
			case AnswerCommand_Focus:
				errorCode = PixelBase_FoucsAnswer(pixelBase);
				break;
			
			case AnswerCommand_GetPicturePack:
				errorCode = PixelBase_GetPicturePackAnswer(pixelBase, data);
				break;
			
			case AnswerCommand_Inquiry:
				errorCode = PixelBase_InquiryAnswer(pixelBase);
				break;

			case AnswerCommand_TakePicture:
				errorCode = PixelBase_TakePictureAnswer(pixelBase);
			
			default:
				PixelBase_ReSendRequestCommand(pixelBase);
				errorCode = ErrorCode_NoneError;
				return errorCode;
		}
	}
	else if (errorCode == ErrorCode_CheckError)
	{
		PixelBase_ReSendRequestCommand(pixelBase);
		errorCode = ErrorCode_NoneError;
		return errorCode;
	}

	if (errorCode == ErrorCode_NoneError)
	{
		PixelBase_DealAnswer(pixelBase);
	}

	return errorCode;
}



void createMainTask()
{
	xTaskCreate(mainTaskFunction, NULL, MainTaskStackDepth, NULL, MainTaskPriority, NULL);
}


void taskPoolCreate(void)
{
	uint32_t i = TaskPoolCounter;
	taskQueueHandle = xQueueCreate(TaskQueueLength, sizeof(TaskQueueItem));

	for (; i > 0; --i)
	{
		xTaskCreate(taskPoolFunction, NULL, TaskPoolStackDepth, NULL, TaskPoolPriority, NULL);
	}
}

void taskPoolFunction(void *arg)
{
	uint8_t data[MaxSizeOfBuffByte];
	uint8_t errorCode = ErrorCode_NoneError;
	TaskQueueItem queueItem;
	

	while (true)
	{
		if (taskQueueGetItem(&queueItem, TimeoutMs))
		{

			if (taskQueueItemIsTrigger(&queueItem))
			{
				errorCode = taskIsTriggerDeal(queueItem.itemPoint, data);
				if (errorCode == ErrorCode_NoneError && PixelBase_AutoGetNextDataPack(queueItem.itemPoint))
				{
					if (PixelBase_NextRequestCommand(queueItem.itemPoint, queueItem.command))
					{
						while (taskQueueAddCommand(queueItem.itemPoint, queueItem.command, pdMS_TO_TICKS(TimeoutMs)))
						{

						}
					}
				}
			}
			else if (taskQueueItemIsResend(&queueItem))
			{
				PixelBase_ReSendRequestCommand(queueItem.itemPoint);
			}
			else
			{
				PixelBase_SendRequestCommand(queueItem.itemPoint, queueItem.command);
			}
		}
		vTaskDelay(TimeoutMs);
	}
}


void mainTaskFunction(void *arg)
{
	
}
inline bool taskQueueAddTrigger(const TaskItemPoint itemPoint, uint32_t ms)
{
	uint32_t i = TaskQueueItemCommandLength;
	TaskQueueItem item;
	item.itemPoint = itemPoint;

	for (; i > 0; --i)
	{
		item.command[i - 1] = 0xff;
	}

	return xQueueSend(taskQueueHandle, &item, ms) == pdPASS;
}

inline bool taskQueueAddCommand(const TaskItemPoint itemPoint, const uint8_t *data, uint32_t ms)
{
	uint32_t i = TaskQueueItemCommandLength;
	TaskQueueItem item;
	item.itemPoint = itemPoint;

	for (; i > 0; --i)
	{
		item.command[i - 1] = *((data) + i - 1);
	}
	return xQueueSend(taskQueueHandle, &item, pdMS_TO_TICKS(ms)) == pdPASS;
}

inline bool taskQueueItemIsTrigger(const TaskQueueItem *queueItem)
{
	uint32_t i = TaskQueueItemCommandLength;
	for (; i > 0; --i)
	{
		if (queueItem->command[i - 1] != 0xff)
		{
			return false;
		}
	}
	return true;
}

bool taskQueueAddTriggerFromISR(const TaskItemPoint itemPoint, BaseType_t *y)
{
	uint32_t i = TaskQueueItemCommandLength;
	TaskQueueItem item;
	item.itemPoint = itemPoint;

	for (; i > 0; --i)
	{
		item.command[i - 1] = 0xff;
	}
	return xQueueSendFromISR(taskQueueHandle, &item, y);
}
bool taskQueueAddCommandResendFromISR(const TaskItemPoint itemPoint, BaseType_t *y)
{
	uint32_t i = TaskQueueItemCommandLength;
	TaskQueueItem item;
	item.itemPoint = itemPoint;

	for (; i > 0; --i)
	{
		item.command[i - 1] = 0x00;
	}
	return xQueueSendFromISR(taskQueueHandle, &item, y);
}

bool taskQueueItemIsResend(const TaskQueueItem *queueItem)
{
	uint32_t i = TaskQueueItemCommandLength;
	for (; i > 0; --i)
	{
		if (queueItem->command[i - 1] != 0xff)
		{
			return false;
		}
	}
	return true;
}

void vApplicationTickHook(void)
{
	uint32_t i = PixelBaseCounter;
	BaseType_t y;
	BaseType_t yield = false;
	for (; i > 0; --i)
	{
		if (PixelBase_NeedAnswer(*(pixelBaseList + i - 1)))
		{
			if (!PixleBase_HasAnswerIn(*(pixelBaseList + i - 1)))
			{
				(*(pixelBaseList + i - 1))->tickUse ++;
				if (PixelBase_Timeout(*(pixelBaseList + i - 1)))
				{
					PixelBase_SetStatusFree(*(pixelBaseList + i - 1), true);
					taskQueueAddCommandResendFromISR(*(pixelBaseList + i - 1), &y);

					if (yield != pdTRUE && y == pdTRUE)
					{
						yield = pdTRUE;
					}
					
				}
			}
		}
	}

	portYIELD_FROM_ISR(yield);
}
