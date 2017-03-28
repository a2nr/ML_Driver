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
	uint8_t Roffset, RoffsetHigh,Rhead;

	void uartClk(UART_HandleTypeDef * ptrUartTd, bool _state);
public:
	UART_HandleTypeDef uartTd;
	SerialUart() : Roffset(0), RoffsetHigh(0), Rhead(0){};
	SerialUart(USART_TypeDef *_instance);
	void begin(uint32_t _baudrate);

	virtual ~SerialUart();
	virtual void mspInit(void) = 0;
	virtual void mspDeinit(void) = 0;

	void write(const char * _someString);
	void write(uint8_t _chr);

	uint8_t read(void);
	uint8_t available(void);

	void RxComplete(void);
	void IRQ(void);
	void HardwareError(void);
};

class SerialUart2 : public SerialUart {
public:
	SerialUart2() : SerialUart(USART2) {};

	void mspInit(void);
	void mspDeinit(void);

};

class SerialUart1 : public SerialUart {
public:
	SerialUart1() : SerialUart(USART1) {};

	void mspInit(void);
	void mspDeinit(void);

};

class SerialUart3 : public SerialUart {
public:
	SerialUart3() : SerialUart(USART3) {};

	void mspInit(void);
	void mspDeinit(void);

};

class SerialUart4 : public SerialUart {
public:
	SerialUart4() : SerialUart(USART6) {};

	void mspInit(void);
	void mspDeinit(void);

};

class SerialUart5 : public SerialUart {
public:
	SerialUart5() : SerialUart(UART4) {};

	void mspInit(void);
	void mspDeinit(void);

};

class SerialUart6 : public SerialUart {
public:
	SerialUart6() : SerialUart(UART5) {};

	void mspInit(void);
	void mspDeinit(void);

};

extern SerialUart2 Serial;
extern SerialUart1 Serial1;
extern SerialUart3 Serial2;
extern SerialUart4 Serial3;
extern SerialUart5 Serial4;
extern SerialUart6 Serial5;

#endif /* HAL_UART_MODULE_ENABLED */

#endif /* SERIALUART_H_ */
