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
    SPI_HandleTypeDef *spiHandle;
	
#ifdef USE_RTOS	
	EventGroupHandle_t eventGroup;
	
#endif
	
};

struct TagSpiSlaver
{
    SpiMaster *master;
    GpioPin csnPin;
};



//SpiMaster Function
HAL_StatusTypeDef SpiMaster_Init(SpiMaster *master, SPI_HandleTypeDef *spiHandle, uint32_t index);
HAL_StatusTypeDef SpiMaster_ReInit(SpiMaster *master, SPI_HandleTypeDef *spiHandle);
HAL_StatusTypeDef SpiMaster_DeInit(SpiMaster *master);


SpiSlaver *SpiMaster_CurrentSlaver(SpiMaster *master);

void SpiMaster_Released(SpiMaster *master, SpiSlaver *slaver);

#define SpiMaster_CurrentSlaver(master) ((master)->currentSlaver)

#define SpiMaster_Transmit(master, pData, size, timeout) ((master)->transmitStatus |= TransmitCompleted, HAL_SPI_Transmit((master)->spiHandle, (pData), (size), (timeout)))
#define SpiMaster_TransmitByDMA(master, pData, size) ((master)->transmitStatus &= (~TransmitCompleted), HAL_SPI_Transmit_DMA((master)->spiHandle, (pData), (size)))
#define SpiMaster_Receive(master, pData, size, timeout) ((master)->transmitStatus |= ReceiveCompleted, HAL_SPI_Receive((master)->spiHandle, (pData), (size), (timeout)))
#define SpiMaster_ReceiveByDMA(master, pData, size) ((master)->transmitStatus &= (~ReceiveCompleted), HAL_SPI_Receive_DMA((master)->spiHandle, (pData), (size)))

#ifdef USE_RTOS

bool SpiMaster_Lock(SpiMaster *master, uint32_t ms);

#define SpiMaster_Unlock(master) (xEventGroupSetBits((master)->eventGroup, TransmitFree | ReceiveFree))

#define SpiMaster_Lock(master, direction, timeoutMs) \
((xEventGroupWaitBits((master)->eventGroup, direction, direction, pdFALSE, pdMS_TO_TICKS(ms)) & direction) > 0)

#endif

bool SpiMaster_WaitForTransmit(SpiMaster *master, uint8_t direction, uint32_t timeoutMs);
bool SpiMaster_WaitForReceive(SpiMaster *master, uint32_t timeoutMs);

//SpiSlaver Function
void SpiSlaver_Init(SpiSlaver *slaver, SpiMaster *master, GPIO_TypeDef *port, uint16_t pin);

bool SpiSlaver_Selected(SpiSlaver *slaver, uint32_t ms);
bool SpiSlaver_Released(SpiSlaver *slaver);

#define SpiSlaver_PrepareForTransmit(slaver, direction, ms) (SpiSlaver_Selected((slaver), (ms))) 
#define SpiSlaver_EndTransmit(slaver, direction) (SpiSlaver_Released((slaver)))


#define SpiSlaver_Transmit(slaver, pData, size, timeout) (SpiMaster_Transmit((slaver)->master, (pData), (size), (timeout)))
#define SpiSlaver_TransmitByDMA(slaver, pData, size) (SpiMaster_TransmitByDMA((slaver)->master, (pData), (size)))
#define SpiSlaver_Receive(slaver, pData, size, timeout) (SpiMaster_Receive((slaver)->master, (pData), (size), (timeout)))
#define SpiSlaver_ReceiveByDMA(slaver, pData, size) (SpiMaster_ReceiveByDMA((slaver)->master, (pData), (size)))

#define SpiSlaver_WaitForTransmit(slaver, direction, timeoutMs) (SpiMaster_WaitForTransmit((slaver)->master, (direction), (timeoutMs)))


#ifdef __cplusplus
}
#endif

#endif
