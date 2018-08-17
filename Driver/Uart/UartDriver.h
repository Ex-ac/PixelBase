#ifndef UARTDRIVER_H_
#define UARTDRIVER_H_

#include "../../PortingLayer/GlobalDefine.h"



typedef struct TagUartDriver
{

	uint8_t transmitStatus;
	uint8_t status;

	UART_HandleTypeDef *spiHandle;

#ifndef USE_RTOS
	EventGroupHandle_t eventGroup;
#endif

} UartDriver;

extern UartDriver *UartDriverList[UartCounter];

HAL_StatusTypeDef UartDriver_Init(UartDriver *driver, UART_HandleTypeDef *handle, index);
HAL_StatusTypeDef UartDriver_ReInit(UartDriver *driver, uint32_t forceTimeout);
HAL_StatusTypeDef UartDriver_DeInitUartDriver *driver, uint32_t forceTimeout);

HAL_StatusTypeDef UartDriver_Transmit(UartDriver *driver, const uint8_t *pData, uint16_t size, uint32_t ms);
HAL_StatusTypeDef UartDriver_TransmitByDMA(UartDriver *driver, const uint8_t *pData, uint16_t size, uint32_t ms);

HAL_StatusTypeDef UartDriver_Receive(UartDriver *driver, uint8_t *pData, uint16_t size, uint32_t ms);
HAL_StatusTypeDef UartDriver_ReceiveByDMA(UartDriver *driver, uint8_t *pData, uint16_t size, uint32_t ms);


bool UartDriver_PrepareForTransmit(UartDriver *driver, uint8_t direction, uint32_t ms);
bool UartDriver_EndTransmit(UartDriver *driver, uint8_t direction);

bool UartDriver_WaitForTransmit(UartDriver *driver, uint8_t direction, uint32_t ms);




#endif
