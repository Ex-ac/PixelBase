#include "FatfsApi.h"
#include <rtc.h>

static char dirPath[DirPathWidth];

inline bool FatfsApi_Open(FIL *fp, const TCHAR *path, BYTE mode)
{
	uint8_t ret;
	ret = f_open(fp, path, mode);

	switch (ret)
	{
	case FR_OK:
		return true;

	case FR_DISK_ERR:
		DebugBreak();
		break;

	case FR_INT_ERR:
		DebugBreak();
		break;

	case FR_NOT_READY:
		DebugBreak();
		break;

	case FR_INVALID_NAME:
		DebugBreak();
		break;

	case FR_EXIST:
		DebugBreak();
		break;

	case FR_INVALID_OBJECT:
		DebugBreak();
		break;

	case FR_WRITE_PROTECTED:
		DebugBreak();
		break;

	case FR_INVALID_DRIVE:
		DebugBreak();
		break;

	case FR_NOT_ENABLED:
		DebugBreak();
		break;

	case FR_NO_FILESYSTEM:
		DebugBreak();
		break;

	case FR_MKFS_ABORTED:
		DebugBreak();
		break;

	case FR_TIMEOUT:
		DebugBreak();
		break;

	case FR_LOCKED:
		DebugBreak();
		break;

	case FR_NOT_ENOUGH_CORE:
		DebugBreak();
		break;

	case FR_TOO_MANY_OPEN_FILES:
		DebugBreak();
		break;

	case FR_INVALID_PARAMETER:
		DebugBreak();
		break;
	}
	return false;
}

inline bool FatfsApi_Lseek(FIL *fp, FSIZE_t ofs)
{
	uint8_t ret = f_lseek(fp, ofs);
	switch (ret)
	{
	case FR_OK:
		return true;

	case FR_DISK_ERR:
		DebugBreak();
		break;

	case FR_INT_ERR:
		DebugBreak();
		break;

	case FR_NOT_READY:
		DebugBreak();
		break;

	case FR_INVALID_NAME:
		DebugBreak();
		break;

	case FR_EXIST:
		DebugBreak();
		break;

	case FR_INVALID_OBJECT:
		DebugBreak();
		break;

	case FR_WRITE_PROTECTED:
		DebugBreak();
		break;

	case FR_INVALID_DRIVE:
		DebugBreak();
		break;

	case FR_NOT_ENABLED:
		DebugBreak();
		break;

	case FR_NO_FILESYSTEM:
		DebugBreak();
		break;

	case FR_MKFS_ABORTED:
		DebugBreak();
		break;

	case FR_TIMEOUT:
		DebugBreak();
		break;

	case FR_LOCKED:
		DebugBreak();
		break;

	case FR_NOT_ENOUGH_CORE:
		DebugBreak();
		break;

	case FR_TOO_MANY_OPEN_FILES:
		DebugBreak();
		break;

	case FR_INVALID_PARAMETER:
		DebugBreak();
		break;
	}
	return false;
}

inline bool FatfsApi_Write(FIL *fp, const void *buff, UINT btw, UINT *bw)
{
	uint8_t ret = f_write(fp, buff, btw, bw);

	switch (ret)
	{
	case FR_OK:
		return true;

	case FR_DISK_ERR:
		DebugBreak();
		break;

	case FR_INT_ERR:
		DebugBreak();
		break;

	case FR_NOT_READY:
		DebugBreak();
		break;

	case FR_INVALID_NAME:
		DebugBreak();
		break;

	case FR_EXIST:
		DebugBreak();
		break;

	case FR_INVALID_OBJECT:
		DebugBreak();
		break;

	case FR_WRITE_PROTECTED:
		DebugBreak();
		break;

	case FR_INVALID_DRIVE:
		DebugBreak();
		break;

	case FR_NOT_ENABLED:
		DebugBreak();
		break;

	case FR_NO_FILESYSTEM:
		DebugBreak();
		break;

	case FR_MKFS_ABORTED:
		DebugBreak();
		break;

	case FR_TIMEOUT:
		DebugBreak();
		break;

	case FR_LOCKED:
		DebugBreak();
		break;

	case FR_NOT_ENOUGH_CORE:
		DebugBreak();
		break;

	case FR_TOO_MANY_OPEN_FILES:
		DebugBreak();
		break;

	case FR_INVALID_PARAMETER:
		DebugBreak();
		break;
	}
	return false;
}

