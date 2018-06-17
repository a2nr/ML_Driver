/*
 * SerialUart.cpp
 *
 *  Created on: Jan 7, 2017
 *      Author: mori3rti
 */

#include "SerialUart.h"

#ifdef USE_FULL_LL_DRIVER
uart_error uart_base::init(uint32_t baudrate)
{
	LL_USART_InitTypeDef USART_InitStruct;

	LL_GPIO_InitTypeDef GPIO_InitStruct;

	/* Peripheral clock enable */
	// LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_USART1);
	{
		__IO uint32_t *myRcc = (__IO uint32_t *)cfg.rcc;
		__IO uint32_t tmpreg;
		SET_BIT(*myRcc, cfg.periph);
		tmpreg = READ_BIT(*myRcc, cfg.periph);
		(void)tmpreg;
		(void)myRcc;
	}

	/**USART1 GPIO Configuration  
  PA9   ------> USART1_TX
  PA10   ------> USART1_RX 
  */
	GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
	GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
	GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;
	GPIO_InitStruct.Alternate = LL_GPIO_AF_7;
	if (cfg.txgpio != cfg.rxgpio)
	{
		GPIO_InitStruct.Pin = cfg.txpin;
		LL_GPIO_Init((GPIO_TypeDef *)cfg.txgpio, &GPIO_InitStruct);
		GPIO_InitStruct.Pin = cfg.rxpin;
		LL_GPIO_Init((GPIO_TypeDef *)cfg.rxgpio, &GPIO_InitStruct);
	}
	else
	{
		GPIO_InitStruct.Pin = cfg.txpin | cfg.rxpin;
		LL_GPIO_Init((GPIO_TypeDef *)cfg.txgpio, &GPIO_InitStruct);
	}

	/* USART1 interrupt Init */
	// NVIC_SetPriority(irqn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 0, 0));
	// NVIC_EnableIRQ(irqn);

	USART_InitStruct.BaudRate = baudrate;
	USART_InitStruct.DataWidth = LL_USART_DATAWIDTH_8B;
	USART_InitStruct.StopBits = LL_USART_STOPBITS_1;
	USART_InitStruct.Parity = LL_USART_PARITY_NONE;
	USART_InitStruct.TransferDirection = LL_USART_DIRECTION_TX_RX;
	USART_InitStruct.HardwareFlowControl = LL_USART_HWCONTROL_NONE;
	USART_InitStruct.OverSampling = LL_USART_OVERSAMPLING_16;
	LL_USART_Init((USART_TypeDef *)cfg.uartx, &USART_InitStruct);

	LL_USART_ConfigAsyncMode((USART_TypeDef *)cfg.uartx);

	// LL_USART_Enable((USART_TypeDef *)cfg.uartx);
}
uint32_t uart_base::read(uint8_t *pByte, uint32_t len)
{
	/*return actual len*/
	return 0x00; 
}
void uart_base::write(const uint8_t *pByte, uint32_t len)
{
	USART_TypeDef *myUsart = (USART_TypeDef *)this->cfg.uartx;
	LL_USART_Enable(myUsart);

	while (len--)
	{
		while (!LL_USART_IsActiveFlag_TXE(myUsart))
		{
			/* waiting TX = 1*/
		}
		/* set the DR and automaticly set TX = 0*/
		LL_USART_TransmitData8(myUsart, *(pByte++));
	}

	LL_USART_Disable(myUsart);
}
uint8_t uart_base::available(void)
{
	return 0;
}
#elif HAL_UART_MODULE_ENABLED

#include "error.h"
#include "gpio.h"
#include <string.h>

uint8_t SerialUart::TxBuff[MAX_BUFFER];
uint8_t SerialUart::RxBuff[MAX_BUFFER];

SerialUart::SerialUart(USART_TypeDef *_instance)
{
	//inisialisasi variable uart
	uartTd.Instance = _instance;
	uartTd.Init.WordLength = UART_WORDLENGTH_8B;
	uartTd.Init.StopBits = UART_STOPBITS_1;
	uartTd.Init.Parity = UART_PARITY_NONE;
	uartTd.Init.Mode = UART_MODE_TX_RX;
	uartTd.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	uartTd.Init.OverSampling = UART_OVERSAMPLING_16;

	Rhead = 0;
	Roffset = 0;
}
void SerialUart::begin(uint32_t _baudrate)
{
	//setting baudrate
	uartTd.Init.BaudRate = _baudrate;
	//Inisialisasi Uart
	if (HAL_UART_Init(&this->uartTd) != HAL_OK)
	{
		Error.raiseError();
	}
}

