#define SWITCH(X) 1<<X

#include <asuro/asuro.h>
#include "helpers.h"
#include <stdlib.h>

void main(void) {
    Init();
    int status = 0;
	//int x = 160;
	//int y = 200;
	int i = 0;
	while (1) {  			//Hauptschleife
		i++;
		char sensor = PollSwitch();
		sensor &= 0b00111111;
		MotorDir(FWD, FWD); //Werkeinstellung
		MotorSpeed(230, 200); 
		
		if (sensor > 0) 	//Abfrage der sechs Drucksensoren -> backwards
		{
			 MotorDir(BWD, BWD); 
			 MotorSpeed(60, 150);
			 StatusLED(RED);
			 msleep(800);
		}
		
		uint16_t data[] = {0,0};
		LineData(data);
		
		LineData(data);
		while ((data[LEFT] > 500) || (data[RIGHT] > 500)) 	//Geschwindigkeit 0 bei Licht über 100E
		{	
			stopAsuro(500);
			LineData(data);
		} 
		
		if ( (i % 3000) == 0) {
			int ran;
			ran = rand() % 2;
			if (ran) {
				StatusLED(YELLOW);
				stopAsuro(3000);
			}
		}
		switch (status) {
			case 0: 
			MotorSpeed(250, 50);
			status = 1;
			msleep(1000);
			break;
			case 1: 
			MotorSpeed(60,250);
			status = 2;
			msleep(1000);
			break;
			case 2: 
			MotorSpeed(230, 200); 
			status = 0;
			msleep(1000);
			break;
			default: 
			MotorDir(BREAK, BREAK);
		}
		StatusLED(GREEN);
		BackLED(OFF, OFF);
		msleep(800); 				//Werkseinstellungen : LEDs auf off | StatusLED auf grün | Pause
	}
}

