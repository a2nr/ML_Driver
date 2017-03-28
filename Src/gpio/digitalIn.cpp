/*
 * digitalIn.cpp
 *
 *  Created on: Mar 27, 2017
 *      Author: root
 */

#include "gpio.h"
#ifdef HAL_GPIO_MODULE_ENABLED

digitalIn::digitalIn(gpio *gpiox, uint16_t _pin){
	this->_pin = _pin;
	this->ptrGpio = gpiox;
}
void digitalIn::begin(void){
	GPIO_InitTypeDef GpioInitStructure;
	GpioInitStructure.Pin = this->_pin;
	GpioInitStructure.Mode = GPIO_MODE_INPUT;
	GpioInitStructure.Pull = GPIO_PULLUP;
	GpioInitStructure.Speed = GPIO_SPEED_FREQ_MEDIUM;

	this->ptrGpio->init(&GpioInitStructure);
}
GPIO_PinState digitalIn::read(){
	return HAL_GPIO_ReadPin(this->ptrGpio->_GPIO,this->_pin);
}
#endif /* HAL_GPIO_MODULE_ENABLED */
