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
		int sstatus = 0;
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
		msleep(intervall);
	}
}

void stopLED () 
{
	StatusLED(OFF);
}
