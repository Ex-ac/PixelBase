#include "PortingLayer.h"

#if SpiCounter == 3
void createSpiMaster()
{
	SpiMaster *temp = (SpiMaster *)(pvPortMalloc(sizeof(SpiMaster)));
	SpiMaster_Init(temp, &hspi1, 0);

	temp = (SpiMaster *)(pvPortMalloc(sizeof(SpiMaster)));
	SpiMaster_Init(temp, &hspi2, 1);

	temp = (SpiMaster *)(pvPortMalloc(sizeof(SpiMaster)));
	SpiMaster_Init(temp, &hspi3, 2);
}

#elif SpiCounter = 6

void createPixelBase()
{
	SpiMaster *temp = (SpiMaster *)(pvPortMalloc(sizeof(SpiMaster)));
	SpiMaster_Init(temp, &hspi5, 4);

	temp = (SpiMaster *)(pvPortMalloc(sizeof(SpiMaster)));
	SpiMaster_Init(temp, &hspi1, 0);

	temp = (SpiMaster *)(pvPortMalloc(sizeof(SpiMaster)));
	SpiMaster_Init(temp, &hspi4, 3);

	SpiMaster *temp = (SpiMaster *)(pvPortMalloc(sizeof(SpiMaster)));
	SpiMaster_Init(temp, &hspi2, 1);

	temp = (SpiMaster *)(pvPortMalloc(sizeof(SpiMaster)));
	SpiMaster_Init(temp, &hspi6, 5);

	temp = (SpiMaster *)(pvPortMalloc(sizeof(SpiMaster)));
	SpiMaster_Init(temp, &hspi3, 2);
}

#endif

