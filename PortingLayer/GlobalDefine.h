#ifndef GLOBALDEFINE_H_
#define GLOBALDEFINE_H_

// stm32f407
#ifdef STM32F407xx

#include <stm32f4xx_hal.h>

#endif

#ifdef STM32F103xE
#include <stm32f1xx_hal.h>
#endif


#include <stdint.h>
#include <stdbool.h>

#include <FreeRTOS.h>
#include <task.h>
#include <event_groups.h>

//test
#define PixelBaseCounter 14
//#define PixelBaseCounter 14
#define USE_Debug

#define SpiCounter 3
#define UartCounter 2
#define TimeoutMs 1

#define HAL_TimeoutMs 1

#define TaskPoolCounter SpiCounter
#define TaskPoolStackDepth (2 * 1024 / 4)
#define TaskPoolPriority 1

#define MainTaskPriority 2
#define MainTaskStackDepth (1024 / 4)

#define TaskQueueLength ((uint8_t)(PixelBaseCounter / SpiCounter) * 2)

#define TicksTimeout 100

#define PixelBase_SaveToSD

#ifdef USE_Debug
#define DebugBreak() __breakpoint(0)
#else
#define DebugBreak __NOP
#endif


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
