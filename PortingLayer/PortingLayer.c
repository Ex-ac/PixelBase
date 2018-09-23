#include "PortingLayer.h"

#ifdef STM32F427xx

void createSpiMaster()
{
	SpiMaster *temp;
	temp = (SpiMaster *)(pvPortMalloc(sizeof(SpiMaster)));
	SpiMaster_Init(temp, &hspi5, 0);

	temp = (SpiMaster *)(pvPortMalloc(sizeof(SpiMaster)));
	SpiMaster_Init(temp, &hspi1, 1);

	temp = (SpiMaster *)(pvPortMalloc(sizeof(SpiMaster)));
	SpiMaster_Init(temp, &hspi2, 2);

	temp = (SpiMaster *)(pvPortMalloc(sizeof(SpiMaster)));
	SpiMaster_Init(temp, &hspi3, 3);
}

#endif

#ifdef STM32F103xE
void createSpiMaster()
{
	SpiMaster *temp;

	temp = (SpiMaster *)(pvPortMalloc(sizeof(SpiMaster)));
	SpiMaster_Init(temp, &hspi1, 0);

	temp = (SpiMaster *)(pvPortMalloc(sizeof(SpiMaster)));
	SpiMaster_Init(temp, &hspi2, 1);

	temp = (SpiMaster *)(pvPortMalloc(sizeof(SpiMaster)));
	SpiMaster_Init(temp, &hspi3, 2);
}

#endif

#ifdef TEST

void createPixelBase()
{
	PixelBase *pixelBase;
	pixelBase = (PixelBase *)(pvPortMalloc(sizeof(PixelBase)));
	PixelBase_Init(pixelBase, 0, spiMasterList[0], GPIOA, GPIO_PIN_4, GPIOC, GPIO_PIN_4, uartDriverList[0]);
}

#else

