#include <stdio.h>

void stopAsuro(int pause) 
{
	MotorSpeed(0, 0);
	if (pause > 0)
		msleep(pause);
}

