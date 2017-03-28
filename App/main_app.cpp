/*
 * main_app.cpp
 *
 *  Created on: Mar 19, 2017
 *      Author: mori3rti
 */

#include "gpio.h"
#include "SerialUart.h"

gpio gpioa(GPIOA);
gpio gpioPushB(GPIOC);
digitalOut ledBiasa(&gpioa,GPIO_PIN_5);
digitalIn PushB(&gpioPushB,GPIO_PIN_13);

#define _SERIAL	Serial

void setup(void){
	  ledBiasa.begin();
	  PushB.begin();
	  _SERIAL.begin(921600);
	  HAL_Delay(2000);
	  _SERIAL.write("\r\nSTM    >>Assalamu'alaikum Homan ... :)");
	  _SERIAL.write("\r\nHoman  >>");
}

void loop(void){
	uint8_t x;
	bool flag = false;
	while(_SERIAL.available() > 0){
		x = _SERIAL.read();
		_SERIAL.write(x);
		flag=true;
	}
	if(flag == true){
		_SERIAL.write("\r\nHoman  >>");
		flag = false;
	}
	if(PushB.read() != GPIO_PIN_SET) {
		HAL_Delay(200);
		ledBiasa.toggle();
	}

}

