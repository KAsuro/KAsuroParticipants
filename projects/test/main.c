/**
 * Beschreibt hier was euer Programm macht!
 */
#include <asuro/asuro.h>

void main(void) {
	int white[] = {0, 0};
	int black[] = {0, 0};
	uint16_t data[]  = {0, 0};

    Init();
    
    MotorDir(FWD,FWD);
    MotorSpeed(80,80);
    
    
    while (1) {
		char switches = PollSwitch();
		int start_white = 0;
		int start_black = 0;
		
		switches &= 0b00111111;
		
		LineData(data);
		if(!start_white) {
			if (switches >= 64) {
				LineData(data);
				white[RIGHT] = data[RIGHT];
				white[LEFT] = data[LEFT];
				start_white = 1;
			}
		}
		
		if(!start_black) {
			if ((switches >= 16) && (switches < 64)) {
				LineData(data);
				black[RIGHT] = data[RIGHT];
				black[LEFT] = black[LEFT];
				start_black = 1;
			}
		}
       
		if(data[RIGHT] == data[LEFT] == start_white) {
		   MotorDir(FWD,FWD);
		   MotorSpeed(85,85);
		}
		if(data[RIGHT] == data[LEFT] == start_black) {
		   GoTurn(0,90,80);
		  
		}
	} 
}
