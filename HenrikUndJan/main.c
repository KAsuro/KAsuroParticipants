/**
 * Beschreibt hier was euer Programm macht!
 */

#define SWITCH(X) 1<<X
#define SLOW 1000
#define FAST 100

#include <asuro/asuro.h>

void main(void) {
    Init();
    
    int left = 0;
    int right = 0;
      
    //int state = 0;
    //int interval = FAST;
	char a_pressed = 0;
	char b_pressed = 0;
    
	//for(int i=0;i<10;i++)
	while(1){
		
		uint16_t lineData[] = {0,0};
		LineData(lineData);
		
		char switches = PollSwitch();
        switches &= 0b00111111;
		
		
		// lineData[LEFT]
		// lineData[RIGHT]
		left  = ((int)lineData[LEFT]+60)  / 6;
		right = ((int)lineData[RIGHT]	) / 6;	
			
		
		if (left>255){
			left=254;
		}
			
		if (right>255){
			right=254;
		}
		
		if(left > right){
			BackLED(OFF,ON);
		}
		if(right > left){
			BackLED(ON,OFF);
		}
		if(right == left){
			BackLED(ON,ON);
		}
		MotorDir(FWD,FWD);
		MotorSpeed(right,left);
		msleep(120);
		
		a_pressed = ( switches & SWITCH(1) );
        b_pressed = ( switches & SWITCH(3) );
		
		if (a_pressed | b_pressed > 0) {
            BackLED(OFF,ON);
            msleep(1);
            BackLED(ON,OFF);
			MotorDir(RWD,RWD);
			MotorSpeed(120,120);
			msleep(50);
			MotorDir(BREAK,BREAK);


		}
		
			 
		//~ if (lineData[LEFT] > lineData[RIGHT]){
			//~ MotorDir(FWD,BWD);
			//~ MotorSpeed(right,left);
			//~ msleep(2);
			//~ MotorDir(BREAK,BREAK);


			
			
			
			//~ }
		//~ else
		//~ {
			//~ MotorDir(BWD,FWD);
			//~ MotorSpeed(right,left);
			//~ msleep(2);
			//~ MotorDir(BREAK,BREAK);
		
			
			//~ }	
	
			
		}
	}	

			
			
			
			
		
	
