/*
 * Serial4.cpp
 *
 *  Created on: Mar 28, 2017
 *      Author: root
 */

#include "SerialUart.h"
#ifdef HAL_UART_MODULE_ENABLED

#include "error.h"
#include "gpio.h"
#include <string.h>

SerialUart5 Serial4;

void SerialUart5::mspInit(void){
	GPIO_InitTypeDef GPIO_InitStruct;
	gpio gpioUart(GPIOA);

	this->uartClk(&this->uartTd,ENABLE);
	/* Peripheral clock enable */
	//__HAL_RCC_UART4_CLK_ENABLE();
	/**UART4 GPIO Configuration
	PA0     ------> UART4_TX
	PA1     ------> UART4_RX
	*/
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1;
	GPIO_InitStruct.Alternate = GPIO_AF8_UART4;
	gpioUart.init(&GPIO_InitStruct);


	/* USART2_IRQn interrupt configuration */
	HAL_NVIC_SetPriority(UART4_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(UART4_IRQn);
}
void SerialUart5::mspDeinit(void){
	uint16_t _pin;
	gpio gpioUart(GPIOA);
	/**UART4 GPIO Configuration
	PA0     ------> UART4_TX
	PA1     ------> UART4_RX
	*/
	_pin = GPIO_PIN_0|GPIO_PIN_1;

	/* Peripheral clock disable */
	this->uartClk(&this->uartTd,DISABLE);
	gpioUart.deInit(_pin);
	HAL_NVIC_DisableIRQ(UART4_IRQn);
}

extern "C" {
	/**
	* @brief This function handles USART2 global interrupt.
	*/
	void UART4_IRQHandler(void)
	{
		Serial4.IRQ();
	}
}
#endif //#ifdef HAL_UART_MODULE_ENABLED