#ifdef STM32F427xx
void createPixelBase()
{

	SendToPCHandle *sendToPChandle = uartDriverList[1];
	PixelBase *pixelBase;

	//spi5 0

	pixelBase = (PixelBase *)(pvPortMalloc(sizeof(PixelBase)));
	PixelBase_Init(pixelBase, 0, spiMasterList[0], GPIOE, GPIO_PIN_2, GPIOE, GPIO_PIN_3, sendToPChandle);

	pixelBase = (PixelBase *)(pvPortMalloc(sizeof(PixelBase)));
	PixelBase_Init(pixelBase, 1, spiMasterList[0], GPIOF, GPIO_PIN_0, GPIOF, GPIO_PIN_1, sendToPChandle);

	pixelBase = (PixelBase *)(pvPortMalloc(sizeof(PixelBase)));
	PixelBase_Init(pixelBase, 2, spiMasterList[0], GPIOF, GPIO_PIN_4, GPIOF, GPIO_PIN_5, sendToPChandle);

	pixelBase = (PixelBase *)(pvPortMalloc(sizeof(PixelBase)));
	PixelBase_Init(pixelBase, 3, spiMasterList[0], GPIOC, GPIO_PIN_0, GPIOC, GPIO_PIN_1, sendToPChandle);

	pixelBase = (PixelBase *)(pvPortMalloc(sizeof(PixelBase)));
	PixelBase_Init(pixelBase, 4, spiMasterList[0], GPIOA, GPIO_PIN_1, GPIOA, GPIO_PIN_2, sendToPChandle);

	pixelBase = (PixelBase *)(pvPortMalloc(sizeof(PixelBase)));
	PixelBase_Init(pixelBase, 5, spiMasterList[0], GPIOE, GPIO_PIN_5, GPIOE, GPIO_PIN_4, sendToPChandle);

	pixelBase = (PixelBase *)(pvPortMalloc(sizeof(PixelBase)));
	PixelBase_Init(pixelBase, 6, spiMasterList[0], GPIOF, GPIO_PIN_3, GPIOF, GPIO_PIN_2, sendToPChandle);

	pixelBase = (PixelBase *)(pvPortMalloc(sizeof(PixelBase)));
	PixelBase_Init(pixelBase, 7, spiMasterList[0], GPIOF, GPIO_PIN_10, GPIOF, GPIO_PIN_6, sendToPChandle);

	pixelBase = (PixelBase *)(pvPortMalloc(sizeof(PixelBase)));
	PixelBase_Init(pixelBase, 8, spiMasterList[0], GPIOC, GPIO_PIN_3, GPIOC, GPIO_PIN_2, sendToPChandle);

	//spi1 1
	pixelBase = (PixelBase *)(pvPortMalloc(sizeof(PixelBase)));
	PixelBase_Init(pixelBase, 9, spiMasterList[1], GPIOA, GPIO_PIN_3, GPIOA, GPIO_PIN_4, sendToPChandle);

	pixelBase = (PixelBase *)(pvPortMalloc(sizeof(PixelBase)));
	PixelBase_Init(pixelBase, 10, spiMasterList[1], GPIOF, GPIO_PIN_11, GPIOF, GPIO_PIN_12, sendToPChandle);

	pixelBase = (PixelBase *)(pvPortMalloc(sizeof(PixelBase)));
	PixelBase_Init(pixelBase, 11, spiMasterList[1], GPIOF, GPIO_PIN_15, GPIOG, GPIO_PIN_0, sendToPChandle);

	pixelBase = (PixelBase *)(pvPortMalloc(sizeof(PixelBase)));
	PixelBase_Init(pixelBase, 12, spiMasterList[1], GPIOE, GPIO_PIN_10, GPIOE, GPIO_PIN_11, sendToPChandle);

	pixelBase = (PixelBase *)(pvPortMalloc(sizeof(PixelBase)));
	PixelBase_Init(pixelBase, 13, spiMasterList[1], GPIOC, GPIO_PIN_4, GPIOC, GPIO_PIN_5, sendToPChandle);

	pixelBase = (PixelBase *)(pvPortMalloc(sizeof(PixelBase)));
	PixelBase_Init(pixelBase, 14, spiMasterList[1], GPIOF, GPIO_PIN_14, GPIOF, GPIO_PIN_13, sendToPChandle);

	pixelBase = (PixelBase *)(pvPortMalloc(sizeof(PixelBase)));
	PixelBase_Init(pixelBase, 15, spiMasterList[1], GPIOE, GPIO_PIN_9, GPIOE, GPIO_PIN_8, sendToPChandle);

	pixelBase = (PixelBase *)(pvPortMalloc(sizeof(PixelBase)));
	PixelBase_Init(pixelBase, 16, spiMasterList[1], GPIOE, GPIO_PIN_12, GPIOE, GPIO_PIN_13, sendToPChandle);

	pixelBase = (PixelBase *)(pvPortMalloc(sizeof(PixelBase)));
	PixelBase_Init(pixelBase, 17, spiMasterList[1], GPIOB, GPIO_PIN_11, GPIOB, GPIO_PIN_10, sendToPChandle);

	//spi2 2
	pixelBase = (PixelBase *)(pvPortMalloc(sizeof(PixelBase)));
	PixelBase_Init(pixelBase, 18, spiMasterList[2], GPIOD, GPIO_PIN_10, GPIOD, GPIO_PIN_11, sendToPChandle);

	pixelBase = (PixelBase *)(pvPortMalloc(sizeof(PixelBase)));
	PixelBase_Init(pixelBase, 19, spiMasterList[2], GPIOD, GPIO_PIN_14, GPIOD, GPIO_PIN_15, sendToPChandle);

	pixelBase = (PixelBase *)(pvPortMalloc(sizeof(PixelBase)));
	PixelBase_Init(pixelBase, 20, spiMasterList[2], GPIOG, GPIO_PIN_4, GPIOG, GPIO_PIN_5, sendToPChandle);

	pixelBase = (PixelBase *)(pvPortMalloc(sizeof(PixelBase)));
	PixelBase_Init(pixelBase, 21, spiMasterList[2], GPIOC, GPIO_PIN_6, GPIOC, GPIO_PIN_7, sendToPChandle);

	pixelBase = (PixelBase *)(pvPortMalloc(sizeof(PixelBase)));
	PixelBase_Init(pixelBase, 22, spiMasterList[2], GPIOD, GPIO_PIN_13, GPIOD, GPIO_PIN_12, sendToPChandle);

	pixelBase = (PixelBase *)(pvPortMalloc(sizeof(PixelBase)));
	PixelBase_Init(pixelBase, 23, spiMasterList[2], GPIOG, GPIO_PIN_3, GPIOG, GPIO_PIN_2, sendToPChandle);

	pixelBase = (PixelBase *)(pvPortMalloc(sizeof(PixelBase)));
	PixelBase_Init(pixelBase, 24, spiMasterList[2], GPIOG, GPIO_PIN_7, GPIOG, GPIO_PIN_6, sendToPChandle);

	pixelBase = (PixelBase *)(pvPortMalloc(sizeof(PixelBase)));
	PixelBase_Init(pixelBase, 25, spiMasterList[2], GPIOA, GPIO_PIN_12, GPIOA, GPIO_PIN_11, sendToPChandle);

	//spi3 3
	pixelBase = (PixelBase *)(pvPortMalloc(sizeof(PixelBase)));
	PixelBase_Init(pixelBase, 26, spiMasterList[3], GPIOD, GPIO_PIN_0, GPIOD, GPIO_PIN_1, sendToPChandle);

	pixelBase = (PixelBase *)(pvPortMalloc(sizeof(PixelBase)));
	PixelBase_Init(pixelBase, 27, spiMasterList[3], GPIOD, GPIO_PIN_6, GPIOD, GPIO_PIN_7, sendToPChandle);

	pixelBase = (PixelBase *)(pvPortMalloc(sizeof(PixelBase)));
	PixelBase_Init(pixelBase, 28, spiMasterList[3], GPIOG, GPIO_PIN_11, GPIOG, GPIO_PIN_12, sendToPChandle);

	pixelBase = (PixelBase *)(pvPortMalloc(sizeof(PixelBase)));
	PixelBase_Init(pixelBase, 29, spiMasterList[3], GPIOB, GPIO_PIN_6, GPIOB, GPIO_PIN_7, sendToPChandle);

	pixelBase = (PixelBase *)(pvPortMalloc(sizeof(PixelBase)));
	PixelBase_Init(pixelBase, 30, spiMasterList[3], GPIOD, GPIO_PIN_4, GPIOD, GPIO_PIN_3, sendToPChandle);

	pixelBase = (PixelBase *)(pvPortMalloc(sizeof(PixelBase)));
	PixelBase_Init(pixelBase, 31, spiMasterList[3], GPIOG, GPIO_PIN_10, GPIOG, GPIO_PIN_9, sendToPChandle);

	pixelBase = (PixelBase *)(pvPortMalloc(sizeof(PixelBase)));
	PixelBase_Init(pixelBase, 32, spiMasterList[3], GPIOG, GPIO_PIN_14, GPIOG, GPIO_PIN_13, sendToPChandle);

	pixelBase = (PixelBase *)(pvPortMalloc(sizeof(PixelBase)));
	PixelBase_Init(pixelBase, 33, spiMasterList[3], GPIOE, GPIO_PIN_1, GPIOE, GPIO_PIN_0, sendToPChandle);
}
#endif

