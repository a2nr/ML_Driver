/*
 * main_app.cpp
 *
 *  Created on: Mar 19, 2017
 *      Author: mori3rti
 */

#include "main.h"
#include "gpio.h"
#include "stm32f4xx_hal.h"
#include "SerialUart.h"

gpio gpioa(GPIOA);
digitalOut ledBiasa(&gpioa,GPIO_PIN_5);

SerialUart Serial(USART2);

void setup(void){
	  ledBiasa.begin();
	  ledBiasa.write(GPIO_PIN_RESET);
	  Serial.begin(9600);
}

void loop(void){

	  Serial.write("Hello World\r\n");
	  ledBiasa.write(GPIO_PIN_RESET);
	  HAL_Delay(1000);
	  ledBiasa.write(GPIO_PIN_SET);
	  HAL_Delay(1000);
}

