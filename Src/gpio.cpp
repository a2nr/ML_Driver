/*
 * gpio.cpp
 *
 *  Created on: Jan 2, 2017
 *      Author: mori3rti
 */

#include <gpio.h>
#ifdef HAL_GPIO_MODULE_ENABLED

gpio::gpio(GPIO_TypeDef * _GPIOx){

	gpio::_GPIO = _GPIOx;

	gpio::enableClk(gpio::_GPIO);
}

void gpio::enableClk(GPIO_TypeDef * _GPIOx){
	__IO uint32_t cntrX =0;

	/* Enabling Clock */
	while (cntrX < 8){
		if (_GPIOx == (GPIO_TypeDef *)(GPIOA_BASE | (0x400U * cntrX))) {
			SET_BIT(RCC->AHB1ENR, (RCC_AHB1ENR_GPIOAEN << cntrX));
			/* Delay after an RCC peripheral clock enabling */
			cntrX = RCC_AHB1ENR_GPIOAEN << cntrX;
			UNUSED(cntrX);
			break;
		}
		cntrX++;
	}
}

void gpio::init(GPIO_InitTypeDef * GpioInitStructure){

	HAL_GPIO_Init(gpio::_GPIO, GpioInitStructure);

}


digitalOut::digitalOut(gpio *gpiox, uint16_t _pin){
	GPIO_InitTypeDef GpioInitStructure;
	digitalOut::_pin = _pin;
	digitalOut::ptrGpio = gpiox;

	GpioInitStructure.Pin = digitalOut::_pin;
	GpioInitStructure.Mode = GPIO_MODE_OUTPUT_PP;
	GpioInitStructure.Pull = GPIO_NOPULL;
	GpioInitStructure.Speed = GPIO_SPEED_FREQ_MEDIUM;

	digitalOut::ptrGpio->init(&GpioInitStructure);
}

void digitalOut::write(GPIO_PinState _state){
	HAL_GPIO_WritePin(digitalOut::ptrGpio->_GPIO,_pin,_state);
}


digitalIn::digitalIn(gpio *gpiox, uint16_t _pin){
	GPIO_InitTypeDef GpioInitStructure;
	digitalIn::_pin = _pin;
	digitalIn::ptrGpio = gpiox;

	GpioInitStructure.Pin = digitalIn::_pin;
	GpioInitStructure.Mode = GPIO_MODE_INPUT;
	GpioInitStructure.Pull = GPIO_PULLUP;
	GpioInitStructure.Speed = GPIO_SPEED_FREQ_MEDIUM;

	digitalIn::ptrGpio->init(&GpioInitStructure);
}
GPIO_PinState digitalIn::read(){
	return HAL_GPIO_ReadPin(digitalIn::ptrGpio->_GPIO,digitalIn::_pin);
}
#endif /* HAL_GPIO_MODULE_ENABLED */