#ifdef STM32F103xE
void createPixelBase()
{
	PixelBase *pixelBase;

	//spi1
	pixelBase = (PixelBase *)(pvPortMalloc(sizeof(PixelBase)));
	PixelBase_Init(pixelBase, 0, spiMasterList[0], GPIOC, GPIO_PIN_1, GPIOC, GPIO_PIN_0, uartDriverList[0]);

	pixelBase = (PixelBase *)(pvPortMalloc(sizeof(PixelBase)));
	PixelBase_Init(pixelBase, 1, spiMasterList[0], GPIOA, GPIO_PIN_0, GPIOA, GPIO_PIN_1, uartDriverList[0]);

	pixelBase = (PixelBase *)(pvPortMalloc(sizeof(PixelBase)));
	PixelBase_Init(pixelBase, 2, spiMasterList[0], GPIOE, GPIO_PIN_4, GPIOE, GPIO_PIN_2, uartDriverList[0]);

	pixelBase = (PixelBase *)(pvPortMalloc(sizeof(PixelBase)));
	PixelBase_Init(pixelBase, 4, spiMasterList[0], GPIOC, GPIO_PIN_5, GPIOC, GPIO_PIN_4, uartDriverList[0]);

	pixelBase = (PixelBase *)(pvPortMalloc(sizeof(PixelBase)));
	PixelBase_Init(pixelBase, 7, spiMasterList[0], GPIOB, GPIO_PIN_1, GPIOE, GPIO_PIN_7, uartDriverList[0]);

	//spi2
	pixelBase = (PixelBase *)(pvPortMalloc(sizeof(PixelBase)));
	PixelBase_Init(pixelBase, 6, spiMasterList[1], GPIOC, GPIO_PIN_7, GPIOC, GPIO_PIN_6, uartDriverList[0]);

	pixelBase = (PixelBase *)(pvPortMalloc(sizeof(PixelBase)));
	PixelBase_Init(pixelBase, 10, spiMasterList[1], GPIOE, GPIO_PIN_9, GPIOE, GPIO_PIN_10, uartDriverList[0]);

	pixelBase = (PixelBase *)(pvPortMalloc(sizeof(PixelBase)));
	PixelBase_Init(pixelBase, 11, spiMasterList[1], GPIOE, GPIO_PIN_12, GPIOE, GPIO_PIN_11, uartDriverList[0]);

	pixelBase = (PixelBase *)(pvPortMalloc(sizeof(PixelBase)));
	PixelBase_Init(pixelBase, 12, spiMasterList[1], GPIOD, GPIO_PIN_11, GPIOD, GPIO_PIN_12, uartDriverList[0]);

	pixelBase = (PixelBase *)(pvPortMalloc(sizeof(PixelBase)));
	PixelBase_Init(pixelBase, 13, spiMasterList[1], GPIOD, GPIO_PIN_14, GPIOD, GPIO_PIN_13, uartDriverList[0]);

	//spi3

	pixelBase = (PixelBase *)(pvPortMalloc(sizeof(PixelBase)));
	PixelBase_Init(pixelBase, 3, spiMasterList[2], GPIOD, GPIO_PIN_1, GPIOD, GPIO_PIN_3, uartDriverList[0]);

	pixelBase = (PixelBase *)(pvPortMalloc(sizeof(PixelBase)));
	PixelBase_Init(pixelBase, 5, spiMasterList[2], GPIOD, GPIO_PIN_6, GPIOD, GPIO_PIN_5, uartDriverList[0]);

	pixelBase = (PixelBase *)(pvPortMalloc(sizeof(PixelBase)));
	PixelBase_Init(pixelBase, 8, spiMasterList[2], GPIOB, GPIO_PIN_7, GPIOB, GPIO_PIN_8, uartDriverList[0]);

	pixelBase = (PixelBase *)(pvPortMalloc(sizeof(PixelBase)));
	PixelBase_Init(pixelBase, 9, spiMasterList[2], GPIOE, GPIO_PIN_0, GPIOB, GPIO_PIN_9, uartDriverList[0]);
}
#endif

