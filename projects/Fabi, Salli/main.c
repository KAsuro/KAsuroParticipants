#define SWITCH(X) 1<<X

#include <asuro/asuro.h>
#include "helpers.h"
#include <stdlib.h>

void main(void) {
    Init();
	//int x = 160;
	//int y = 200;
	int i = 0;
	while (1) {  			//Hauptschleife
		i++;
		char sensor = PollSwitch();
		sensor &= 0b00111111;
		
		MotorDir(FWD, FWD); //Werkeinstellung
		MotorSpeed(230, 190); 
		
		
		if (sensor > 0) 	//Abfrage der sechs Drucksensoren -> backwards
		{
			 MotorDir(BWD, BWD); 
			 MotorSpeed(60, 150);
			 StatusLED(RED);
			 msleep(800);
		}
		
		uint16_t data[] = {0,0};
		LineData(data);
		
		/*if (((data[LEFT] > 100) || (data[RIGHT] > 100)) && ((data[LEFT] < 400) || (data[RIGHT] < 400))) { 		//Geschwindigkeit geringer bei Licht über 50E
				MotorSpeed(50, 70);
				msleep(2000);
		}*/
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
		
		/*if ((data[LEFT] > data[RIGHT]) && ((data[LEFT] < 400) || (data[RIGHT] < 400)))
		{
			MotorSpeed(70, 140);
			LineData(data);
			msleep(100);
		}
		
		if ((data[LEFT] < data[RIGHT]) && ((data[LEFT] < 400) || (data[RIGHT] < 400)))
		{
			MotorSpeed(180, 90);
			LineData(data);
			msleep(100);
		}*/
		StatusLED(GREEN);
		BackLED(OFF, OFF);
		msleep(800); 				//Werkseinstellungen : LEDs auf off | StatusLED auf grün | Pause
	}
}

