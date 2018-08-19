#include "SpiDriver.h"

SpiMaster *spiMasterList[SpiCounter] = {0x00};

#ifdef USE_RTOS

inline static bool SpiMaster_Lock(SpiMaster *driver, uint8_t direction, uint32_t ms)
{
	return (xEventGroupWaitBits(driver->eventGroup, Transmit | Receive, Transmit | Receive, pdTRUE, pdMS_TO_TICKS(ms)) & (Transmit | Receive)) > 0;
}

inline static void SpiMaster_Unlock(SpiMaster *driver, uint8_t direction)
{
	xEventGroupSetBits(driver->eventGroup, Transmit | Receive);
}

#endif

HAL_StatusTypeDef SpiMaster_Init(SpiMaster *driver, SPI_HandleTypeDef *handle, uint32_t index)
{
	HAL_StatusTypeDef temp;

#ifdef USE_RTOS
	driver->eventGroup = xEventGroupCreate();
#endif

	SpiMaster_EndTransmit(driver, Transmit | Receive);
	SpiMaster_PrepareForTransmit(driver, Transmit | Receive, 0);

	driver->currentSlaver = NULL;
	driver->handle = handle;
	driver->status = DriverStatus_Init;

	temp = HAL_SPI_Init(driver->handle);

	if (temp != HAL_OK)
	{
		driver->status = DriverStatus_Error_Init;
		SpiMaster_EndTransmit(driver, Transmit | Receive);
		return temp;
	}

	driver->status = DriverStatus_Run;
	driver->transmitStatus = TransmitFree | ReceiveFree | TransmitCompleted | ReceiveCompleted;

	spiMasterList[index] = driver;

	SpiMaster_EndTransmit(driver, Transmit | Receive);
	return temp;
}

HAL_StatusTypeDef SpiMaster_ReInit(SpiMaster *driver, SPI_HandleTypeDef *handle, uint32_t forceTimeout)
{
	uint32_t i;
	HAL_StatusTypeDef temp;

	temp = SpiMaster_DeInit(driver, forceTimeout);
	if (temp != HAL_OK)
	{
		return temp;
	}

	if (handle != NULL && driver->handle != handle)
	{
		driver->handle->Init.Mode = handle->Init.Mode;
		driver->handle->Init.Direction = handle->Init.Direction;
		driver->handle->Init.DataSize = handle->Init.DataSize;
		driver->handle->Init.CLKPolarity = handle->Init.CLKPolarity;
		driver->handle->Init.CLKPhase = handle->Init.CLKPhase;
		driver->handle->Init.NSS = handle->Init.NSS;
		driver->handle->Init.BaudRatePrescaler = handle->Init.BaudRatePrescaler;
		driver->handle->Init.FirstBit = handle->Init.FirstBit;
		driver->handle->Init.TIMode = handle->Init.TIMode;
		driver->handle->Init.CRCCalculation = handle->Init.CRCCalculation;
		driver->handle->Init.CRCPolynomial = handle->Init.CRCPolynomial;
	}
	for (i = 0; i < SpiCounter; ++i)
	{
		if (spiMasterList[i] == driver)
		{
			break;
		}
	}
	return SpiMaster_Init(driver, driver->handle, i);
}

HAL_StatusTypeDef SpiMaster_DeInit(SpiMaster *driver, uint32_t forceTimeout)
{
	HAL_StatusTypeDef temp;

	if (!SpiMaster_PrepareForTransmit(driver, Transmit | Receive, forceTimeout))
	{
		if (driver->currentSlaver != NULL)
		{
			SpiSlaver_EndTransmit(driver->currentSlaver, Transmit | Receive);
		}
		else
		{
			SpiMaster_EndTransmit(driver, Transmit | Receive);
		}
		SpiMaster_PrepareForTransmit(driver, Transmit | Receive, forceTimeout);
	}

	driver->status = DriverStatus_DeInit;
	temp = HAL_SPI_DeInit(driver->handle);
	driver->transmitStatus = 0x00;

	if (temp != HAL_OK)
	{
		driver->status = DriverStatus_Error_DeInit;
	}

	return temp;
}

inline SpiSlaver *SpiMaster_CurrentSlaver(SpiMaster *driver)
{
	return driver->currentSlaver;
}

inline HAL_StatusTypeDef SpiMaster_Transmit(SpiMaster *driver, const uint8_t *pData, uint16_t size, uint32_t ms)
{
	driver->transmitStatus |= TransmitCompleted;
#ifdef USE_RTOS
	xEventGroupSetBits(driver->eventGroup, TransmitCompleted);
#endif
	return HAL_SPI_Transmit(driver->handle, (uint8_t *)(pData), size, ms);
}
inline HAL_StatusTypeDef SpiMaster_TransmitByDMA(SpiMaster *driver, const uint8_t *pData, uint16_t size)
{
	driver->transmitStatus &= (~TransmitCompleted);
	return HAL_SPI_Transmit_DMA(driver->handle, (uint8_t *)(pData), size);
}

