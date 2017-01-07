/*
 * gpio.h
 *
 *  Created on: Jan 2, 2017
 *      Author: mori3rti
 */

#ifndef INC_GPIO_H_
#define INC_GPIO_H_
#include "stm32f4xx_hal.h"

#ifdef HAL_GPIO_MODULE_ENABLED

#define HIGH 	GPIO_PIN_SET
#define LOW		GPIO_PIN_RESET

class digitalOut;
class digitalIn;

class gpio{
friend class digitalOut;
friend class digitalIn;
private:
	GPIO_TypeDef * _GPIO;
	void enableClk(GPIO_TypeDef * _GPIO);
public:
	gpio(){};
	gpio(GPIO_TypeDef * _GPIOx );
	~gpio();

	void init(GPIO_InitTypeDef * GpioInitStructure);
	void deInit(uint16_t _pin);
};

class digitalOut: public gpio{
private:
	uint16_t _pin;
	gpio *ptrGpio;
public:
	digitalOut(){};
	digitalOut(gpio *gpiox, uint16_t _pin);
	void write(GPIO_PinState _state);
};

class digitalIn: public gpio{
private:
	uint16_t _pin;
	gpio *ptrGpio;
public:
	digitalIn(){};
	digitalIn(gpio *gpiox, uint16_t _pin);
	GPIO_PinState read();

};


#endif /* HAL_GPIO_MODULE_ENABLED */
#endif /* INC_GPIO_H_ */
