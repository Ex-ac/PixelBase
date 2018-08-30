#include "GpioPin.h"

void GpioPin_Init(GpioPin *gpioPin, GPIO_InitTypeDef *gpioInitData)
{
	uint16_t pin = gpioInitData->Pin;
	gpioInitData->Pin = gpioPin->pin;

	HAL_GPIO_Init(gpioPin->port, gpioInitData);
	gpioInitData->Pin = pin;
}

void GpioPin_OutputHight(GpioPin *gpioPin, bool y)
{
	HAL_GPIO_WritePin(gpioPin->port, gpioPin->pin, y ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

void GpioPin_Toggle(GpioPin *gpioPin)
{
	HAL_GPIO_TogglePin(gpioPin->port, gpioPin->pin);
}

bool GpioPin_IsInputHight(GpioPin *gpioPin)
{
	return HAL_GPIO_ReadPin(gpioPin->port, gpioPin->pin) == GPIO_PIN_SET;
}
