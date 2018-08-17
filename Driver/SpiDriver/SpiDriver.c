#include "SpiDriver.h"

SpiMaster *spiMasterList[SpiCounter] = {0x00};

HAL_StatusTypeDef SpiMaster_Init(SpiMaster *master, SPI_HandleTypeDef *spiHandle, uint32_t index)
{
	HAL_StatusTypeDef temp;
    
	spiMasterList[index] = master;

#ifdef USE_RTOS
	master->eventGroup = xEventGroupCreate();
#endif
	
	master->currentSlaver = NULL;
	master->spiHandle = spiHandle;
	master->status = DriverStatus_Init;
	
	
	
    temp = HAL_SPI_Init(master->spiHandle);
	
	if (temp != HAL_OK)
    {
        master->status = DriverStatus_Error_Init;

        return temp;
	}
	
    master->status = DriverStatus_Run;
	master->transmitStatus = TransmitFree | ReceiveFree | TransmitCompleted | ReceiveCompleted;
	SpiMaster_Unlock(master);
    return temp;
}

HAL_StatusTypeDef SpiMaster_ReInit(SpiMaster *master, SPI_HandleTypeDef *spiHandle)
{
	uint32_t i;
	HAL_StatusTypeDef temp;
	
	temp = SpiMaster_DeInit(master);
    if (temp != HAL_OK)
    {
        return temp;
    }
	
	if (spiHandle != NULL && master->spiHandle != spiHandle)
	{
		master->spiHandle->Init.Mode = spiHandle->Init.Mode;
		master->spiHandle->Init.Direction = spiHandle->Init.Direction;
		master->spiHandle->Init.DataSize = spiHandle->Init.DataSize;
		master->spiHandle->Init.CLKPolarity = spiHandle->Init.CLKPolarity;
		master->spiHandle->Init.CLKPhase = spiHandle->Init.CLKPhase;
		master->spiHandle->Init.NSS = spiHandle->Init.NSS;
		master->spiHandle->Init.BaudRatePrescaler = spiHandle->Init.BaudRatePrescaler;
		master->spiHandle->Init.FirstBit = spiHandle->Init.FirstBit;
		master->spiHandle->Init.TIMode = spiHandle->Init.TIMode;
		master->spiHandle->Init.CRCCalculation = spiHandle->Init.CRCCalculation;
		master->spiHandle->Init.CRCPolynomial = spiHandle->Init.CRCPolynomial;
	}
	for (i = 0; i < SpiCounter; ++i)
	{
		if (spiMasterList[i] == master)
		{
			break;
		}
	}
    return SpiMaster_Init(master, master->spiHandle, i);
}

HAL_StatusTypeDef SpiMaster_DeInit(SpiMaster *master)
{
    HAL_StatusTypeDef temp;
    uint32_t timeout = 0xff;
	

	
    while (master->currentSlaver != NULL && timeout != 0)
    {
#ifdef USE_RTOS
		vTaskDelay(pdMS_TO_TICKS(1));
#else
        HAL_Delay(1);
#endif
        timeout --;
    }
	
    if (timeout == 0)
    {
        SpiMaster_Released(master, 0);
    }

	
    master->status = DriverStatus_DeInit;
    temp = HAL_SPI_DeInit(master->spiHandle);
	master->transmitStatus = 0x00;
	
    if (temp != HAL_OK)
    {
        master->status = DriverStatus_Error_DeInit;
    }

    return temp;
}



void SpiMaster_Released(SpiMaster *master, SpiSlaver *slaver)
{
    if (slaver == NULL)
    {
        SpiSlaver_Released(master->currentSlaver);
		SpiMaster_ReInit(master, master->spiHandle);
    }
    else
    {
        SpiSlaver_Released(master->currentSlaver);
    }
}

bool SpiMaster_WaitForTransmit(SpiMaster *master, uint8_t direction, uint32_t timeoutMs)
{
	
#ifdef USE_RTOS

	if (master->transmitStatus & (direction << 2))
	{
		return true;
	}
	if (xEventGroupWaitBits((master)->eventGroup, direction << 2, direction << 2, pdFALSE, pdMS_TO_TICKS(timeoutMs)) & direction << 2)
	{
		return true;
	}
	else
	{
		return false;
	}
#else
	while (master->transmitStatus & direction != 0 && timeoutMs != 0)
	{
		HAL_Delay(1);
		-- timeoutMs;
	}
	if (timeoutMs == 0)
	{
		return false;
	}
	else
	{
		return true;
	};
#endif
}

//#ifdef USE_RTOS
//bool SpiMaster_Lock(SpiMaster *master, uint32_t ms)
//{ 	

//	for (; ms > 0 && (((uint16_t)(xEventGroupGetBits(master->eventGroup)) & (uint16_t)(TransmitFree | ReceiveFree)) == 0); --ms)
//	{
//	 
//	}
//	if (ms > 0)
//	{
//		xEventGroupClearBits((master)->eventGroup, TransmitFree | ReceiveFree);
//		return true;
//	}
//		
//	return false;
//}

//#endif

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



bool SpiSlaver_Selected(SpiSlaver *slaver, uint32_t ms)
{
	if (slaver->master->status != DriverStatus_Run)
	{
		return false;
	}
	
#ifdef USE_RTOS
	//bool y = xEventGroupWaitBits(slaver->master->eventGroup, TransmitFree | ReceiveFree, TransmitFree | ReceiveFree, pdFALSE, pdMS_TO_TICKS(ms)) & (TransmitFree | ReceiveFree);
	
	if (!SpiMaster_Lock(slaver->master, TransmitFree | ReceiveFree, ms))
	{
		return false;
	}
#else
	if (slaver->master->currentSlaver != NULL)
	{
		return false;
	}
#endif
	
	slaver->master->currentSlaver = slaver;
	GpioPin_OutputHight(&(slaver->csnPin), false);
	return true;
}

bool SpiSlaver_Released(SpiSlaver *slaver)
{
	
	if (slaver != slaver->master->currentSlaver)
	{
		return false;
	}
	GpioPin_OutputHight(&(slaver->csnPin), true);
	slaver->master->currentSlaver = NULL;
	
#ifdef USE_RTOS
	slaver->master->currentTaskHandle = NULL;
	SpiMaster_Unlock(slaver->master);
#endif
	
	return true;
}

void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi)
{
	uint32_t i;
	BaseType_t temp = pdFALSE;
	for (i = 0; i < SpiCounter; --i)
	{
		if (spiMasterList[i] != 0 && spiMasterList[i]->spiHandle == hspi)
		{
			if (xEventGroupSetBitsFromISR(spiMasterList[i]->eventGroup, TransmitCompleted, &temp) != pdPASS)
			{
				__breakpoint(0);
			}
		}
	}
	portYIELD_FROM_ISR(temp);
}

void HAL_SPI_RxCpltCallback(SPI_HandleTypeDef *hspi)
{
	uint32_t i;
	BaseType_t temp = pdFALSE;
	for (i = 0; i < SpiCounter; --i)
	{
		if (spiMasterList[i] != 0 && spiMasterList[i]->spiHandle == hspi)
		{
			if (xEventGroupSetBitsFromISR(spiMasterList[i]->eventGroup, ReceiveCompleted, &temp) != pdPASS)
			{
				__breakpoint(0);
			}
			
		}
	}
	portYIELD_FROM_ISR(temp);
}



