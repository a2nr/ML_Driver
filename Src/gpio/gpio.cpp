/*
 * gpio.cpp
 *
 *  Created on: Jan 2, 2017
 *      Author: mori3rti
 */

#include "gpio.h"
#ifdef HAL_GPIO_MODULE_ENABLED

void gpio::deInit(uint16_t _pin){
	HAL_GPIO_DeInit(this->_GPIO, _pin);
}

void gpio::init(GPIO_InitTypeDef * GpioInitStructure){
	this->gpioClk(this->_GPIO,ENABLE);
	HAL_GPIO_Init(this->_GPIO, GpioInitStructure);
}

void gpio::gpioClk(GPIO_TypeDef * _GPIOx, bool _state){
	__IO uint32_t cntrX =0;

	/* Enabling Clock */
	while (cntrX < 8){
		if (_GPIOx == (GPIO_TypeDef *)(GPIOA_BASE + (0x400U * cntrX))) {
			if(_state == true)
				SET_BIT(RCC->AHB1ENR, (RCC_AHB1ENR_GPIOAEN << cntrX));
			else
				RCC->AHB1ENR &= ~(RCC_AHB1ENR_GPIOAEN << (cntrX));
			/* Delay after an RCC peripheral clock enabling */
			cntrX = RCC_AHB1ENR_GPIOAEN << cntrX;
			UNUSED(cntrX);
			break;
		}
		cntrX++;
	}
}
#endif /* HAL_GPIO_MODULE_ENABLED */
