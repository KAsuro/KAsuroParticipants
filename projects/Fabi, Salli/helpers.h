#include <stdio.h>

void stopAsuro(int pause) // While Schleife zum Blinken benötigt!
{
	int status = 0;
	MotorSpeed(0, 0);
	if (status) { 				
		BackLED(ON, OFF);
		status = 0;
	} else {
		BackLED(OFF, ON);
		status = 1;
	}
	if (pause > 0)
		msleep(pause);
}

