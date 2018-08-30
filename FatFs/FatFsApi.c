#include "FatFsApi.h"

#ifdef USE_RTOS

SemaphoreHandle_t mutex;

#else
bool isUsed;
#endif

inline void FatFsApi_Init()
{
#ifdef USE_RTOS

	mutex = xSemaphoreCreateBinary();
	xSemaphoreGive(mutex);
#else
	isUsef = false;
#endif
}

inline bool FatFsApi_Prepare(uint32_t ms)
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

void FatFsApi_End()
{
#ifdef USE_RTOS
	xSemaphoreGive(mutex);
#else
	isUse = false;
#endif
}

bool FatFsApi_Error(uint8_t ret)
{
	return true;
}


inline FRESULT FatFsApi_open(FIL* fp, const TCHAR* path, BYTE mode)
{	
	uint8_t ret;
	ret = f_open(fp, path, mode);

	switch (ret)
	{
	case FR_OK:
		return ret;

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
	FatFsApi_End();
	return ret;
	
}

FRESULT FatFsApi_lseek(FIL* fp, DWORD ofs)
{
	uint8_t ret = f_lseek(fp, ofs);
	switch (ret)
	{
	case FR_OK:
		return ret;

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
	FatFsApi_End();
	return ret;
}

FRESULT FatFsApi_write(FIL* fp, const void *buff, UINT btw, UINT* bw)
{
	uint8_t ret = f_write(fp, buff, btw, bw);

	switch (ret)
	{
	case FR_OK:
		return ret;

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
	FatFsApi_End();
	return ret;

	
}

FRESULT FatFsApi_close(FIL* fp)
{
	uint8_t ret = f_close(fp);

	switch (ret)
	{
	case FR_OK:
		return ret;

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
	FatFsApi_End();
	return ret;

}