/*
 * gpio.h
 *
 *  Created on: Jan 2, 2017
 *      Author: mori3rti
 */

#ifndef INC_GPIO_H_
#define INC_GPIO_H_

#ifdef USE_FULL_LL_DRIVER
#include "stm32f4xx_ll_bus.h"
#include "stm32f4xx_ll_gpio.h"

#include "SerialUart.h"

class gpio
{
	friend class uart_base;
private:
	/* data */
	GPIO_TypeDef *GPIOx;
	uint8_t pin;

	void initFromStruct(LL_GPIO_InitTypeDef * initStruct);
public:
	gpio() : GPIOx(nullptr), pin(0) {}
	gpio(GPIO_TypeDef *GPIOx, uint32_t pin) : GPIOx(GPIOx), pin(pin) {}
	gpio(GPIO_TypeDef *GPIOx) : GPIOx(GPIOx), pin(0) {}

	void asOutput(void);
	void asInput(void);

	void setPullDown(void)
	{
		LL_GPIO_SetPinPull(this->GPIOx, 0x1U << this->pin, LL_GPIO_PULL_DOWN);
	}
	void setPullUp(void)
	{
		LL_GPIO_SetPinPull(this->GPIOx, 0x1U << this->pin, LL_GPIO_PULL_UP);
	}
	void toggle(){
		LL_GPIO_TogglePin(this->GPIOx, 0x1U << this->pin);
	}
	void set(uint8_t bit) {
		LL_GPIO_SetOutputPin(this->GPIOx, (bit == 0x1U) ? 0x1U << this->pin :(0x1U << this->pin) << 16 );
	}
	uint8_t get(void)
	{
		return LL_GPIO_IsInputPinSet(this->GPIOx, 0x1U << this->pin) != 0 ? 0x1U : 0x0U;
	}
};

#else
#include "stm32f4xx_hal.h"

#ifdef HAL_GPIO_MODULE_ENABLED

#define HIGH GPIO_PIN_SET
#define LOW GPIO_PIN_RESET

class digitalOut;
class digitalIn;

#define ENABLE true
#define DISABLE false

class gpio
{
protected:
	void gpioClk(GPIO_TypeDef *_GPIO, bool _state);

public:
	GPIO_TypeDef *_GPIO;
	gpio(GPIO_TypeDef *_GPIOx) : _GPIO(_GPIOx){};
	gpio() : _GPIO(NULL){};
	virtual ~gpio(){};

	void init(GPIO_InitTypeDef *GpioInitStructure);
	void deInit(uint16_t _pin);
};

class digitalOut : public gpio
{
private:
	uint16_t _pin;
	gpio *ptrGpio;

public:
	digitalOut(gpio *gpiox, uint16_t _pin);
	void begin(void);
	void write(GPIO_PinState _state);
	void toggle(void);
};

class digitalIn : public gpio
{
private:
	uint16_t _pin;
	gpio *ptrGpio;

public:
	digitalIn(gpio *gpiox, uint16_t _pin);
	void begin();
	GPIO_PinState read();
};

#endif /* HAL_GPIO_MODULE_ENABLED */
#endif
#endif /* INC_GPIO_H_ */
