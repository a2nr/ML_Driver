/*
 * Serial3.cpp
 *
 *  Created on: Mar 28, 2017
 *      Author: root
 */



#include "SerialUart.h"
#ifdef HAL_UART_MODULE_ENABLED

#include "error.h"
#include "gpio.h"
#include <string.h>

SerialUart4 Serial3;

void SerialUart4::mspInit(void){
	GPIO_InitTypeDef GPIO_InitStruct;
	gpio gpioUartTxRx(GPIOC);

	this->uartClk(&this->uartTd,ENABLE);
	/* Peripheral clock enable */
	//__HAL_RCC_USART6_CLK_ENABLE();
	/**USART6 GPIO Configuration
	PC6     ------> USART3_TX
	PC7     ------> USART3_RX
	*/
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF8_USART6;

	GPIO_InitStruct.Pin = GPIO_PIN_6 | GPIO_PIN_7;
	gpioUartTxRx.init(&GPIO_InitStruct);


	/* USART2_IRQn interrupt configuration */
	HAL_NVIC_SetPriority(USART6_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(USART6_IRQn);
}
void SerialUart4::mspDeinit(void){

	gpio gpioUartTxRx(GPIOC);

	/* Peripheral clock disable */
	this->uartClk(&this->uartTd,DISABLE);

	/**USART6 GPIO Configuration
	PC6     ------> USART3_TX
	PC7     ------> USART3_RX
	*/
	gpioUartTxRx.deInit(GPIO_PIN_6 | GPIO_PIN_7);
	HAL_NVIC_DisableIRQ(USART6_IRQn);
}

extern "C" {
	/**
	* @brief This function handles USART6 global interrupt.
	*/
	void USART6_IRQHandler(void)
	{
		Serial3.IRQ();
	}
}
#endif //#ifdef HAL_UART_MODULE_ENABLED


