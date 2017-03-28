/*
 * Serial5.cpp
 *
 *  Created on: Mar 28, 2017
 *      Author: root
 */


#include "SerialUart.h"
#ifdef HAL_UART_MODULE_ENABLED

#include "error.h"
#include "gpio.h"
#include <string.h>

SerialUart6 Serial5;

void SerialUart6::mspInit(void){
	GPIO_InitTypeDef GPIO_InitStruct;
	gpio gpioUartTx(GPIOC);
	gpio gpioUartRx(GPIOD);

	this->uartClk(&this->uartTd,ENABLE);
	/* Peripheral clock enable */
	//__HAL_RCC_UART5_CLK_ENABLE();
	/**U5ART5 GPIO Configuration
	PC12    ------> UART5_TX
	PD2     ------> UART5_RX
	*/
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF8_UART5;

	GPIO_InitStruct.Pin = GPIO_PIN_12;
	gpioUartTx.init(&GPIO_InitStruct);

	GPIO_InitStruct.Pin = GPIO_PIN_2;
	gpioUartRx.init(&GPIO_InitStruct);


	/* USART2_IRQn interrupt configuration */
	HAL_NVIC_SetPriority(UART5_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(UART5_IRQn);
}
void SerialUart6::mspDeinit(void){

	gpio gpioUartTx(GPIOC);
	gpio gpioUartRx(GPIOD);

	/* Peripheral clock disable */
	this->uartClk(&this->uartTd,DISABLE);

	/**U5ART5 GPIO Configuration
	PC12    ------> UART5_TX
	PD2     ------> UART5_RX
	*/
	gpioUartTx.deInit(GPIO_PIN_12);
	gpioUartRx.deInit(GPIO_PIN_2);
	HAL_NVIC_DisableIRQ(UART5_IRQn);
}

extern "C" {
	/**
	* @brief This function handles UART5 global interrupt.
	*/
	void UART5_IRQHandler(void)
	{
		Serial5.IRQ();
	}
}
#endif //#ifdef HAL_UART_MODULE_ENABLED
