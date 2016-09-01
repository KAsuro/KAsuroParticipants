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
			 msleep(800);
		}
		MotorDir(FWD, FWD); 
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
				
				stopAsuro(3000);
				
			}
		}
		switch (status) {
			
			case 0: 
			MotorSpeed(250, 175);
			status = 1;
			msleep(4000);
			break;
			
			case 1: 
			MotorSpeed(80,140);
			status = 2;
			msleep(4000);
			break;
			
			case 2: 
			MotorSpeed(220, 205); 
			status = 3;
			msleep(4000);
			break;
			
			case 3: 
			MotorSpeed(200, 260); 
			status = 0;
			msleep(4000);
			break;
			
			default: 
			MotorDir(BREAK, BREAK);
		}
		flashLED(500);
		BackLED(OFF, OFF);
		msleep(800); 				//Werkseinstellungen : LEDs auf off | StatusLED auf grün | Pause
	}
}

