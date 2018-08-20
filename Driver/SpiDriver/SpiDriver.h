#ifndef SPIDRIVER_H_
#define SPIDRIVER_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>
#include <stdbool.h>



#include "../GpioPin/GpioPin.h"
#include "../../PortingLayer/GlobalDefine.h"


#ifndef SpiCounter
#define SpiCounter 3
#endif

#ifndef TimeoutMs
#define TimeoutMs 1
#endif 



#ifdef USE_RTOS

#include <FreeRTOS.h>
#include <task.h>
#include <event_groups.h>

#endif



struct TagSpiSlaver;
typedef struct TagSpiSlaver SpiSlaver;

struct TagSpiMaster;
typedef struct TagSpiMaster SpiMaster;

extern SpiMaster *spiMasterList[SpiCounter];

struct TagSpiMaster
{
	uint8_t transmitStatus;
	uint8_t status;
	
    SpiSlaver *currentSlaver;
    SPI_HandleTypeDef *handle;
	
#ifdef USE_RTOS	
	EventGroupHandle_t eventGroup;
	
#endif
	
};

struct TagSpiSlaver
{
    SpiMaster *master;
    GpioPin csnPin;
};



HAL_StatusTypeDef SpiMaster_Init(SpiMaster *driver, SPI_HandleTypeDef *handle, uint32_t index);

HAL_StatusTypeDef SpiMaster_ReInit(SpiMaster *driver, SPI_HandleTypeDef *handle, uint32_t forceTimeout);

HAL_StatusTypeDef SpiMaster_DeInit(SpiMaster *driver, uint32_t forceTimeout);

SpiSlaver *SpiMaster_CurrentSlaver(SpiMaster *driver);

HAL_StatusTypeDef SpiMaster_Transmit(SpiMaster *driver, const uint8_t *pData, uint16_t size, uint32_t ms);

HAL_StatusTypeDef SpiMaster_TransmitByDMA(SpiMaster *driver, const uint8_t *pData, uint16_t size);

HAL_StatusTypeDef SpiMaster_Receive(SpiMaster *driver, uint8_t *pData, uint16_t size, uint32_t ms);

HAL_StatusTypeDef SpiMaster_ReceiveByDMA(SpiMaster *driver, uint8_t *pData, uint16_t size);


bool SpiMaster_PrepareForTransmit(SpiMaster *driver, uint8_t direction, uint32_t ms);

bool SpiMaster_EndTransmit(SpiMaster *driver, uint8_t direction);


bool SpiMaster_WaitForTransmit(SpiMaster *driver, uint8_t direction, uint32_t ms);




void SpiSlaver_Init(SpiSlaver *slaver, SpiMaster *master, GPIO_TypeDef *port, uint32_t pin);

bool SpiSlaver_PrepareForTransmit(SpiSlaver *slaver, uint8_t direction, uint32_t ms);

bool SpiSlaver_EndTransmit(SpiSlaver *slaver, uint8_t direction);


bool SpiSlaver_WaitForTransmit(SpiSlaver *slaver, uint8_t direction, uint32_t ms);

HAL_StatusTypeDef SpiSlaver_Transmit(SpiSlaver *slaver, const uint8_t *pData, uint16_t size, uint32_t ms);

HAL_StatusTypeDef SpiSlaver_TransmitByDMA(SpiSlaver *slaver, const uint8_t *pData, uint16_t size);


HAL_StatusTypeDef SpiSlaver_Receive(SpiSlaver *slaver, uint8_t *pData, uint16_t size, uint32_t ms);

HAL_StatusTypeDef SpiSlaver_ReceiveByDMA(SpiSlaver *slaver, uint8_t *pData, uint16_t size);

#ifdef __cplusplus
}
#endif

#endif
