/*
 * main_app.cpp
 *
 *  Created on: Mar 19, 2017
 *      Author: mori3rti
 */


gpio gpioa(GPIOA);
gpio gpioPushB(GPIOC);
digitalOut ledBiasa(&gpioa,GPIO_PIN_5);
digitalIn PushB(&gpioPushB,GPIO_PIN_13);


void setup(void){
	  ledBiasa.begin();
	  PushB.begin();
	  Serial.begin(9600);
	  HAL_Delay(2000);
	  Serial.write("\r\nSTM    >>Assalamu'alaikum Homan ... :)");
	  Serial.write("\r\nHoman  >>");
}

void loop(void){
	uint8_t x;
	bool flag = false;
	while(Serial.available() > 0){
		x = Serial.read();
		Serial.write(x);
		flag=true;
	}
	if(flag == true){
		Serial.write("\r\nHoman  >>");
		flag = false;
	}
	if(PushB.read() != GPIO_PIN_SET) {
		HAL_Delay(200);
		ledBiasa.write(GPIO_PIN_SET);
		HAL_Delay(200);
		ledBiasa.write(GPIO_PIN_RESET);
	}

}

