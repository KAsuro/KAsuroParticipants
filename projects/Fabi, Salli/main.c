#define SWITCH(X) 1<<X

#include <asuro/asuro.h>
#include "helpers.h"

void main(void) {
    Init();
	int x = 160;
	int y = 200;
	
	while (1) {  	//Hauptschleife
		
		char sensor = PollSwitch();
		sensor &= 0b00111111;
		
		MotorDir(FWD, FWD); //Werkeinstellung
		MotorSpeed(160, 200); 
		
		
		if (sensor > 0) //Abfrage der sechs Drucksensoren -> backwards
		{
			 MotorDir(BWD, BWD); 
			 MotorSpeed(80, 200);
			 StatusLED(RED);
			 msleep(1500);
		}
		
		uint16_t data[] = {0,0};
		LineData(data);
		
		while (((data[LEFT] > 40) || (data[RIGHT] > 40)) && (x > 10)) //Geschwindigkeit geringer bei Licht über 40E
		{	
			StatusLED(YELLOW);
			x -= 5;
			y -= 5;
			MotorSpeed(x, y);
			LineData(data);
		} 
		while ((data[LEFT] > 100) || (data[RIGHT] > 100)) //Geschwindigkeit 0 bei Licht über 100E
		{	
			stopAsuro(500);
			LineData(data);
		} 
		MotorSpeed(160, 200);
		int i = 0;
		for(i = 0; i % 100 == 0; i++) //Gelegentlich kurzer Richtungswechsel
		{
			MotorSpeed(10, 200);
			msleep(100);
		}
		for(i = 0; i % 101 == 0; i++) //Gelegentlich kurzer Richtungswechsel 2.0
		{
			MotorSpeed(200, 10);
			msleep(100);
		}
		StatusLED(GREEN);
		BackLED(OFF, OFF);
		msleep(500); 				//Werkseinstellungen : LEDs auf off | StatusLED auf grün | Pause
	}
};

