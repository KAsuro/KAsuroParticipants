/**
 * Beschreibt hier was euer Programm macht!
 */

#define SWITCH(X) 1<<X
#define SLOW 1000
#define FAST 100

#include <asuro/asuro.h>

void main(void) {
    Init();
	int status = 0;
	int x = 160;
	int y = 200;
	while (1) {
		char sensor = PollSwitch();
		sensor &= 0b00111111;
		
		MotorDir(FWD, FWD);
		MotorSpeed(160, 200);
		
		
		if (sensor > 0)
		{
			 MotorDir(BWD, BWD);
			 MotorSpeed(80, 200);
			 StatusLED(RED);
		}
		msleep(1000);
		uint16_t data[] = {0,0};
		LineData(data);
		while (((data[LEFT] > 40) || (data[RIGHT] > 40)) && (x > 10))
		{	
			MotorSpeed(x, y);
			x -= 5;
			y -= 5;
			LineData(data);
		} 
		while ((data[LEFT] > 100) || (data[RIGHT] > 100))
		{	
			MotorSpeed(0, 0);
			if (status) {
				BackLED(ON, OFF);
				status = 0;
			} else {
				BackLED(OFF, ON);
				status = 1;
			}
			msleep(500);
			LineData(data);
		} 
		StatusLED(GREEN);
		BackLED(OFF, OFF);
		msleep(500);
	}
};

