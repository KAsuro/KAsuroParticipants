/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   any later version.                                                    *
 ***************************************************************************/

#include "asuro.h"
#include "timerservice.h"

#define FAULT 250
#define MAX_SPEED 120
#define MIN_SPEED 40

unsigned int switches;

inline int lineCorrection(int ldata)
{
	// only use for right line data (in my case)
	float x = ldata / 1024.0;
	return ldata + FAULT * 4 * x * (x - 1);
}

inline int getSpeed(int light)
{
	float x = light / 1024.0;
	x = 1-x;
	//return MIN_SPEED + (MAX_SPEED - MIN_SPEED)*x*x;
	return light;
}

int main(void)
{
	Init();

	unsigned int linData[2];
	FrontLED(ON);
	while (1)
	{
		unsigned short SerDelay = 2;
		LineData(linData);
		SerPrint("a,");
		Msleep(SerDelay);
		PrintInt(linData[LEFT]);
		Msleep(SerDelay);
		SerPrint(",");
		Msleep(SerDelay);
		PrintInt(lineCorrection(linData[RIGHT]));
		Msleep(SerDelay);
		SerPrint("\n");
		
	}
	
	return 0;
}
