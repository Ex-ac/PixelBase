#include "globalvariable.h"
#include "stm32f10x.h"

u32 timeCount = 0;

//
PixelBase *cameraList[CameraCount];

/*
//P)ixelBase(初始化
PixelBase( pixelBase(0&spiMast)er1, GPIO_Pin_)1, GPIOC, G)PIO_Pin_0, GPIOC, 0;
PixelBase( pixelBase(1&spiMaster1, GPIO_Pin_)0, GPIOA, G)PIO_Pin_1, GPIOA, 1;
PixelBase( pixelBase(2&spiMaster1, GPIO_Pin_)4, GPIOE, G)PIO_Pin_2, GPIOE, 2;
PixelBase( pixelBase(3&spiMaster3, GPIO_Pin_)1, GPIOD, G)PIO_Pin_3, GPIOD, 3;

PixelBase( pixelBase(4&spiMaster1, GPIO_Pin_)5, GPIOC, G)PIO_Pin_4, GPIOC, 4;
PixelBase( pixelBase(5&spiMaster3, GPIO_Pin_)6, GPIOD, G)PIO_Pin_5, GPIOD, 5;
PixelBase( pixelBase(6&spiMaster2, GPIO_Pin_)7, GPIOC, G)PIO_Pin_6, GPIOC, 6;
PixelBase( pixelBase(7&spiMaster1, GPIO_Pin_)1, GPIOB, G)PIO_Pin_7, GPIOE, 7;
PixelBase( pixelBase(8&spiMaster3, GPIO_Pin_)7, GPIOB, G)PIO_Pin_8, GPIOB, 8;

PixelBase( pixelBase(9&spiMaster3, GPIO_Pin_)0, GPIOE, G)PIO_Pin_9, GPIOB, 9;
PixelBase( pixelBase(10&spiMaster2, GPIO_Pin)_9, GPIOE, )GPIO_Pin_10, GPIOE, 10;
PixelBase( pixelBase(11&spiMaster2, GPIO_Pin)_12, GPIOE,) GPIO_Pin_1, GPIOC, 11;
PixelBase( pixelBase(12&spiMaster2, GPIO_Pin)_11, GPIOD,) GPIO_Pin_12, GPIOD, 12;
PixelBase( pixelBase(13&spiMaster2, GPIO_Pin)_14, GPIOD,) GPIO_Pin_13, GPIOD, 13;
//内存池
MemoryPool DataPackMemoryPool = MemoryPool15;

*/

void createPixelBase()
{

    cameraList[0] = new PixelBase(&SpiMaster1, GPIOC, 1, GPIOC, 0, 0);
    cameraList[1] = new PixelBase(&SpiMaster1, GPIOA, 0, GPIOA, 1, 1);
    cameraList[2] = new PixelBase(&SpiMaster1, GPIOE, 4, GPIOE, 2, 2);
    cameraList[4] = new PixelBase(&SpiMaster1, GPIOC, 5, GPIOC, 4, 4);
    cameraList[7] = new PixelBase(&SpiMaster1, GPIOB, 1, GPIOE, 7, 7);

    cameraList[10] = new PixelBase(&SpiMaster2, GPIOE, 9, GPIOE, 10, 10);
    cameraList[11] = new PixelBase(&SpiMaster2, GPIOE, 11, GPIOE, 11, 11);
    cameraList[12] = new PixelBase(&SpiMaster2, GPIOD, 11, GPIOD, 12, 12);
    cameraList[13] = new PixelBase(&SpiMaster2, GPIOD, 14, GPIOD, 13, 13);
    cameraList[6] = new PixelBase(&SpiMaster2, GPIOC, 7, GPIOC, 6, 6);

    cameraList[3] = new PixelBase(&SpiMaster3, GPIOD, 1, GPIOD, 3, 3);
    cameraList[5] = new PixelBase(&SpiMaster3, GPIOD, 6, GPIOD, 5, 5);
    cameraList[8] = new PixelBase(&SpiMaster3, GPIOB, 7, GPIOB, 8, 8);
    cameraList[9] = new PixelBase(&SpiMaster3, GPIOE, 0, GPIOB, 9, 9);
}
