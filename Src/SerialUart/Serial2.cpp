/*
 * Serial2.cpp
 *
 *  Created on: Mar 28, 2017
 *      Author: root
 */


#include "SerialUart.h"
#ifdef HAL_UART_MODULE_ENABLED

#include "error.h"
#include "gpio.h"
#include <string.h>

SerialUart3 Serial2;

void SerialUart3::mspInit(void){
	GPIO_InitTypeDef GPIO_InitStruct;
	gpio gpioUartTx(GPIOB);
	gpio gpioUartRx(GPIOC);

	this->uartClk(&this->uartTd,ENABLE);
	/* Peripheral clock enable */
	//__HAL_RCC_USART3_CLK_ENABLE();
	/**USART3 GPIO Configuration
	PB10    ------> USART3_TX
	PC5     ------> USART3_RX
	*/
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF7_USART3;

	GPIO_InitStruct.Pin = GPIO_PIN_10;
	gpioUartTx.init(&GPIO_InitStruct);

	GPIO_InitStruct.Pin = GPIO_PIN_5;
	gpioUartRx.init(&GPIO_InitStruct);


	/* USART2_IRQn interrupt configuration */
	HAL_NVIC_SetPriority(USART3_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(USART3_IRQn);
}
void SerialUart3::mspDeinit(void){

	gpio gpioUartTx(GPIOB);
	gpio gpioUartRx(GPIOC);

	/* Peripheral clock disable */
	this->uartClk(&this->uartTd,DISABLE);

	/**USART3 GPIO Configuration
	PB10     ------> USART3_TX
	PC5      ------> USART3_RX
	*/
	gpioUartTx.deInit(GPIO_PIN_10);
	gpioUartRx.deInit(GPIO_PIN_5);
	HAL_NVIC_DisableIRQ(USART3_IRQn);
}

extern "C" {
	/**
	* @brief This function handles USART3 global interrupt.
	*/
	void USART3_IRQHandler(void)
	{
		Serial2.IRQ();
	}
}
#endif //#ifdef HAL_UART_MODULE_ENABLED


