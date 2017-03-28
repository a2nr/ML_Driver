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

#define ENABLE true
#define DISABLE false

class gpio {
protected:
	void gpioClk(GPIO_TypeDef * _GPIO, bool _state);
public:
	GPIO_TypeDef * _GPIO;
	gpio(GPIO_TypeDef * _GPIOx ) : _GPIO(_GPIOx){};
	gpio():_GPIO(NULL){};
	virtual ~gpio(){};

	void init(GPIO_InitTypeDef * GpioInitStructure);
	void deInit(uint16_t _pin);
};

class digitalOut : public gpio{
private:
	uint16_t _pin;
	gpio *ptrGpio;
public:
	digitalOut(gpio *gpiox, uint16_t _pin);
	void begin(void);
	void write(GPIO_PinState _state);
	void toggle(void);
};

class digitalIn : public gpio{
private:
	uint16_t _pin;
	gpio *ptrGpio;
public:
	digitalIn(gpio *gpiox, uint16_t _pin);
	void begin();
	GPIO_PinState read();

};


#endif /* HAL_GPIO_MODULE_ENABLED */
#endif /* INC_GPIO_H_ */