#endif

#ifdef STM32F427xx
void createUartDriver()
{
	UartDriver *uartDriver = (UartDriver *)(pvPortMalloc(sizeof(UartDriver)));
	UartDriver_Init(uartDriver, &huart3, 1);

	uartDriver = (UartDriver *)(pvPortMalloc(sizeof(UartDriver)));
	UartDriver_Init(uartDriver, &huart1, 0);
}
#endif

#ifdef STM32F103xE
void createUartDriver()
{
	UartDriver *uartDriver = (UartDriver *)(pvPortMalloc(sizeof(UartDriver)));
	UartDriver_Init(uartDriver, &huart1, 0);
}
#endif

void Debug_Msg(const char *txt)
{
#ifdef USE_Debug
	bool ok;
	do
	{
		ok = true;
		if (UartDriver_PrepareForTransmit(uartDriverList[0], Transmit, 1))
		{
			if (UartDriver_Transmit(uartDriverList[0], txt, strlen(txt), 100 * HAL_TimeoutMs) != HAL_OK)
			{
				ok = false;
			}
			UartDriver_EndTransmit(uartDriverList[0], Transmit);
		}
		else
		{
			ok = false;
		}

		if (!ok)
		{
			delayMs(1);
		}
	} while (!ok);

#else
	__NOP;
#endif
}
