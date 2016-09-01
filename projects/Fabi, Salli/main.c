#define SWITCH(X) 1<<X

#include <asuro/asuro.h>
#include "helpers.h"
#include <stdlib.h>

void main(void) {
    Init();
	//int x = 160;
	//int y = 200;
	int i = 0;
	
	while (1) {  	//Hauptschleife
		i++;
		char sensor = PollSwitch();
		sensor &= 0b00111111;
		
		MotorDir(FWD, FWD); //Werkeinstellung
		MotorSpeed(170, 130); 
		
		
		if (sensor > 0) //Abfrage der sechs Drucksensoren -> backwards
		{
			 MotorDir(BWD, BWD); 
			 MotorSpeed(60, 150);
			 StatusLED(RED);
			 msleep(800);
		}
		
		uint16_t data[] = {0,0};
		LineData(data);
		
		if (((data[LEFT] > 50) || (data[RIGHT] > 50)) && ((data[LEFT] < 150) || (data[RIGHT] < 150))) { 		//Geschwindigkeit geringer bei Licht über 50E
				MotorSpeed(60, 80);
				LineData(data);
				msleep(2000);
		}
		while ((data[LEFT] > 150) || (data[RIGHT] > 150)) 	//Geschwindigkeit 0 bei Licht über 100E
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
		MotorSpeed(160, 220);
		StatusLED(GREEN);
		BackLED(OFF, OFF);
		msleep(1000); 				//Werkseinstellungen : LEDs auf off | StatusLED auf grün | Pause
	}
};

