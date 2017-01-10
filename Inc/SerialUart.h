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

#define ENABLE true
#define DISABLE false

#define MAX_BUFFER	128

static uint8_t TxBuff[MAX_BUFFER];
class SerialUart {
private:
	void uartClk(UART_HandleTypeDef * ptrUartTd, bool _state);
public:
	UART_HandleTypeDef uartTd;
	SerialUart(){};
	SerialUart(USART_TypeDef *_instance);
	void begin(uint32_t _baudrate);
	virtual ~SerialUart();
	void mspInit(UART_HandleTypeDef * ptrUartTd);
	void mspDeinit(UART_HandleTypeDef * ptrUartTd);
	void write(const char * _someString);
};


#endif /* HAL_UART_MODULE_ENABLED */

#endif /* SERIALUART_H_ */