inline HAL_StatusTypeDef SpiMaster_Receive(SpiMaster *driver, uint8_t *pData, uint16_t size, uint32_t ms)
{
	driver->transmitStatus |= ReceiveCompleted;
#ifdef USE_RTOS
	xEventGroupSetBits(driver->eventGroup, ReceiveCompleted);
#endif
	return HAL_SPI_Receive(driver->handle, pData, size, ms);
}
inline HAL_StatusTypeDef SpiMaster_ReceiveByDMA(SpiMaster *driver, uint8_t *pData, uint16_t size)
{
	driver->transmitStatus &= (~ReceiveCompleted);
	return HAL_SPI_Transmit_DMA(driver->handle, pData, size);
}

inline bool SpiMaster_PrepareForTransmit(SpiMaster *driver, uint8_t direction, uint32_t ms)
{
	direction = Transmit | Receive;
#ifdef USE_RTOS
	if (SpiMaster_Lock(driver, direction, ms))
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
inline bool SpiMaster_EndTransmit(SpiMaster *driver, uint8_t direction)
{
	direction = Transmit | Receive;
	driver->transmitStatus &= (~direction);
	driver->currentSlaver = NULL;
#ifdef USE_RTOS
	SpiMaster_Unlock(driver, direction);
#endif
	return true;
}

inline bool SpiMaster_WaitForTransmit(SpiMaster *driver, uint8_t direction, uint32_t ms)
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




void SpiSlaver_Init(SpiSlaver *slaver, SpiMaster *master, GPIO_TypeDef *port, uint16_t pin)
{
	GPIO_InitTypeDef csnPinInitData;

	slaver->master = master;
	slaver->csnPin.port = port;
	slaver->csnPin.pin = pin;

	csnPinInitData.Pin = pin;
	csnPinInitData.Mode = GPIO_MODE_OUTPUT_PP;
	csnPinInitData.Pull = GPIO_PULLUP;
	csnPinInitData.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GpioPin_Init(&(slaver->csnPin), &csnPinInitData);

	GpioPin_OutputHight(&(slaver->csnPin), true);
}

inline bool SpiSlaver_PrepareForTransmit(SpiSlaver *slaver, uint8_t direction, uint32_t ms)
{
	if (SpiMaster_PrepareForTransmit(slaver->master, direction, ms))
	{
		GpioPin_OutputHight(&(slaver->csnPin), false);
		slaver->master->currentSlaver = slaver;
		return true;
	}
	return false;
}
inline bool SpiSlaver_EndTransmit(SpiSlaver *slaver, uint8_t direction)
{
	if (SpiMaster_CurrentSlaver(slaver->master) != slaver)
	{
		return false;
	}
	GpioPin_OutputHight(&(slaver->csnPin), true);

	return SpiMaster_EndTransmit(slaver->master, direction);
}

inline bool SpiSlaver_WaitForTransmit(SpiSlaver *slaver, uint8_t direction, uint32_t ms)
{
	return SpiMaster_WaitForTransmit(slaver->master, direction, ms);
}

inline HAL_StatusTypeDef SpiSlaver_Transmit(SpiSlaver *slaver, const uint8_t *pData, uint16_t size, uint32_t ms)
{
	return SpiMaster_Transmit(slaver->master, pData, size, ms);
}
inline HAL_StatusTypeDef SpiSlaver_TransmitByDMA(SpiSlaver *slaver, const uint8_t *pData, uint16_t size)
{
	return SpiMaster_TransmitByDMA(slaver->master, pData, size);
}

inline HAL_StatusTypeDef SpiSlaver_Receive(SpiSlaver *slaver, uint8_t *pData, uint16_t size, uint32_t ms)
{
	return SpiMaster_Receive(slaver->master, pData, size, ms);
}
inline HAL_StatusTypeDef SpiSlaver_ReceiveByDMA(SpiSlaver *slaver, uint8_t *pData, uint16_t size)
{
	return SpiMaster_ReceiveByDMA(slaver->master, pData, size);
}






void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi)
{
#ifdef USE_RTOS
	BaseType_t temp = pdFALSE;
#endif

	uint32_t i = 0;
	for (; i < SpiCounter; ++i)
	{
		if (spiMasterList[i] != 0 && spiMasterList[i]->handle == hspi)
		{
#ifdef USE_RTOS
			if (xEventGroupSetBitsFromISR(spiMasterList[i]->eventGroup, TransmitCompleted, &temp) != pdPASS)
			{
				__breakpoint(0);
			}
#endif
			spiMasterList[i]->transmitStatus |= TransmitCompleted;
			break;
		}
	}

#ifdef USE_RTOS
	portYIELD_FROM_ISR(temp);
#endif
}

void HAL_SPI_RxCpltCallback(SPI_HandleTypeDef *hspi)
{
#ifdef USE_RTOS
	BaseType_t temp = pdFALSE;
#endif

	uint32_t i = 0;
	for (; i < SpiCounter; ++i)
	{
		if (spiMasterList[i] != 0 && spiMasterList[i]->handle == hspi)
		{
#ifdef USE_RTOS
			if (xEventGroupSetBitsFromISR(spiMasterList[i]->eventGroup, ReceiveCompleted, &temp) != pdPASS)
			{
				__breakpoint(0);
			}
#endif
			spiMasterList[i]->transmitStatus |= ReceiveCompleted;
			break;
		}
	}

#ifdef USE_RTOS
	portYIELD_FROM_ISR(temp);
#endif
}
