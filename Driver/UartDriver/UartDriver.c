#include "UartDriver.h"

UartDriver *uartDriverList[UartCounter];

#ifdef USE_RTOS
inline static bool UartDriver_Lock(UartDriver *driver, uint8_t direction, uint32_t ms)
{
	return (xEventGroupWaitBits(driver->eventGroup, direction, direction, pdTRUE, pdMS_TO_TICKS(ms)) & direction) > 0;
}

inline static void UartDriver_Unlock(UartDriver *driver, uint8_t direction)
{
	xEventGroupSetBits(driver->eventGroup, direction);
}

#endif

HAL_StatusTypeDef UartDriver_Init(UartDriver *driver, UART_HandleTypeDef *handle, uint32_t index)
{
	HAL_StatusTypeDef temp;
	

#ifdef USE_RTOS
		driver->eventGroup = xEventGroupCreate();
#endif

	UartDriver_EndTransmit(driver, Transmit | Receive);
	UartDriver_PrepareForTransmit(driver, Transmit | Receive, 0);

	driver->status = DriverStatus_Init;
	driver->handle = handle;
	temp = HAL_UART_Init(driver->handle);

	if (temp != HAL_OK)
	{
		driver->status = DriverStatus_Error_DeInit;
		UartDriver_EndTransmit(driver, Transmit | Receive);
		return temp;
	}

	driver->status = DriverStatus_Run;

	driver->transmitStatus = TransmitFree | ReceiveFree | TransmitCompleted | ReceiveCompleted;

	uartDriverList[index] = driver;
	UartDriver_EndTransmit(driver, Transmit | Receive);
	return temp;
}

HAL_StatusTypeDef UartDriver_ReInit(UartDriver *driver, UART_HandleTypeDef *handle, uint32_t forceTimeout)
{
	uint32_t i;
	HAL_StatusTypeDef temp;

	temp = UartDriver_DeInit(driver, forceTimeout);

	if (temp != HAL_OK)
	{
		return temp;
	}

	if (handle != NULL && driver->handle != handle)
	{
		driver->handle->Init.BaudRate = handle->Init.BaudRate;
		driver->handle->Init.WordLength = handle->Init.WordLength;
		driver->handle->Init.StopBits = handle->Init.StopBits;
		driver->handle->Init.Parity = handle->Init.Parity;
		driver->handle->Init.Mode = handle->Init.Mode;
		driver->handle->Init.HwFlowCtl = handle->Init.HwFlowCtl;
		driver->handle->Init.OverSampling = handle->Init.OverSampling;
	}

	for (i = 0; i < UartCounter; ++i)
	{
		if (uartDriverList[i] == driver)
		{
			break;
		}
	}

	return UartDriver_Init(driver, driver->handle, i);
}

HAL_StatusTypeDef UartDriver_DeInit(UartDriver *driver, uint32_t forceTimeout)
{
	HAL_StatusTypeDef temp;

	if (!UartDriver_PrepareForTransmit(driver, Transmit | Receive, forceTimeout)) //等待释放
	{
		//可以添加一个较高的优先级，会添加必要的事务线程，使用线程任务是十分的低效，要频发的切换任务
		UartDriver_EndTransmit(driver, Transmit | Receive);				  //强制释放
		UartDriver_PrepareForTransmit(driver, Transmit | Receive, forceTimeout); //强占，避免在此时去被使用。
	}

	driver->status = DriverStatus_DeInit;
	temp = HAL_UART_DeInit(driver->handle);
	driver->transmitStatus = 0x00;

	if (temp != HAL_OK)
	{
		driver->status = DriverStatus_Error_DeInit;
	}
	return temp;
}

inline HAL_StatusTypeDef UartDriver_Transmit(UartDriver *driver, const uint8_t *pData, uint16_t size, uint32_t ms)
{
	driver->transmitStatus |= TransmitCompleted;
#ifdef USE_RTOS
	xEventGroupSetBits(driver->eventGroup, TransmitCompleted);
#endif
	return HAL_UART_Transmit(driver->handle, (uint8_t *)(pData), size, ms);
}

