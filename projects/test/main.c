/**
 * Beschreibt hier was euer Programm macht!
 */
#include <asuro/asuro.h>

void main(void) {
	uint16_t white[] = {0, 0};
	uint16_t data[]  = {0, 0};
	int turns = 0;

    Init();
    
    MotorDir(FWD,FWD);
    MotorSpeed(80,80);
    
    char switches = PollSwitch();
		
	switches &= 0b00111111;
		
	LineData(data);
	white[LEFT] = data[LEFT] - 200;
	white[RIGHT] = data[RIGHT] - 200;
	
	while(1) {
		LineData(data);
		if((white[LEFT] > data[LEFT]) && (white[RIGHT] > data[RIGHT])) {
			GoTurn(0,90,80);
			turns++;
			if(turns == 2) {
				turns = 0;
				GoTurn(0,180,80);
			}
		}
		msleep(5);
	}
}
