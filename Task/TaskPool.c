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
				break;
			
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

void taskInit(void)
{
	createSpiMaster();
	createUartDriver();
	createPixelBase();
	FatFsApi_Init();
	xTaskCreate(mainTaskFunction, NULL, MainTaskStackDepth, NULL, MainTaskPriority, NULL);

	createTaskPool();

	vTaskStartScheduler();
}

void mainTaskFunction(void *arg)
{
	//init hardwave
	uint32_t i;
	uint8_t data[ShortCommandBuffSize] = {0x05, 0x08, 0x01, 0x01, 0x00, 0x00, 0x00};
	TaskQueueItem item;

	MX_FATFS_Init();


	if (f_mount(&SDFatFS, SDPath, 1) != FR_OK)
	{
		__breakpoint(0);
	}
	
	taskQueueAddCommand(pixelBaseList[0], data, pdMS_TO_TICKS(1));

	while (true)
	{
		vTaskDelay(1000);
	}

}

void createTaskPool(void)
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
						if (!taskQueueAddCommand(queueItem.itemPoint, queueItem.command, pdMS_TO_TICKS(TimeoutMs)))
						{
							__breakpoint(0);
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



inline bool taskQueueGetItem(TaskQueueItem *item, uint32_t ms)
{
	return xQueueReceive(taskQueueHandle, (void *)(item), pdMS_TO_TICKS(ms)) == pdPASS;
}

inline bool taskQueueAddTrigger(const TaskItemPoint itemPoint, uint32_t ms)
{
	uint32_t i = 0;
	TaskQueueItem item;
	item.itemPoint = itemPoint;

	for (; i < TaskQueueItemCommandLength; ++i)
	{
		item.command[i] = 0xff;
	}

	return xQueueSend(taskQueueHandle, &item, ms) == pdPASS;
}

inline bool taskQueueAddCommand(const TaskItemPoint itemPoint, const uint8_t *data, uint32_t ms)
{
	uint32_t i = 0;
	TaskQueueItem item;
	item.itemPoint = itemPoint;

	for (; i < TaskQueueItemCommandLength; ++i)
	{
		item.command[i] = *(data + i);
	}
	return xQueueSend(taskQueueHandle, &item, pdMS_TO_TICKS(ms)) == pdPASS;
}

inline bool taskQueueItemIsTrigger(const TaskQueueItem *queueItem)
{
	uint32_t i = 0;
	for (; i < TaskQueueItemCommandLength; ++i)
	{
		if (queueItem->command[i] != 0xff)
		{
			return false;
		}
	}
	return true;
}

bool taskQueueAddTriggerFromISR(const TaskItemPoint itemPoint, BaseType_t *y)
{
	uint32_t i = 0;
	TaskQueueItem item;
	item.itemPoint = itemPoint;

	for (; i < TaskQueueItemCommandLength ; ++i)
	{
		item.command[i] = 0xff;
	}
	return xQueueSendFromISR(taskQueueHandle, &item, y);
}

bool taskQueueAddCommandResendFromISR(const TaskItemPoint itemPoint, BaseType_t *y)
{
	uint32_t i = 0;
	TaskQueueItem item;
	item.itemPoint = itemPoint;

	for (; i < TaskQueueItemCommandLength; ++i)
	{
		item.command[i] = 0x00;
	}
	return xQueueSendFromISR(taskQueueHandle, &item, y);
}

bool taskQueueItemIsResend(const TaskQueueItem *queueItem)
{
	uint32_t i = 0;
	for (; i < TaskQueueItemCommandLength ; ++i)
	{
		if (queueItem->command[i - 1] != 0x00)
		{
			return false;
		}
	}
	return true;
}

void vApplicationTickHook(void)
{
	uint32_t i = 0;
	BaseType_t y;
	BaseType_t yield = false;
	for (; i < PixelBaseCounter; ++i)
	{
		if (PixelBase_NeedAnswer(*(pixelBaseList + i)))
		{
			if (!PixleBase_HasAnswerIn(*(pixelBaseList + i)))
			{
				(*(pixelBaseList + i))->tickUse ++;
				if (PixelBase_Timeout(*(pixelBaseList + i)))
				{
					PixelBase_SetStatusFree(*(pixelBaseList + i), true);
					taskQueueAddCommandResendFromISR(*(pixelBaseList + i), &y);

					if (yield != pdTRUE && y == pdTRUE)
					{
						yield = pdTRUE;
					}
					
				}
			}
			else
			{
				taskQueueAddTriggerFromISR(*(pixelBaseList + i), &y);
				if (yield != pdTRUE && y == pdTRUE)
				{
					yield = pdTRUE;
				}
			}
		}
	}

	portYIELD_FROM_ISR(yield);
}
