#include <stdio.h>

void stopAsuro(int pause) 
{
	MotorSpeed(0, 0);
	if (pause > 0)
		msleep(pause);
}
/*float randomNumb() 
{
	int konst = 1;
	float teiler[20] = {0.1,0.2,0.3,0.4,0.5,0.6,0.7,0.8,0.9,1,2,3,4,5,6,7,8,9,10};
	return randomNumber = konst / teiler.at() 
}*/

