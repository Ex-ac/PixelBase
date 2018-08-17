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
		ms --;
		HAL_Delay(1);
		if (!isUsed)
		{
			return true;
			
		}
	}while (ms > 0)
	
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