inline bool FatfsApi_Close(FIL *fp)
{
	uint8_t ret = f_close(fp);

	switch (ret)
	{
	case FR_OK:
		return true;

	case FR_DISK_ERR:
		DebugBreak();
		break;

	case FR_INT_ERR:
		DebugBreak();
		break;

	case FR_NOT_READY:
		DebugBreak();
		break;

	case FR_INVALID_NAME:
		DebugBreak();
		break;

	case FR_EXIST:
		DebugBreak();
		break;

	case FR_INVALID_OBJECT:
		DebugBreak();
		break;

	case FR_WRITE_PROTECTED:
		DebugBreak();
		break;

	case FR_INVALID_DRIVE:
		DebugBreak();
		break;

	case FR_NOT_ENABLED:
		DebugBreak();
		break;

	case FR_NO_FILESYSTEM:
		DebugBreak();
		break;

	case FR_MKFS_ABORTED:
		DebugBreak();
		break;

	case FR_TIMEOUT:
		DebugBreak();
		break;

	case FR_LOCKED:
		DebugBreak();
		break;

	case FR_NOT_ENOUGH_CORE:
		DebugBreak();
		break;

	case FR_TOO_MANY_OPEN_FILES:
		DebugBreak();
		break;

	case FR_INVALID_PARAMETER:
		DebugBreak();
		break;
	}
	return false;
}

inline bool FatfsApi_MkDir(const char *path)
{
	uint8_t ret = f_mkdir(path);

	switch (ret)
	{
	case FR_OK:
		return true;

	case FR_DISK_ERR:
		DebugBreak();
		break;

	case FR_INT_ERR:
		DebugBreak();
		break;

	case FR_NOT_READY:
		DebugBreak();
		break;

	case FR_INVALID_NAME:
		DebugBreak();
		break;

	case FR_EXIST:
		DebugBreak();
		break;

	case FR_INVALID_OBJECT:
		DebugBreak();
		break;

	case FR_WRITE_PROTECTED:
		DebugBreak();
		break;

	case FR_INVALID_DRIVE:
		DebugBreak();
		break;

	case FR_NOT_ENABLED:
		DebugBreak();
		break;

	case FR_NO_FILESYSTEM:
		DebugBreak();
		break;

	case FR_MKFS_ABORTED:
		DebugBreak();
		break;

	case FR_TIMEOUT:
		DebugBreak();
		break;

	case FR_LOCKED:
		DebugBreak();
		break;

	case FR_NOT_ENOUGH_CORE:
		DebugBreak();
		break;

	case FR_TOO_MANY_OPEN_FILES:
		DebugBreak();
		break;

	case FR_INVALID_PARAMETER:
		DebugBreak();
		break;
	}
	return false;
}

#ifdef USE_FatfsThread

static QueueHandle_t fatfsThreadQueue;

// static uint8_t *firstAddress;
// static uint8_t *controlBlockAddress;
void FatfsThread_Init(void)
{
	fatfsThreadQueue = xQueueCreate(1, sizeof(FatfsThreadQueueItem));

	xTaskCreate(FatfsThread_TaskFunction, NULL, FatfsThreadTaskStackDepth, NULL, FatfsThreadTaskPriority, NULL);
}
bool FatfsThread_AddCreateSaveDirCommand(const RTC_TimeTypeDef *time, const RTC_DateTypeDef *date, uint32_t ms)
{
	FatfsThreadQueueItem item;

	item.data[0] = date->Year;
	item.data[1] = date->Month;
	item.data[2] = date->Date;
	item.data[3] = time->Hours;
	item.data[4] = time->Minutes;
	item.data[5] = time->Seconds;
	item.data[6] = (uint8_t)(time->SubSeconds >> 24);
	item.data[7] = (uint8_t)(time->SubSeconds >> 16);
	item.data[8] = (uint8_t)(time->SubSeconds >> 8);
	item.data[9] = (uint8_t)(time->SubSeconds);
	return xQueueSend(fatfsThreadQueue, &item, pdMS_TO_TICKS(ms)) == pdPASS;
}
bool FatfsThread_AddCreatePixelBaseDirCommand(uint8_t id, uint32_t ms)
{
	FatfsThreadQueueItem item;
	item.data[1] = 0xff;
	item.data[2] = 0xff;
	item.data[0] = id;
	return xQueueSend(fatfsThreadQueue, (void *)(&item), pdMS_TO_TICKS(ms)) == pdPASS;
}

bool FatfsThread_AddSavePixelBaseDataCommand(uint8_t id, uint32_t ms)
{
	FatfsThreadQueueItem item;
	item.data[1] = 0xee;
	item.data[2] = 0xee;
	item.data[0] = id;
	return xQueueSend(fatfsThreadQueue, (void *)(&item), pdMS_TO_TICKS(ms)) == pdPASS;
}

