#include <stdio.h>

void stopAsuro(int pause) 
{
	MotorSpeed(0, 0);
	if (pause > 0)
		msleep(pause);
}

void flashLED (int intervall)
{
	while (1)
	{
		int status = 0;
		switch (status)
		{
			case 0:
			StatusLED(RED);
			status = 1;
			break;
			
			case 1:
			StatusLED(GREEN);
			status = 2;
			break;
			
			case 2:
			StatusLED(YELLOW);
			status = 0;
			break;
		}
		msleep(intervall);
	}
}

void stopLED () 
{
	StatusLED(OFF);
}
