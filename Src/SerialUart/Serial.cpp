/*
 * SerialUart2.cpp
 *
 *  Created on: Mar 21, 2017
 *      Author: root
 */

#include "SerialUart.h"
#ifdef HAL_UART_MODULE_ENABLED

#include "error.h"
#include "gpio.h"
#include <string.h>

SerialUart2 Serial;

void SerialUart2::mspInit(void){
	GPIO_InitTypeDef GPIO_InitStruct;
	gpio gpioUart(GPIOA);

	this->uartClk(&this->uartTd,ENABLE);
	/* Peripheral clock enable */
	//__HAL_RCC_USART2_CLK_ENABLE();
	/**USART2 GPIO Configuration
	PA2     ------> USART2_TX
	PA3     ------> USART2_RX
	*/
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Pin = GPIO_PIN_2|GPIO_PIN_3;
	GPIO_InitStruct.Alternate = GPIO_AF7_USART2;
	gpioUart.init(&GPIO_InitStruct);


	/* USART2_IRQn interrupt configuration */
	HAL_NVIC_SetPriority(USART2_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(USART2_IRQn);
}
void SerialUart2::mspDeinit(void){
	uint16_t _pin;
	gpio gpioUart(GPIOA);
	/**USART2 GPIO Configuration
	PA2     ------> USART2_TX
	PA3     ------> USART2_RX
	*/
	_pin = GPIO_PIN_2|GPIO_PIN_3;

	/* Peripheral clock disable */
	this->uartClk(&this->uartTd,DISABLE);
	gpioUart.deInit(_pin);
	HAL_NVIC_DisableIRQ(USART2_IRQn);
}

extern "C" {
	/**
	* @brief This function handles USART2 global interrupt.
	*/
	void USART2_IRQHandler(void)
	{
		Serial.IRQ();
	}
}
#endif //#ifdef HAL_UART_MODULE_ENABLED