bool FatfsThread_IsCreatePixelBaseDirCommand(const FatfsThreadQueueItem *item)
{
	if (item->data[1] == 0xff && item->data[2] == 0xff)
	{
		return true;
	}
	return false;
}
bool FatfsThread_IsSavePixelBaseDataCommand(const FatfsThreadQueueItem *item)
{
	if (item->data[1] == 0xee && item->data[2] == 0xee)
	{
		return true;
	}
	return false;
}

void FatfsThread_TaskFunction(void *arg)
{
	FatfsThreadQueueItem item;
	char path[32];
	PixelBase *pixelBase;
	bool ok;
	uint32_t count;
	FIL file;

	MX_FATFS_Init();

	if (f_mount(&SDFatFS, SDPath, 1) != FR_OK)
	{
		DebugBreak();
	}

	while (true)
	{
		if (xQueueReceive(fatfsThreadQueue, (void *)(&item), pdMS_TO_TICKS(1000)) == pdPASS)
		{
			if (FatfsThread_IsCreatePixelBaseDirCommand(&item))
			{
				sprintf(path, "%s\\%2d", dirPath, item.data[0]);

				for (int32_t i = 0; i < 32; ++i)
				{
					if (path[i] == ' ')
					{
						path[i] = '0';
					}
				}
				while (!FatfsApi_MkDir(path))
				{
					delayMs(1);
				}
			}
			else if (FatfsThread_IsSavePixelBaseDataCommand(&item))
			{
				pixelBase = pixelBaseList[item.data[0]];
				sprintf(path, "%s\\%2d\\%3d.data", dirPath, pixelBase->id, (pixelBase->packData.numberOfPack - 1) / PixelBaseFileFragmentSize_KB);

				for (int32_t i = 0; i < 32; ++i)
				{
					if (path[i] == ' ')
					{
						path[i] = '0';
					}
				}

				do
				{
					ok = true;

					if (ok && !FatfsApi_Open(&file, path, FA_CREATE_ALWAYS | FA_WRITE))
					{
						ok = false;
					}

					if (ok && !FatfsApi_Lseek(&file, (pixelBase->packData.numberOfPack - 1) % PixelBaseFileFragmentSize_KB * 1024))
					{
						ok = false;
					}

					if (ok && !FatfsApi_Write(&file, pixelBase->packData.data, pixelBase->packData.sizeOfByte, &count))
					{
						ok = false;
					}
					if (!FatfsApi_Close(&file))
					{
						ok = false;
					}

					if (!ok)
					{
						delayMs(1);
					}
				} while (!ok);
			}
			else
			{
				uint32_t subSeconds = (uint32_t)(item.data[9]) + (uint32_t)(item.data[8] << 8) + (uint32_t)(item.data[7] << 16) + (uint32_t)(item.data[6] << 24);

				sprintf(dirPath, "%2d%2d%2d_%2d%2d%2d%3d", item.data[0], item.data[1], item.data[2], item.data[3], item.data[4], item.data[5], subSeconds);

				for (int32_t i = 0; i < DirPathWidth; ++i)
				{
					if (dirPath[i] == ' ')
					{
						dirPath[i] = '0';
					}
				}

				while (!FatfsApi_MkDir(dirPath))
				{
					delayMs(1);
				}
			}
		}
	}
}

// bool MemaryPool_Create();
// uint8_t *MemaryPool_Request();
// void MemaryPool_Release(uint8_t *data);

#else

#ifdef USE_RTOS

SemaphoreHandle_t mutex;

#else
bool isUsed;
#endif

inline void FatfsApi_Init()
{
#ifdef USE_RTOS

	mutex = xSemaphoreCreateBinary();
	xSemaphoreGive(mutex);
#else
	isUsef = false;
#endif
}

inline bool FatfsApi_Prepare(uint32_t ms)
{
#ifdef USE_RTOS
	return xSemaphoreTake(mutex, pdMS_TO_TICKS(ms)) == pdPASS;
#else
	if (!isUsed)
	{
		return true;
	}
	do
	{
		ms--;
		HAL_Delay(1);
		if (!isUsed)
		{
			return true;
		}
	} while (ms > 0)

		return false;

#endif
}

void FatfsApi_End()
{
#ifdef USE_RTOS
	xSemaphoreGive(mutex);
#else
	isUse = false;
#endif
}

bool FatfsApi_Error(uint8_t ret)
{
	return true;
}

#endif
