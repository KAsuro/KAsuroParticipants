/**
 * 	   (K)iT lives!
 *		  	  *
 *           * *
 * 			* * *
 *         * * * *
 *        * * * * *
 * 		 * * * * * *
 * 		* * * * * * *
 *     * * * * * * * *
 *    * * * * * * * * *
 *		   	 * *
 *		   	 * *
 * 		   	 * *
 * 
 */

#define SWITCH(X) 1<<X
#define SLOW 1000
#define FAST 100

#include <asuro/asuro.h>

void main(void)		{
    Init();
    
    int left 		= 0;
    int right 		= 0;
	char a_pressed	= 0;
	char b_pressed	= 0;
	char c_pressed	= 0;
	char d_pressed	= 0;
	char e_pressed	= 0;
	char f_pressed	= 0;
    
	while(1){
		
		
		uint16_t lineData[] = {0,0};
		LineData(lineData);
		
		char switches = PollSwitch();
        switches &= 0b00111111;
		
		left  = ((int)lineData[LEFT]+60	)  / 6;
		right = ((int)lineData[RIGHT]	)  / 6;
		
		a_pressed = ( switches & SWITCH(1) );
		b_pressed = ( switches & SWITCH(4) );
		c_pressed = ( switches & SWITCH(0) );
		d_pressed = ( switches & SWITCH(3) );
		e_pressed = ( switches & SWITCH(2) );
		f_pressed = ( switches & SWITCH(5) );
		
		//fahren
		
		MotorDir(FWD,FWD);
		MotorSpeed(right,left);
		msleep(120);
		
		if (left>255){
			left=254;
			}
		else if(left<100){
			left=0;
			}	
			
		if (right>255){
			right=254;
			}
		else if(right<100){
			right=0;
			}	
					
					
		//LED Steuerung
		
		if(left > right){
			BackLED(OFF,ON);
		}
		if(right > left){
			BackLED(ON,OFF);
		}
		if(right == left){
			BackLED(ON,ON);
		}
		if(right && left < 80){
			StatusLED(RED);
		}
		if(80 < right && left < 100){
			StatusLED(YELLOW);
		}
		if(right && left > 100){
			StatusLED(GREEN);
		}
		
		//Rückfahrt
		
		if (a_pressed | b_pressed > 0) {
			BackLED(OFF,ON);
			msleep(1);
			BackLED(ON,OFF);
			MotorDir(RWD,RWD);
			MotorSpeed(120,120);
			msleep(600);
			MotorDir(BREAK,BREAK);
			}
		
		if (c_pressed | d_pressed > 0) {
			BackLED(OFF,ON);
			msleep(1);
			BackLED(ON,OFF);
			MotorDir(RWD,RWD);
			MotorSpeed(120,60);
			msleep(600);
			MotorDir(BREAK,BREAK);
			}
		
		if (e_pressed | f_pressed > 0) {
			BackLED(OFF,ON);
			msleep(1);
			BackLED(ON,OFF);
			MotorDir(RWD,RWD);
			MotorSpeed(60,120);
			msleep(600);
			MotorDir(BREAK,BREAK);
			}
			
			
		//Lichtsuche
		
		if(left && right < 90){
			MotorDir(RWD,FWD);
			MotorSpeed(120,120);
			}
					
}	
}		
	
