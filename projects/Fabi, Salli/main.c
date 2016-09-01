#define SWITCH(X) 1<<X

#include <asuro/asuro.h>
#include "helpers.h"
#include <stdlib.h>

void main(void) {
    Init();
    int status = 0;
    int sstatus = 0;
	//int x = 160;
	//int y = 200;
	//int i = 0;
	while (1) {  			//Hauptschleife
		//i++;
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
		while ((data[LEFT] > 400) || (data[RIGHT] > 400)) 	//Geschwindigkeit 0 bei Licht über 100E
		{	
			stopAsuro(500);
			LineData(data);
		} 
		
		/*if ( (i % 3000) == 0) {
			int ran;
			ran = rand() % 2;
			if (ran) {
				
				stopAsuro(3000);
				
			}
		}*/
		switch (status) {
			
			case 0: 
			MotorSpeed(250, 220);
			status = 1;
			msleep(4000);
			break;
			
			case 1: 
			MotorSpeed(80,140);
			status = 2;
			msleep(1000);
			break;
			
			case 2: 
			MotorSpeed(220, 205); 
			status = 3;
			msleep(4000);
			break;
			
			case 3: 
			MotorSpeed(235, 260); 
			status = 0;
			msleep(4000);
			break;
			
			default: 
			MotorDir(BREAK, BREAK);
		}
		
		switch (sstatus)
		{
			case 0:
			StatusLED(RED);
			sstatus = 1;
			break;
			
			case 1:
			StatusLED(GREEN);
			sstatus = 2;
			break;
			
			case 2:
			StatusLED(YELLOW);
			sstatus = 0;
			break;
		}
		msleep(500); 				
	}
}
