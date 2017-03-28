/*
 * Serial1.cpp
 *
 *  Created on: Mar 28, 2017
 *      Author: root
 */

#include "SerialUart.h"
#ifdef HAL_UART_MODULE_ENABLED

#include "error.h"
#include "gpio.h"
#include <string.h>

SerialUart1 Serial1;

void SerialUart1::mspInit(void){
	GPIO_InitTypeDef GPIO_InitStruct;
	gpio gpioUart(GPIOA);

	this->uartClk(&this->uartTd,ENABLE);
	/* Peripheral clock enable */
	//__HAL_RCC_USART1_CLK_ENABLE();
	/**USART1 GPIO Configuration
	PA9      ------> USART1_TX
	PA10     ------> USART1_RX
	*/
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Pin = GPIO_PIN_9|GPIO_PIN_10;
	GPIO_InitStruct.Alternate = GPIO_AF7_USART1;
	gpioUart.init(&GPIO_InitStruct);


	/* USART2_IRQn interrupt configuration */
	HAL_NVIC_SetPriority(USART1_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(USART1_IRQn);
}
void SerialUart1::mspDeinit(void){
	uint16_t _pin;
	gpio gpioUart(GPIOA);
	/**USART1 GPIO Configuration
	PA9     ------> USART1_TX
	PA10    ------> USART1_RX
	*/
	_pin = GPIO_PIN_9|GPIO_PIN_10;

	/* Peripheral clock disable */
	this->uartClk(&this->uartTd,DISABLE);
	gpioUart.deInit(_pin);
	HAL_NVIC_DisableIRQ(USART1_IRQn);
}

extern "C" {
	/**
	* @brief This function handles USART1 global interrupt.
	*/
	void USART1_IRQHandler(void)
	{
		Serial1.IRQ();
	}
}
#endif //#ifdef HAL_UART_MODULE_ENABLED