inline HAL_StatusTypeDef UartDriver_TransmitByDMA(UartDriver *driver, const uint8_t *pData, uint16_t size)
{
	driver->transmitStatus &= (~TransmitCompleted);
	return HAL_UART_Transmit_DMA(driver->handle, (uint8_t *)(pData), size);
}

inline HAL_StatusTypeDef UartDriver_Receive(UartDriver *driver, uint8_t *pData, uint16_t size, uint32_t ms)
{
	driver->transmitStatus |= ReceiveCompleted;
#ifdef USE_RTOS
	xEventGroupSetBits(driver->eventGroup, ReceiveCompleted);
#endif
	return HAL_UART_Receive(driver->handle, pData, size, ms);
}

inline HAL_StatusTypeDef UartDriver_ReceiveByDMA(UartDriver *driver, uint8_t *pData, uint16_t size)
{
	driver->transmitStatus &= (~ReceiveCompleted);
	return HAL_UART_Transmit_DMA(driver->handle, pData, size);
}

inline bool UartDriver_PrepareForTransmit(UartDriver *driver, uint8_t direction, uint32_t ms)
{
#ifdef USE_RTOS
	if (UartDriver_Lock(driver, direction, ms))
	{
		driver->transmitStatus |= direction;
		return true;
	}
#else

	for (; ms > 0; --ms)
	{
		if ((driver->transmitStatus & direction) == direction)
		{
			driver->transmitStatus |= direction;
			return true;
		}
		HAL_Delay(1);
	}
#endif
	return false;
}

inline bool UartDriver_EndTransmit(UartDriver *driver, uint8_t direction)
{
	driver->transmitStatus &= (~direction);
#ifdef USE_RTOS
	UartDriver_Unlock(driver, direction);
#endif
	return true;
}

inline bool UartDriver_WaitForTransmit(UartDriver *driver, uint8_t direction, uint32_t ms)
{
#ifdef USE_RTOS
	return xEventGroupWaitBits(driver->eventGroup, direction << 2, direction << 2, pdTRUE, pdMS_TO_TICKS(ms)) & (direction << 2);
#else
	for (; ms > 0; --ms)
	{
		if ((driver->transmitStatus & (direction << 2)) == (direction << 2))
		{
			return true;
		}
		HAL_Delay(1);
	}
	return false;
#endif
	
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
#ifdef USE_RTOS
	BaseType_t temp = pdFALSE;
#endif

	uint32_t i = 0;
	for (; i < UartCounter; ++i)
	{
		if (uartDriverList[i] != 0 && uartDriverList[i]->handle == huart)
		{
#ifdef USE_RTOS
			if (xEventGroupSetBitsFromISR(uartDriverList[i]->eventGroup, TransmitCompleted, &temp) != pdPASS)
			{
				__breakpoint(0);
			}
#endif
			uartDriverList[i]->transmitStatus |= TransmitCompleted;
			break;
		}
	}

#ifdef USE_RTOS
	portYIELD_FROM_ISR(temp);
#endif
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{

#ifdef USE_RTOS
		BaseType_t temp = pdFALSE;
#endif

		uint32_t i = 0;
		for (; i < UartCounter; ++i)
		{
			if (uartDriverList[i] != 0 && uartDriverList[i]->handle == huart)
			{
#ifdef USE_RTOS
				if (xEventGroupSetBitsFromISR(uartDriverList[i]->eventGroup, ReceiveCompleted, &temp) != pdPASS)
				{
					__breakpoint(0);
				}
#endif
				uartDriverList[i]->transmitStatus |= ReceiveCompleted;
				break;
			}
		}

#ifdef USE_RTOS
		portYIELD_FROM_ISR(temp);
#endif
}
