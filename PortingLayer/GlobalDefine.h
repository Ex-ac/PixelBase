#ifndef GLOBALDEFINE_H_
#define GLOBALDEFINE_H_

// stm32f407
#include <stm32f4xx_hal.h>
#include <stdint.h>
#include <stdbool.h>

#include <FreeRTOS.h>
#include <task.h>
#include <event_groups.h>

#define PixelBaseCounter 14
#define SpiCounter 3
#define UartCounter 2
#define TimeoutMs 1

#define HAL_TimeoutMs 1

#define TaskPoolCounter 3
#define TaskPoolStackDepth (2 * 1024 / 4)
#define TaskPoolPriority 2

#define MainTaskPriority 3
#define MainTaskStackDepth (1024 / 4)

#define TaskQueueLength (PixelBaseCounter * 2)


typedef enum TagTransmitDirection
{
	Transmit = 0x01 << 0,
	TransmitFree = 0x01 << 0,
	Receive = 0x01 << 1,
	ReceiveFree = 0x01 << 1,
	TransmitCompleted = 0x01 << 2,
	ReceiveCompleted = 0x01 << 3,
}TransmitDirection;

typedef enum TagDriverStatus
{
    DriverStatus_Run,
	
    DriverStatus_Init,
    DriverStatus_Error_Init,

	DriverStatus_Error_TransmitTimeout,
	
    DriverStatus_DeInit,
    DriverStatus_Error_DeInit,
}DriverStatus;


#endif
