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
/*float random() 
{
	int konst = 1;
	int teiler[10] = {1,2,3,4,5,6,7,8,9,10};
	randomNumber = konst / a;
} */

