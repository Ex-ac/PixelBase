#ifndef GPIOPIN_H_
#define GPIOPIN_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include "../../PortingLayer/GlobalDefine.h"



typedef struct TagGpioPin
{
	GPIO_TypeDef *port;
	uint16_t pin;
}GpioPin;

void GpioPin_Init(GpioPin *gpinPin, GPIO_InitTypeDef *gpioInitData);
void GpioPin_OutputHight(GpioPin *gpioPin, bool y);
void GpioPin_Toggle(GpioPin *gpioPin);

bool GpioPin_IsInputHight(GpioPin *gpioPin);

#ifdef __cplusplus
}
#endif

#endif
