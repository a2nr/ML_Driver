/*
 * gpio.cpp
 *
 *  Created on: Jan 2, 2017
 *      Author: mori3rti
 */

#include "gpio.h"
#include "main.h"

#ifdef USE_FULL_LL_DRIVER

#include "stm32f4xx_ll_gpio.h"
#include "stm32f4xx_ll_bus.h"

static void gpio_enableClock(GPIO_TypeDef *GPIOx)
{
	union addres_gpio {
		struct byte_
		{
			uint32_t _1 : 8;
			uint32_t _2 : 8;
		} byte;
		uint32_t _32;
	};
	union addres_gpio gpio_curent;
	uint8_t addresMap;

	gpio_curent._32 = (uint32_t)GPIOx;

	/* chose gpiox depend on address */
	switch (gpio_curent.byte._2)
	{
	case 0x00:
		addresMap = 0;
		break;
	case 0x04:
		addresMap = 1;
		break;
	case 0x08:
		addresMap = 2;
		break;
	case 0x0C:
		addresMap = 3;
		break;
	case 0x10:
		addresMap = 4;
		break;
	case 0x14:
		addresMap = 5;
		break;
	case 0x18:
		addresMap = 6;
		break;
	case 0x1C:
		addresMap = 7;
		break;
	}

	LL_AHB1_GRP1_EnableClock((LL_AHB1_GRP1_PERIPH_GPIOA << addresMap));
}

void gpio::asOutput(void)
{
	LL_GPIO_InitTypeDef GPIO_InitStruct;
	this->GPIOx = GPIOx;

	gpio_enableClock(this->GPIOx);

	this->pin = pin;

	LL_GPIO_StructInit(&GPIO_InitStruct);
	GPIO_InitStruct.Pin = 0x1U << pin;
	GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
	GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
	GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
	GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
	LL_GPIO_Init(this->GPIOx, &GPIO_InitStruct);
}

void gpio::asInput(void)
{
	LL_GPIO_InitTypeDef GPIO_InitStruct;
	this->GPIOx = GPIOx;

	gpio_enableClock(this->GPIOx);

	this->pin = pin;

	LL_GPIO_StructInit(&GPIO_InitStruct);
	GPIO_InitStruct.Pin = 0x1U << pin;
	GPIO_InitStruct.Mode = LL_GPIO_MODE_INPUT;
	GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
	GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
	LL_GPIO_Init(this->GPIOx, &GPIO_InitStruct);
}

#else
#ifdef HAL_GPIO_MODULE_ENABLED

void gpio::deInit(uint16_t _pin)
{
	HAL_GPIO_DeInit(this->_GPIO, _pin);
}

void gpio::init(GPIO_InitTypeDef *GpioInitStructure)
{
	this->gpioClk(this->_GPIO, ENABLE);
	HAL_GPIO_Init(this->_GPIO, GpioInitStructure);
}

void gpio::gpioClk(GPIO_TypeDef *_GPIOx, bool _state)
{
	union addres_gpio {
		struct byte_
		{
			uint32_t _1 : 8;
			uint32_t _2 : 8;
		} byte;
		uint32_t _32;
	};
	union addres_gpio gpio_curent;
	uint32_t temp;
	gpio_curent._32 = (uint32_t)_GPIOx;
	/* chose gpiox depend on address */
	switch (gpio_curent.byte._2)
	{
	case 0x00:
		temp = 0;
		break;
	case 0x04:
		temp = 1;
		break;
	case 0x08:
		temp = 2;
		break;
	case 0x0C:
		temp = 3;
		break;
	case 0x10:
		temp = 4;
		break;
	case 0x14:
		temp = 5;
		break;
	case 0x18:
		temp = 6;
		break;
	case 0x1C:
		temp = 7;
		break;
	}

	/* Enabling Clock */
	if (_state == true)
		SET_BIT(RCC->AHB1ENR, (RCC_AHB1ENR_GPIOAEN << temp));
	else
		CLEAR_BIT(RCC->AHB1ENR, (RCC_AHB1ENR_GPIOAEN << temp));
	/* Delay after an RCC peripheral clock enabling */
	temp = READ_BIT(RCC->AHB1ENR, (RCC_AHB1ENR_GPIOAEN << temp));
}
#endif /* HAL_GPIO_MODULE_ENABLED */
#endif
