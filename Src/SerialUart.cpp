/*
 * SerialUart.cpp
 *
 *  Created on: Jan 7, 2017
 *      Author: mori3rti
 */

#include "SerialUart.h"
#include "error.h"
#include "gpio.h"
#include <string.h>

SerialUart *SerialForMsp;

SerialUart::SerialUart(USART_TypeDef *_instance) {
	// TODO Auto-generated constructor stub
	SerialForMsp = this;
	uartTd.Instance = _instance;
	uartTd.Init.WordLength = UART_WORDLENGTH_8B;
	uartTd.Init.StopBits = UART_STOPBITS_1;
	uartTd.Init.Parity = UART_PARITY_NONE;
	uartTd.Init.Mode = UART_MODE_TX_RX;
	uartTd.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	uartTd.Init.OverSampling = UART_OVERSAMPLING_16;
}
void SerialUart::begin(uint32_t _baudrate){

	uartTd.Init.BaudRate = _baudrate;//115200;
	if (HAL_UART_Init(&this->uartTd) != HAL_OK)
	{
		Error.raiseError();
	}

}

SerialUart::~SerialUart() {
	// TODO Auto-generated destructor stub
	if(HAL_UART_DeInit(&this->uartTd) != HAL_OK){
		Error.raiseError();
	}
}

void SerialUart::uartClk(UART_HandleTypeDef * ptrUartTd, bool _state){
	__IO uint32_t tmpreg = 0x000U;
	while(tmpreg < 3){
		if(ptrUartTd->Instance == (USART_TypeDef*)(USART2+(0x400U * tmpreg))){
			if(_state == true)
				SET_BIT(RCC->APB1ENR, RCC_APB1ENR_USART2EN << (tmpreg));
			else
				RCC->APB1ENR &= ~(RCC_APB1ENR_USART2EN << (tmpreg));
			/* Delay after an RCC peripheral clock enabling */
			tmpreg = (RCC_APB1ENR_USART2EN << (tmpreg));
			UNUSED(tmpreg);
			break;
		}
		tmpreg++;
	}
}

void SerialUart::write(const char  * _someString){
	uint32_t size = strlen(_someString);
	memcpy(&TxBuff[0],_someString,size);

	HAL_UART_Transmit(&this->uartTd,TxBuff,strlen(_someString),500);
}

void SerialUart::mspInit(UART_HandleTypeDef * ptrUartTd){
	GPIO_InitTypeDef GPIO_InitStruct;
	this->uartClk(ptrUartTd,ENABLE);

	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;

	if(ptrUartTd->Instance==USART2)
	{
		gpio gpioUart(GPIOA);
		/* Peripheral clock enable */
		//__HAL_RCC_USART2_CLK_ENABLE();
		/**USART2 GPIO Configuration
		PA2     ------> USART2_TX
		PA3     ------> USART2_RX
		*/
		GPIO_InitStruct.Pin = GPIO_PIN_2|GPIO_PIN_3;
		GPIO_InitStruct.Alternate = GPIO_AF7_USART2;
		gpioUart.init(&GPIO_InitStruct);
	}
}

void SerialUart::mspDeinit(UART_HandleTypeDef * ptrUartTd){
	uint16_t _pin;
	if(ptrUartTd->Instance == USART2){
		gpio gpioUart(GPIOA);
		/**USART2 GPIO Configuration
		PA2     ------> USART2_TX
		PA3     ------> USART2_RX
		*/
		_pin = GPIO_PIN_2|GPIO_PIN_3;

		/* Peripheral clock disable */
		this->uartClk(ptrUartTd,DISABLE);
		gpioUart.deInit(_pin);
	}
}

extern "C" {
	void HAL_UART_MspInit(UART_HandleTypeDef* huart)
	{
		SerialForMsp->mspInit(huart);
	}

	void HAL_UART_MspDeInit(UART_HandleTypeDef* huart)
	{
		SerialForMsp->mspDeinit(huart);
	}
}