#if PixelBaseCounter == 34
void createPixelBase()
{
	uint8_t i = 0;
	SendToPCHandle sendToPChandle = void *;
	PixelBase *pixelBase;

	//spi5 0
	pixelBase = (PixelBase *)(pvPortMalloc(sizeof(PixelBase)));
	PixelBase_Init(pixelBase, i++, spiMasterList[4], GPIOE, GPIO_PIN_3, GPIOE, GPIO_PIN_2, sendToPChandle);

	pixelBase = (PixelBase *)(pvPortMalloc(sizeof(PixelBase)));
	PixelBase_Init(pixelBase, i++, spiMasterList[4], GPIOE, GPIO_PIN_4, GPIOE, GPIO_PIN_5, sendToPChandle);

	pixelBase = (PixelBase *)(pvPortMalloc(sizeof(PixelBase)));
	PixelBase_Init(pixelBase, i++, spiMasterList[4], GPIOE, GPIO_PIN_6, GPIOC, GPIO_PIN_13, sendToPChandle);

	pixelBase = (PixelBase *)(pvPortMalloc(sizeof(PixelBase)));
	PixelBase_Init(pixelBase, i++, spiMasterList[4], GPIOF, GPIO_PIN_1, GPIOF, GPIO_PIN_0, sendToPChandle);

	pixelBase = (PixelBase *)(pvPortMalloc(sizeof(PixelBase)));
	PixelBase_Init(pixelBase, i++, spiMasterList[4], GPIOF, GPIO_PIN_3, GPIOF, GPIO_PIN_4, sendToPChandle);

	pixelBase = (PixelBase *)(pvPortMalloc(sizeof(PixelBase)));
	PixelBase_Init(pixelBase, i++, spiMasterList[4], GPIOF, GPIO_PIN_5, GPIOF, GPIO_PIN_4, sendToPChandle);

	//spi1 6
	pixelBase = (PixelBase *)(pvPortMalloc(sizeof(PixelBase)));
	PixelBase_Init(pixelBase, i++, spiMasterList[0], GPIOC, GPIO_PIN_1, GPIOC, GPIO_PIN_0, sendToPChandle);

	pixelBase = (PixelBase *)(pvPortMalloc(sizeof(PixelBase)));
	PixelBase_Init(pixelBase, i++, spiMasterList[0], GPIOC, GPIO_PIN_3, GPIOC, GPIO_PIN_2, sendToPChandle);

	pixelBase = (PixelBase *)(pvPortMalloc(sizeof(PixelBase)));
	PixelBase_Init(pixelBase, i++, spiMasterList[0], GPIOA, GPIO_PIN_1, GPIOA, GPIO_PIN_2, sendToPChandle);

	pixelBase = (PixelBase *)(pvPortMalloc(sizeof(PixelBase)));
	PixelBase_Init(pixelBase, i++, spiMasterList[0], GPIOA, GPIO_PIN_4, GPIOA, GPIO_PIN_3, sendToPChandle);

	pixelBase = (PixelBase *)(pvPortMalloc(sizeof(PixelBase)));
	PixelBase_Init(pixelBase, i++, spiMasterList[0], GPIOC, GPIO_PIN_4, GPIOC, GPIO_PIN_5, sendToPChandle);

	pixelBase = (PixelBase *)(pvPortMalloc(sizeof(PixelBase)));
	PixelBase_Init(pixelBase, i++, spiMasterList[0], GPIOB, GPIO_PIN_0, GPIOB, GPIO_PIN_1, sendToPChandle);

	//spi4 12
	pixelBase = (PixelBase *)(pvPortMalloc(sizeof(PixelBase)));
	PixelBase_Init(pixelBase, i++, spiMasterList[3], GPIOF, GPIO_PIN_12, GPIOF, GPIO_PIN_11, sendToPChandle);

	pixelBase = (PixelBase *)(pvPortMalloc(sizeof(PixelBase)));
	PixelBase_Init(pixelBase, i++, spiMasterList[3], GPIOF, GPIO_PIN_14, GPIOF, GPIO_PIN_13, sendToPChandle);

	pixelBase = (PixelBase *)(pvPortMalloc(sizeof(PixelBase)));
	PixelBase_Init(pixelBase, i++, spiMasterList[3], GPIOG, GPIO_PIN_0, GPIOF, GPIO_PIN_15, sendToPChandle);

	pixelBase = (PixelBase *)(pvPortMalloc(sizeof(PixelBase)));
	PixelBase_Init(pixelBase, i++, spiMasterList[3], GPIOG, GPIO_PIN_1, GPIOE, GPIO_PIN_7, sendToPChandle);

	pixelBase = (PixelBase *)(pvPortMalloc(sizeof(PixelBase)));
	PixelBase_Init(pixelBase, i++, spiMasterList[3], GPIOE, GPIO_PIN_8, GPIOE, GPIO_PIN_9, sendToPChandle);

	pixelBase = (PixelBase *)(pvPortMalloc(sizeof(PixelBase)));
	PixelBase_Init(pixelBase, i++, spiMasterList[3], GPIOE, GPIO_PIN_10, GPIOE, GPIO_PIN_11, sendToPChandle);

	//spi2 18
	pixelBase = (PixelBase *)(pvPortMalloc(sizeof(PixelBase)));
	PixelBase_Init(pixelBase, i++, spiMasterList[1], GPIOD, GPIO_PIN_10, GPIOD, GPIO_PIN_11, sendToPChandle);

	pixelBase = (PixelBase *)(pvPortMalloc(sizeof(PixelBase)));
	PixelBase_Init(pixelBase, i++, spiMasterList[1], GPIOD, GPIO_PIN_12, GPIOF, GPIO_PIN_13, sendToPChandle);

	pixelBase = (PixelBase *)(pvPortMalloc(sizeof(PixelBase)));
	PixelBase_Init(pixelBase, i++, spiMasterList[1], GPIOD, GPIO_PIN_15, GPIOD, GPIO_PIN_14, sendToPChandle);

	pixelBase = (PixelBase *)(pvPortMalloc(sizeof(PixelBase)));
	PixelBase_Init(pixelBase, i++, spiMasterList[1], GPIOG, GPIO_PIN_3, GPIOG, GPIO_PIN_2, sendToPChandle);

	pixelBase = (PixelBase *)(pvPortMalloc(sizeof(PixelBase)));
	PixelBase_Init(pixelBase, i++, spiMasterList[1], GPIOG, GPIO_PIN_5, GPIOG, GPIO_PIN_4, sendToPChandle);

	//spi6 23
	pixelBase = (PixelBase *)(pvPortMalloc(sizeof(PixelBase)));
	PixelBase_Init(pixelBase, i++, spiMasterList[5], GPIOG, GPIO_PIN_7, GPIOG, GPIO_PIN_6, sendToPChandle);

	pixelBase = (PixelBase *)(pvPortMalloc(sizeof(PixelBase)));
	PixelBase_Init(pixelBase, i++, spiMasterList[5], GPIOC, GPIO_PIN_6, GPIOC, GPIO_PIN_7, sendToPChandle);

	pixelBase = (PixelBase *)(pvPortMalloc(sizeof(PixelBase)));
	PixelBase_Init(pixelBase, i++, spiMasterList[5], GPIOA, GPIO_PIN_11, GPIOA, GPIO_PIN_12, sendToPChandle);

	pixelBase = (PixelBase *)(pvPortMalloc(sizeof(PixelBase)));
	PixelBase_Init(pixelBase, i++, spiMasterList[5], GPIOA, GPIO_PIN_15, GPIOD, GPIO_PIN_0, sendToPChandle);

	pixelBase = (PixelBase *)(pvPortMalloc(sizeof(PixelBase)));
	PixelBase_Init(pixelBase, i++, spiMasterList[5], GPIOA, GPIO_PIN_1, GPIOD, GPIO_PIN_3, sendToPChandle);

	pixelBase = (PixelBase *)(pvPortMalloc(sizeof(PixelBase)));
	PixelBase_Init(pixelBase, i++, spiMasterList[5], GPIOD, GPIO_PIN_4, GPIOD, GPIO_PIN_5, sendToPChandle);

	//spi3 29
	pixelBase = (PixelBase *)(pvPortMalloc(sizeof(PixelBase)));
	PixelBase_Init(pixelBase, i++, spiMasterList[2], GPIOD, GPIO_PIN_7, GPIOD, GPIO_PIN_7, sendToPChandle);

	pixelBase = (PixelBase *)(pvPortMalloc(sizeof(PixelBase)));
	PixelBase_Init(pixelBase, i++, spiMasterList[2], GPIOG, GPIO_PIN_10, GPIOG, GPIO_PIN_9, sendToPChandle);

	pixelBase = (PixelBase *)(pvPortMalloc(sizeof(PixelBase)));
	PixelBase_Init(pixelBase, i++, spiMasterList[2], GPIOG, GPIO_PIN_15, GPIOG, GPIO_PIN_11, sendToPChandle);

	pixelBase = (PixelBase *)(pvPortMalloc(sizeof(PixelBase)));
	PixelBase_Init(pixelBase, i++, spiMasterList[2], GPIOB, GPIO_PIN_7, GPIOB, GPIO_PIN_6, sendToPChandle);

	pixelBase = (PixelBase *)(pvPortMalloc(sizeof(PixelBase)));
	PixelBase_Init(pixelBase, i++, spiMasterList[2], GPIOE, GPIO_PIN_0, GPIOE, GPIO_PIN_1, sendToPChandle);
}