SerialUart::~SerialUart()
{
	//deinitialisasi uart
	if (HAL_UART_DeInit(&this->uartTd) != HAL_OK)
	{
		Error.raiseError();
	}
}
/*
 * @param UART_HandleTypeDef * ptrUartTd -> ini UART Handle TypeDef
 * @param bool _state -> ENABLE Clock
 * 						 DISABLE Clock
 */
void SerialUart::uartClk(UART_HandleTypeDef *ptrUartTd, bool _state)
{
	__IO uint32_t tmpreg = 0x000U;
	__IO uint32_t x;
	__IO uint32_t uart;

	if ((ptrUartTd->Instance <= UART5) && (ptrUartTd->Instance >= USART2))
	{
		while (tmpreg < 4)
		{
			x = (0x400U * tmpreg);
			uart = (USART2_BASE + x);
			if (ptrUartTd->Instance == (USART_TypeDef *)uart)
			{
				if (_state == true)
					SET_BIT(RCC->APB1ENR, RCC_APB1ENR_USART2EN << (tmpreg));
				else
					RCC->APB1ENR &= ~(RCC_APB1ENR_USART2EN << (tmpreg));
				/* Delay after an RCC peripheral clock enabling */
				tmpreg = (RCC_APB1ENR_USART2EN << (tmpreg));
				break;
			}

			tmpreg++;
		}
	}
	else if ((ptrUartTd->Instance >= USART1) && (ptrUartTd->Instance <= USART6))
	{
		while (tmpreg < 2)
		{
			x = (0x400U * tmpreg);
			uart = (USART1_BASE + x);
			if (ptrUartTd->Instance == (USART_TypeDef *)uart)
			{
				if (_state == true)
					SET_BIT(RCC->APB2ENR, RCC_APB2ENR_USART1EN << (tmpreg));
				else
					RCC->APB2ENR &= ~(RCC_APB2ENR_USART1EN << (tmpreg));

				/* Delay after an RCC peripheral clock enabling */
				tmpreg = (RCC_APB2ENR_USART1EN << (tmpreg));
				break;
			}

			tmpreg++;
		}
	}
	UNUSED(tmpreg);
}

void SerialUart::write(const char *_someString)
{
	uint32_t size = strlen(_someString);
	memcpy(&this->TxBuff[0], _someString, size);

	if (HAL_UART_Transmit(&this->uartTd, (uint8_t *)this->TxBuff, size, 500) != HAL_OK)
		Error.raiseError();
}

void SerialUart::write(uint8_t _chr)
{
	if (HAL_UART_Transmit(&this->uartTd, &_chr, 1, 500) != HAL_OK)
		Error.raiseError();
}

uint8_t SerialUart::read(void)
{
	//	HAL_UART_AbortReceive_IT(&this->uartTd);
	//	if(this->uartTd.ErrorCode & )d){
	if (this->Rhead > this->Roffset)
		return this->RxBuff[this->Roffset++];
	return 0;
}

uint8_t SerialUart::available(void)
{
	HAL_UART_StateTypeDef State;
	if (this->Rhead > this->Roffset)
	{
		UNUSED(State);
		return this->Rhead - this->Roffset;
	}
	else
	{
		State = HAL_UART_GetState(&this->uartTd);
	}
	if ((State == HAL_UART_STATE_READY))
		if (HAL_UART_Receive_IT(&this->uartTd, &this->RxBuff[0], MAX_BUFFER) == HAL_OK)
		{
		};
	return 0;
}

void SerialUart::RxComplete(void)
{

	this->Roffset = 0;
	this->Rhead = this->RoffsetHigh;
	this->RoffsetHigh = 0;
}

void SerialUart::IRQ(void)
{
	//baca data dari register dan mengakses RxComplate
	HAL_UART_IRQHandler(&uartTd);

	if ((this->RxBuff[this->RoffsetHigh] != '\n') && (this->RxBuff[this->RoffsetHigh] != '\r') && (this->RxBuff[this->RoffsetHigh] != '\0'))
		this->RoffsetHigh++;
	else
		HAL_UART_AbortReceive_IT(&this->uartTd);
}

