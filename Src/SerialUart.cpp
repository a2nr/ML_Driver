/*
 * SerialUart.cpp
 *
 *  Created on: Jan 7, 2017
 *      Author: mori3rti
 */

#include "SerialUart.h"
#include "error.h"
#include "gpio.h"

SerialUart *SerialForMsp;

void InitMsp(UART_HandleTypeDef* huart);
void DeInitMsp(UART_HandleTypeDef* huart);


SerialUart::SerialUart(USART_TypeDef *_instance, uint32_t _baudrate) {
	// TODO Auto-generated constructor stub
	SerialForMsp = this;
	uartTd.Instance = _instance;
	uartTd.Init.BaudRate = _baudrate;//115200;
	uartTd.Init.WordLength = UART_WORDLENGTH_8B;
	uartTd.Init.StopBits = UART_STOPBITS_1;
	uartTd.Init.Parity = UART_PARITY_NONE;
	uartTd.Init.Mode = UART_MODE_TX_RX;
	uartTd.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	uartTd.Init.OverSampling = UART_OVERSAMPLING_16;
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

void SerialUart::clkEnable(UART_HandleTypeDef * ptrUartTd){
	__IO uint32_t tmpreg = 0x000U;
	while(tmpreg < 3){
		if(ptrUartTd->Instance == (USART_TypeDef*)(USART2+(0x400U * tmpreg))){
			SET_BIT(RCC->APB1ENR, RCC_APB1ENR_USART2EN << (tmpreg));
			/* Delay after an RCC peripheral clock enabling */
			tmpreg = READ_BIT(RCC->APB1ENR, RCC_APB1ENR_USART2EN << (tmpreg));
			UNUSED(tmpreg);
			break;
		}
		tmpreg++;
	}
}

void SerialUart::clkDisable(UART_HandleTypeDef * ptrUartTd){
	__IO uint32_t tmpreg = 0x000U;
	while(tmpreg < 3){
		if(ptrUartTd->Instance == (USART_TypeDef*)(USART2+(0x400U * tmpreg))){
			RCC->APB1ENR &= ~(RCC_APB1ENR_USART2EN << (tmpreg));
			/* Delay after an RCC peripheral clock enabling */
			tmpreg = READ_BIT(RCC->APB1ENR, RCC_APB1ENR_USART2EN << (tmpreg));
			UNUSED(tmpreg);
			break;
		}
		tmpreg++;
	}
}

void SerialUart::mspInit(UART_HandleTypeDef * ptrUartTd){
	GPIO_InitTypeDef GPIO_InitStruct;

	this->clkEnable(ptrUartTd);

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
	if(ptrUartTd->Instance==USART2)
	{
		gpio gpioUart(GPIOA);
		/* Peripheral clock disable */
		this->clkDisable(ptrUartTd);

		/**USART2 GPIO Configuration
		PA2     ------> USART2_TX
		PA3     ------> USART2_RX
		*/
		gpioUart.deInit(GPIO_PIN_2|GPIO_PIN_3);
	}
}
void InitMsp(UART_HandleTypeDef* huart){
	SerialForMsp->mspInit(huart);
}
void DeInitMsp(UART_HandleTypeDef* huart){
	SerialForMsp->mspDeinit(huart);
}
void HAL_UART_MspInit(UART_HandleTypeDef* huart)
{
	InitMsp(huart);
}

void HAL_UART_MspDeInit(UART_HandleTypeDef* huart)
{
	DeInitMsp(huart);
}