#elif PixelBaseCounter == 14

void createPixelBase()
{
	SendToPCHandle *sendToPCHandle = uartDriverList[0];
	PixelBase *pixelBase;

	//spi1
	pixelBase = (PixelBase *)(pvPortMalloc(sizeof(PixelBase)));
	PixelBase_Init(pixelBase, 0, spiMasterList[0], GPIOC, GPIO_PIN_1, GPIOC, GPIO_PIN_0, uartDriverList[0]);
	
	pixelBase = (PixelBase *)(pvPortMalloc(sizeof(PixelBase)));
	PixelBase_Init(pixelBase, 1, spiMasterList[0], GPIOA, GPIO_PIN_0, GPIOA, GPIO_PIN_1, uartDriverList[0]);
	
	
//	pixelBase = (PixelBase *)(pvPortMalloc(sizeof(pixelBase)));
//	PixelBase_Init(pixelBase, 0, spiMasterList[0], GPIOC, GPIO_PIN_1, GPIOC, GPIO_PIN_0, sendToPCHandle);

//	pixelBase = (PixelBase *)(pvPortMalloc(sizeof(pixelBase)));
//	PixelBase_Init(pixelBase, 1, spiMasterList[0], GPIOA, GPIO_PIN_0, GPIOA, GPIO_PIN_1, sendToPCHandle);

//	pixelBase = (PixelBase *)(pvPortMalloc(sizeof(pixelBase)));
//	PixelBase_Init(pixelBase, 2, spiMasterList[0], GPIOE, GPIO_PIN_4, GPIOE, GPIO_PIN_2, sendToPCHandle);

//	pixelBase = (PixelBase *)(pvPortMalloc(sizeof(pixelBase)));
//	PixelBase_Init(pixelBase, 4, spiMasterList[0], GPIOC, GPIO_PIN_5, GPIOC, GPIO_PIN_4, sendToPCHandle);

//	pixelBase = (PixelBase *)(pvPortMalloc(sizeof(pixelBase)));
//	PixelBase_Init(pixelBase, 7, spiMasterList[0], GPIOB, GPIO_PIN_1, GPIOE, GPIO_PIN_7, sendToPCHandle);

//	//spi2
//	pixelBase = (PixelBase *)(pvPortMalloc(sizeof(pixelBase)));
//	PixelBase_Init(pixelBase, 10, spiMasterList[1], GPIOE, GPIO_PIN_9, GPIOE, GPIO_PIN_10, sendToPCHandle);

//	pixelBase = (PixelBase *)(pvPortMalloc(sizeof(pixelBase)));
//	PixelBase_Init(pixelBase, 11, spiMasterList[1], GPIOE, GPIO_PIN_12, GPIOE, GPIO_PIN_11, sendToPCHandle);

//	pixelBase = (PixelBase *)(pvPortMalloc(sizeof(pixelBase)));
//	PixelBase_Init(pixelBase, 12, spiMasterList[1], GPIOD, GPIO_PIN_11, GPIOD, GPIO_PIN_12, sendToPCHandle);

//	pixelBase = (PixelBase *)(pvPortMalloc(sizeof(pixelBase)));
//	PixelBase_Init(pixelBase, 13, spiMasterList[1], GPIOD, GPIO_PIN_14, GPIOD, GPIO_PIN_13, sendToPCHandle);

//	pixelBase = (PixelBase *)(pvPortMalloc(sizeof(pixelBase)));
//	PixelBase_Init(pixelBase, 6, spiMasterList[1], GPIOC, GPIO_PIN_7, GPIOC, GPIO_PIN_6, sendToPCHandle);

//	//spi3
//	pixelBase = (PixelBase *)(pvPortMalloc(sizeof(pixelBase)));
//	PixelBase_Init(pixelBase, 3, spiMasterList[2], GPIOD, GPIO_PIN_1, GPIOD, GPIO_PIN_3, sendToPCHandle);

//	pixelBase = (PixelBase *)(pvPortMalloc(sizeof(pixelBase)));
//	PixelBase_Init(pixelBase, 5, spiMasterList[2], GPIOD, GPIO_PIN_6, GPIOD, GPIO_PIN_5, sendToPCHandle);

//	pixelBase = (PixelBase *)(pvPortMalloc(sizeof(pixelBase)));
//	PixelBase_Init(pixelBase, 8, spiMasterList[2], GPIOB, GPIO_PIN_7, GPIOB, GPIO_PIN_8, sendToPCHandle);

//	pixelBase = (PixelBase *)(pvPortMalloc(sizeof(pixelBase)));
//	PixelBase_Init(pixelBase, 9, spiMasterList[2], GPIOE, GPIO_PIN_0, GPIOE, GPIO_PIN_9, sendToPCHandle);
}

