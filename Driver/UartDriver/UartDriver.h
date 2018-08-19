#ifndef UARTDRIVER_H_
#define UARTDRIVER_H_

#include "../../PortingLayer/GlobalDefine.h"



typedef struct TagUartDriver
{

	uint8_t transmitStatus;
	uint8_t status;

	UART_HandleTypeDef *handle;


#ifdef USE_RTOS
	EventGroupHandle_t eventGroup;
#endif

} UartDriver;

extern UartDriver *uartDriverList[UartCounter];

HAL_StatusTypeDef UartDriver_Init(UartDriver *driver, UART_HandleTypeDef *handle, uint32_t index);
HAL_StatusTypeDef UartDriver_ReInit(UartDriver *driver, UART_HandleTypeDef *handle, uint32_t forceTimeout);
HAL_StatusTypeDef UartDriver_DeInit(UartDriver *driver, uint32_t forceTimeout);

HAL_StatusTypeDef UartDriver_Transmit(UartDriver *driver, const uint8_t *pData, uint16_t size, uint32_t ms);
HAL_StatusTypeDef UartDriver_TransmitByDMA(UartDriver *driver, const uint8_t *pData, uint16_t size);

HAL_StatusTypeDef UartDriver_Receive(UartDriver *driver, uint8_t *pData, uint16_t size, uint32_t ms);
HAL_StatusTypeDef UartDriver_ReceiveByDMA(UartDriver *driver, uint8_t *pData, uint16_t size);


bool UartDriver_PrepareForTransmit(UartDriver *driver, uint8_t direction, uint32_t ms);
bool UartDriver_EndTransmit(UartDriver *driver, uint8_t direction);

bool UartDriver_WaitForTransmit(UartDriver *driver, uint8_t direction, uint32_t ms);




#endif
