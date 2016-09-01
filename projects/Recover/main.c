/**
 * Beschreibt hier was euer Programm macht!
 */

#include <asuro/asuro.h>

void main(void) {
    Init();
	
	StatusLED(YELLOW);

	uint16_t data[2];
	uint16_t black[2];
	
	//set black value
	LineData(data);
	black[LEFT] = data[LEFT] + 150;
	black[RIGHT] = data[RIGHT] + 150;

    while (1) {
		//updated sensor value
		LineData(data);
        if((black[LEFT] < data[LEFT]) && (black[RIGHT] < data[RIGHT])) { //if not white
			MotorDir(FWD, RWD);
			MotorSpeed(90, 90);
			StatusLED(GREEN);
		} else if (data[LEFT] > data[RIGHT]) { //if left is brighter than right
			MotorDir(FWD, FWD);
			MotorSpeed(90, 100);
			StatusLED(RED);
		} else if (data[RIGHT] > data[LEFT]) { // if right is brighter than left
			MotorDir(FWD, FWD);
			MotorSpeed(100, 90);
		} else { //else
			MotorDir(FWD, FWD);
			MotorSpeed(90, 90);
			StausLED(ORANGE)
		}
        msleep(5);
    }
}