#elif PixelBaseCounter == 10

void createPixelBase()
{
	uint8_t i = 0;
	SendToPCHandle sendToPChandle = void *;
	PixelBase *pixelBate;
}

#elif PixelBaseCounter == 3
void createPixelBase()
{
	PixelBase *pixelBase = (PixelBase *)(pvPortMalloc(sizeof(PixelBase)));
	PixelBase_Init(pixelBase, 0, spiMasterList[0], GPIOA, GPIO_PIN_4, GPIOC, GPIO_PIN_4, uartDriverList[0]);


	pixelBase = (PixelBase *)(pvPortMalloc(sizeof(PixelBase)));
	PixelBase_Init(pixelBase, 1, spiMasterList[1], GPIOG, GPIO_PIN_1, GPIOB, GPIO_PIN_12, uartDriverList[0]);

	
//	pixelBase = (PixelBase *)(pvPortMalloc(sizeof(PixelBase)));
//	PixelBase_Init(pixelBase, 2, spiMasterList[2], GPIOG, GPIO_PIN_15, GPIOB, GPIO_PIN_7, uartDriverList[0]);

}
#endif


#if UartCounter == 2
void createUartDriver()
{
	UartDriver *uartDriver = (UartDriver *)(pvPortMalloc(sizeof(UartDriver)));
	UartDriver_Init(uartDriver, &huart1, 0);
}
#endif
