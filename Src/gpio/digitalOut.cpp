/*
 * digitalOut.cpp
 *
 *  Created on: Mar 27, 2017
 *      Author: root
 */

#include "gpio.h"
#ifdef HAL_GPIO_MODULE_ENABLED

digitalOut::digitalOut(gpio *gpiox, uint16_t _pin){
	this->_pin = _pin;
	this->ptrGpio = gpiox;
}

void digitalOut::begin(void){
	GPIO_InitTypeDef GpioInitStructure;

	GpioInitStructure.Pin = this->_pin;
	GpioInitStructure.Mode = GPIO_MODE_OUTPUT_PP;
	GpioInitStructure.Pull = GPIO_NOPULL;
	GpioInitStructure.Speed = GPIO_SPEED_FREQ_MEDIUM;

	this->ptrGpio->init(&GpioInitStructure);
}

void digitalOut::write(GPIO_PinState _state){
	HAL_GPIO_WritePin(this->ptrGpio->_GPIO,_pin,_state);
}

void digitalOut::toggle(void){
	HAL_GPIO_TogglePin(this->ptrGpio->_GPIO,_pin);
}

#endif

