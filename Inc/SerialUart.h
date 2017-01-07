/*
 * SerialUart.h
 *
 *  Created on: Jan 7, 2017
 *      Author: mori3rti
 */

#ifndef SERIALUART_H_
#define SERIALUART_H_

#include "stm32f4xx_hal.h"

#ifdef HAL_UART_MODULE_ENABLED

class SerialUart {
private:
	void clkEnable(UART_HandleTypeDef * ptrUartTd);
	void clkDisable(UART_HandleTypeDef * ptrUartTd);
public:
	UART_HandleTypeDef uartTd;
	SerialUart(){};
	SerialUart(USART_TypeDef *_instance, uint32_t _baudrate);
	virtual ~SerialUart();
	void mspInit(UART_HandleTypeDef * ptrUartTd);
	void mspDeinit(UART_HandleTypeDef * ptrUartTd);
};


#endif /* HAL_UART_MODULE_ENABLED */

#endif /* SERIALUART_H_ */
