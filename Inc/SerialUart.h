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

class SerialUart {
protected:

	static uint8_t TxBuff[MAX_BUFFER];
	static uint8_t RxBuff[MAX_BUFFER];
	uint8_t Roffset, Toffset,RoffsetHigh;
	uint8_t Rhead, Thead;

	void uartClk(UART_HandleTypeDef * ptrUartTd, bool _state);
public:
	UART_HandleTypeDef uartTd;
	SerialUart(){};
	SerialUart(USART_TypeDef *_instance);
	void begin(uint32_t _baudrate);

	~SerialUart();
	virtual void mspInit(void) = 0;
	virtual void mspDeinit(void) = 0;
	void write(const char * _someString);
	void write(uint8_t _chr);
	uint8_t read(void);
	uint8_t available(void);
	void RxComplete(void);
	void RxAbort(void);
	void IRQ(void);
	void HardwareError(void);
};

class SerialUart2 : public SerialUart {
private:

public:
	SerialUart2() : SerialUart(USART2) {};

	void mspInit(void);
	void mspDeinit(void);

};

extern SerialUart2 Serial;

#endif /* HAL_UART_MODULE_ENABLED */

#endif /* SERIALUART_H_ */
