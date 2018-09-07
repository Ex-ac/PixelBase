#ifndef GLOBALDEFINE_H_
#define GLOBALDEFINE_H_

// stm32f427

//#define TEST
//#define USE_Debug

#ifdef STM32F427xx
#include <stm32f4xx_hal.h>

#define SpiCounter 4
#define UartCounter 2
#define PixelBaseCounter 34
#endif

#ifdef STM32F103xE
#include <stm32f1xx_hal.h>
#define PixelBaseCounter 14
#define SpiCounter 3
#define UartCounter 2
#endif



#include <stdint.h>
#include <stdbool.h>

#include <FreeRTOS.h>
#include <task.h>
#include <event_groups.h>

//test

#define USE_FatfsThread


#define TimeoutMs 10

#define HAL_TimeoutMs 10

#define TaskPoolCounter SpiCounter
#define TaskPoolStackDepth (2 * 1024 / 4)
#define TaskPoolPriority 1

#define MainTaskPriority 2
#define MainTaskStackDepth (1024 / 4)

#define TaskQueueLength 20

#define TicksTimeout 10

//#define PixelBase_SaveToSD

#define PixelBaseFileFragmentSize_KB 32


#ifdef USE_RTOS
#define delayMs(ms) vTaskDelay(pdMS_TO_TICKS(ms))
#else
#define delayMs(ms) HAL_Delay(ms)
#endif


#ifdef USE_Debug
#define DebugBreak() __breakpoint(0)
#else
#define DebugBreak() __NOP
#endif

typedef enum TagTransmitDirection
{
	Transmit = 0x01 << 0,
	TransmitFree = 0x01 << 0,
	Receive = 0x01 << 1,
	ReceiveFree = 0x01 << 1,
	TransmitCompleted = 0x01 << 2,
	ReceiveCompleted = 0x01 << 3,
} TransmitDirection;

typedef enum TagDriverStatus
{
	DriverStatus_Run,

	DriverStatus_Init,
	DriverStatus_Error_Init,

	DriverStatus_Error_TransmitTimeout,

	DriverStatus_DeInit,
	DriverStatus_Error_DeInit,
} DriverStatus;

#endif