void SerialUart::HardwareError(void)
{
	if (this->RoffsetHigh != MAX_BUFFER)
	{
		uint32_t Error = HAL_UART_GetError(&this->uartTd);
		switch (Error)
		{
		case HAL_UART_ERROR_NONE: /*!< No error            */
			break;
		case HAL_UART_ERROR_PE: /*!< Parity error        */
			break;
		case HAL_UART_ERROR_NE: /*!< Noise error         */
			break;
		case HAL_UART_ERROR_FE: /*!< Frame error         */
			break;
		case HAL_UART_ERROR_ORE: /*!< Overrun error       */
			//TODO getstat dulu baru ke sini
			if (HAL_UART_Receive_IT(&this->uartTd, (uint8_t *)this->RxBuff, MAX_BUFFER) == HAL_OK)
				break;
			break;
		case HAL_UART_ERROR_DMA: /*!< DMA transfer error  */
			break;
		}
	}
	else
	{
		//overrun buffer
		this->RxComplete();
	}
}
extern "C"
{
	void HAL_UART_AbortReceiveCpltCallback(UART_HandleTypeDef *huart)
	{
		if (huart->Instance == USART2)
		{
			Serial.RxComplete();
		}
		else if (huart->Instance == USART1)
		{
			Serial1.RxComplete();
		}
		else if (huart->Instance == USART3)
		{
			Serial2.RxComplete();
		}
		else if (huart->Instance == USART6)
		{
			Serial3.RxComplete();
		}
		else if (huart->Instance == UART4)
		{
			Serial4.RxComplete();
		}
		else if (huart->Instance == UART5)
		{
			Serial5.RxComplete();
		}
		else
		{
			Error.raiseError();
		}
	}
	void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
	{

		if (huart->Instance == USART2)
		{
			Serial.HardwareError();
		}
		else if (huart->Instance == USART1)
		{
			Serial1.HardwareError();
		}
		else if (huart->Instance == USART3)
		{
			Serial2.HardwareError();
		}
		else if (huart->Instance == USART6)
		{
			Serial3.HardwareError();
		}
		else if (huart->Instance == UART4)
		{
			Serial4.HardwareError();
		}
		else if (huart->Instance == UART5)
		{
			Serial5.HardwareError();
		}
		else
		{
			Error.raiseError();
		}
	}

	void HAL_UART_MspInit(UART_HandleTypeDef *huart)
	{
		if (huart->Instance == USART2)
		{
			Serial.mspInit();
		}
		else if (huart->Instance == USART1)
		{
			Serial1.mspInit();
		}
		else if (huart->Instance == USART3)
		{
			Serial2.mspInit();
		}
		else if (huart->Instance == USART6)
		{
			Serial3.mspInit();
		}
		else if (huart->Instance == UART4)
		{
			Serial4.mspInit();
		}
		else if (huart->Instance == UART5)
		{
			Serial5.mspInit();
		}
		else
		{
			Error.raiseError();
		}
	}

	void HAL_UART_MspDeInit(UART_HandleTypeDef *huart)
	{
		if (huart->Instance == USART2)
		{
			Serial.mspDeinit();
		}
		else if (huart->Instance == USART1)
		{
			Serial1.mspDeinit();
		}
		else if (huart->Instance == USART3)
		{
			Serial2.mspDeinit();
		}
		else if (huart->Instance == USART6)
		{
			Serial3.mspDeinit();
		}
		else if (huart->Instance == UART4)
		{
			Serial4.mspDeinit();
		}
		else if (huart->Instance == UART5)
		{
			Serial5.mspDeinit();
		}
		else
		{
			Error.raiseError();
		}
	}

	void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
	{
		if (huart->Instance == USART2)
		{
			Serial.HardwareError();
		}
		else if (huart->Instance == USART1)
		{
			Serial1.HardwareError();
		}
		else if (huart->Instance == USART3)
		{
			Serial2.HardwareError();
		}
		else if (huart->Instance == USART6)
		{
			Serial3.HardwareError();
		}
		else if (huart->Instance == UART4)
		{
			Serial4.HardwareError();
		}
		else if (huart->Instance == UART5)
		{
			Serial5.HardwareError();
		}
		else
		{
			Error.raiseError();
		}
	}
}

#endif //#ifdef HAL_UART_MODULE_ENABLED
