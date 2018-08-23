#include "TaskPool.h"

QueueHandle_t taskQueueHandle[TaskPoolCounter];

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

//	createTaskPool();

	xTaskCreate(testTask, NULL, MainTaskStackDepth, NULL, MainTaskPriority, NULL);

	vTaskStartScheduler();
}

void mainTaskFunction(void *arg)
{
	//	init hardwave
	uint8_t data[ShortCommandBuffSize] = {0x05, 0x08, 0x01, 0x01, 0x00, 0x00, 0x00};
	TaskQueueItem item;

	MX_FATFS_Init();

	FatFsApi_Error(f_mount(&SDFatFS, SDPath, 1));

	taskQueueAddCommand(pixelBaseList[0], data, pdMS_TO_TICKS(1));
	taskQueueAddCommand(pixelBaseList[1], data, pdMS_TO_TICKS(1));
	taskQueueAddCommand(pixelBaseList[2], data, pdMS_TO_TICKS(1));

	while (true)
	{
		vTaskDelay(1000);
	}
}

void createTaskPool(void)
{
	uint32_t i = 0;

	for (; i < TaskPoolCounter; ++i)
	{
		taskQueueHandle[i] = xQueueCreate(TaskQueueLength, sizeof(TaskQueueItem));
		xTaskCreate(taskPoolFunction, NULL, TaskPoolStackDepth, (void *)(i), TaskPoolPriority, NULL);
	}
}

void taskPoolFunction(void *arg)
{
	uint8_t data[MaxSizeOfBuffByte];
	uint8_t errorCode = ErrorCode_NoneError;
	TaskQueueItem queueItem;
	uint8_t index = (uint8_t)(arg);

	while (true)
	{
		if (taskQueueGetItem(index, &queueItem, TimeoutMs))
		{

			if (taskQueueItemIsTrigger(&queueItem))
			{
				errorCode = taskIsTriggerDeal(queueItem.itemPoint, data);
				switch (errorCode)
				{
				case ErrorCode_BusyError:
					taskQueueAddCommandResend(queueItem.itemPoint, pdMS_TO_TICKS(TimeoutMs));
					DebugBreak();
					break;

				case ErrorCode_CheckError:
					DebugBreak();
					taskQueueAddCommandResend(queueItem.itemPoint, pdMS_TO_TICKS(TimeoutMs));
					DebugBreak();
					break;

				case ErrorCode_CommandError:
					DebugBreak();
					break;

				case ErrorCode_DriverFaliureError:
					DebugBreak();
					break;

				case ErrorCode_NoneError:
					if (PixelBase_AutoGetNextDataPack(queueItem.itemPoint))
					{
						if (PixelBase_NextRequestCommand(queueItem.itemPoint, queueItem.command))
						{
							if (!taskQueueAddCommand(queueItem.itemPoint, queueItem.command, pdMS_TO_TICKS(TimeoutMs)))
							{
								DebugBreak();
							}
						}
					}
					break;

				default:
					DebugBreak();
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

inline bool taskQueueGetItem(uint8_t index, TaskQueueItem *item, uint32_t ms)
{
	return xQueueReceive(taskQueueHandle[index], (void *)(item), pdMS_TO_TICKS(ms)) == pdPASS;
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

	return xQueueSend(taskQueueHandle[itemPoint->transmitHandle->master->index], &item, ms) == pdPASS;
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
	return xQueueSend(taskQueueHandle[itemPoint->transmitHandle->master->index], &item, pdMS_TO_TICKS(ms)) == pdPASS;
}

bool taskQueueAddCommandResend(const TaskItemPoint itemPoint, uint32_t ms)
{
	uint32_t i = 0;
	TaskQueueItem item;
	item.itemPoint = itemPoint;

	for (; i < TaskQueueItemCommandLength; ++i)
	{
		item.command[i] = 0x00;
	}
	return xQueueSend(taskQueueHandle[itemPoint->transmitHandle->master->index], &item, pdMS_TO_TICKS(ms)) == pdPASS;
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

	for (; i < TaskQueueItemCommandLength; ++i)
	{
		item.command[i] = 0xff;
	}
	return xQueueSendFromISR(taskQueueHandle[itemPoint->transmitHandle->master->index], &item, y);
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
	return xQueueSendFromISR(taskQueueHandle[itemPoint->transmitHandle->master->index], &item, y);
}

bool taskQueueItemIsResend(const TaskQueueItem *queueItem)
{
	uint32_t i = 0;
	for (; i < TaskQueueItemCommandLength; ++i)
	{
		if (queueItem->command[i - 1] != 0x00)
		{
			return false;
		}
	}
	return true;
}

void testTask(void *arg)
{
	uint32_t i;
	uint8_t ret;
	uint32_t count;
	char txt[25];

	MX_FATFS_Init();

	FatFsApi_Error(f_mount(&SDFatFS, SDPath, 1));

	while (true)
	{
		FatFsApi_Prepare(100);

		sprintf(txt, "NO:%20d\n", i);
		ret = f_open(&SDFile, "test.txt", FA_CREATE_ALWAYS | FA_WRITE);
		FatFsApi_Error(ret);

		ret = f_lseek(&SDFile, i * 24);
		FatFsApi_Error(ret);

		ret = f_write(&SDFile, txt, 24, &count);
		FatFsApi_Error(ret);

		ret = f_close(&SDFile);
		FatFsApi_Error(ret);

		FatFsApi_End();
		UartDriver_PrepareForTransmit(uartDriverList[0], Transmit, 1);
		UartDriver_Transmit(uartDriverList[0], (const uint8_t *)(txt), 24, 10);
		UartDriver_EndTransmit(uartDriverList[0], Transmit);
		i++;
	}
}

void vApplicationTickHook(void)
{
	uint32_t i = 0;
	BaseType_t y;
	BaseType_t yield = false;
	for (; i < PixelBaseCounter; ++i)
	{
		if ((pixelBaseList[i] != 0x00) && PixelBase_NeedAnswer(*(pixelBaseList + i)))
		{
			if (!PixleBase_HasAnswerIn(*(pixelBaseList + i)))
			{
				(*(pixelBaseList + i))->tickUse++;
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
