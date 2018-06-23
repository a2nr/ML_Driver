/*
 * SerialUart.h
 *
 *  Created on: Jan 7, 2017
 *      Author: mori3rti
 */

#ifndef SERIALUART_H_
#define SERIALUART_H_

#ifdef USE_FULL_LL_DRIVER

#include "stm32f4xx_ll_gpio.h"
#include "stm32f4xx_ll_rcc.h"
#include "stm32f4xx_ll_usart.h"
#include "stm32f4xx_ll_bus.h"

typedef struct uart_ts_cfg
{
	uint32_t txgpio;
	uint32_t txpin;
	uint32_t rxgpio;
	uint32_t rxpin;
	uint32_t uartx;
	uint32_t rcc;
	uint32_t periph;
	uint32_t irqn;
	uint32_t ioaf;
} uart_cfg_ts;

typedef enum uart_error_code : uint8_t
{
	uart_nok,
	uart_ok
} uart_error;
namespace uart_1
{

static const uart_cfg_ts uart_config = {
	GPIOA_BASE,
	LL_GPIO_PIN_9,
	GPIOA_BASE,
	LL_GPIO_PIN_10,
	USART1_BASE,
	(uint32_t)&RCC->APB2ENR,
	LL_APB2_GRP1_PERIPH_USART1,
	USART1_IRQn,
	LL_GPIO_AF_7};

} // namespace uart_1
namespace uart_2
{
static const uart_cfg_ts uart_config = {
	GPIOA_BASE,
	LL_GPIO_PIN_2,
	GPIOA_BASE,
	LL_GPIO_PIN_3,
	USART2_BASE,
	(uint32_t)&RCC->APB1ENR,
	LL_APB1_GRP1_PERIPH_USART2,
	USART2_IRQn,
	LL_GPIO_AF_7};
} // namespace uart_2
namespace uart_3
{
static const uart_cfg_ts uart_config = {
	GPIOB_BASE,
	LL_GPIO_PIN_10,
	GPIOC_BASE,
	LL_GPIO_PIN_5,
	USART3_BASE,
	(uint32_t)&RCC->APB1ENR,
	LL_APB1_GRP1_PERIPH_USART3,
	USART3_IRQn,
	LL_GPIO_AF_7};
} // namespace uart_3
namespace uart_4
{
static const uart_cfg_ts uart_config = {
	GPIOA_BASE,
	LL_GPIO_PIN_0,
	GPIOA_BASE,
	LL_GPIO_PIN_1,
	UART4_BASE,
	(uint32_t)&RCC->APB1ENR,
	LL_APB1_GRP1_PERIPH_UART4,
	UART4_IRQn,
	LL_GPIO_AF_8};
} // namespace uart_4
namespace uart_5
{
static const uart_cfg_ts uart_config = {
	GPIOC_BASE,
	LL_GPIO_PIN_12,
	GPIOD_BASE,
	LL_GPIO_PIN_2,
	UART5_BASE,
	(uint32_t)&RCC->APB1ENR,
	LL_APB1_GRP1_PERIPH_UART5,
	UART5_IRQn,
	LL_GPIO_AF_8};
} // namespace uart_5
namespace uart_6
{
static const uart_cfg_ts uart_config = {
	GPIOC_BASE,
	LL_GPIO_PIN_6,
	GPIOC_BASE,
	LL_GPIO_PIN_7,
	USART6_BASE,
	(uint32_t)&RCC->APB2ENR,
	LL_APB2_GRP1_PERIPH_USART6,
	USART6_IRQn,
	LL_GPIO_AF_8};
} // namespace uart_6

void uart_init(uint32_t baudrate, const uart_cfg_ts &cfg);
class uart_base
{
  private:
	const uart_cfg_ts &cfg;

  protected:
	uart_base(const uart_cfg_ts &cfg) : cfg(cfg){};

  public:
	uart_error init(uint32_t baudrate);
	void write(const uint8_t *x, uint32_t len);
	uint32_t read(uint8_t *pByte, uint32_t len, uint32_t timeout);
};

class uart1 : public uart_base
{
  public:
	uart1() : uart_base(uart_1::uart_config){};
};
class uart2 : public uart_base
{
  public:
	uart2() : uart_base(uart_2::uart_config){};
};
class uart3 : public uart_base
{
  public:
	uart3() : uart_base(uart_3::uart_config){};
};
class uart4 : public uart_base
{
  public:
	uart4() : uart_base(uart_4::uart_config){};
};
class uart5 : public uart_base
{
  public:
	uart5() : uart_base(uart_5::uart_config){};
};
class uart6 : public uart_base
{
  public:
	uart6() : uart_base(uart_6::uart_config){};
};
#elif HAL_UART_MODULE_ENABLED

#define ENABLE true
#define DISABLE false

#define MAX_BUFFER 128

class SerialUart
{
  protected:
	static uint8_t TxBuff[MAX_BUFFER];
	static uint8_t RxBuff[MAX_BUFFER];
	uint8_t Roffset, RoffsetHigh, Rhead;

	void uartClk(UART_HandleTypeDef *ptrUartTd, bool _state);

  public:
	UART_HandleTypeDef uartTd;
	SerialUart() : Roffset(0), RoffsetHigh(0), Rhead(0){};
	SerialUart(USART_TypeDef *_instance);
	void begin(uint32_t _baudrate);

	virtual ~SerialUart();
	virtual void mspInit(void) = 0;
	virtual void mspDeinit(void) = 0;

	void write(const char *_someString);
	void write(uint8_t _chr);

	uint8_t read(void);
	uint8_t available(void);

	void RxComplete(void);
	void IRQ(void);
	void HardwareError(void);
};

class SerialUart2 : public SerialUart
{
  public:
	SerialUart2() : SerialUart(USART2){};

	void mspInit(void);
	void mspDeinit(void);
};

class SerialUart1 : public SerialUart
{
  public:
	SerialUart1() : SerialUart(USART1){};

	void mspInit(void);
	void mspDeinit(void);
};

class SerialUart3 : public SerialUart
{
  public:
	SerialUart3() : SerialUart(USART3){};

	void mspInit(void);
	void mspDeinit(void);
};

class SerialUart4 : public SerialUart
{
  public:
	SerialUart4() : SerialUart(USART6){};

	void mspInit(void);
	void mspDeinit(void);
};

class SerialUart5 : public SerialUart
{
  public:
	SerialUart5() : SerialUart(UART4){};

	void mspInit(void);
	void mspDeinit(void);
};

class SerialUart6 : public SerialUart
{
  public:
	SerialUart6() : SerialUart(UART5){};

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
