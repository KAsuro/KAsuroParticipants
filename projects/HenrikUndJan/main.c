	
// 	   			(K)iT lives!
//*********************************************
//*|x|**| |***| |****[___]****[_________]**|x|*
//*|x|**| |***/ /*****| |*********| |******|x|*	 
//*|x|**| |**/ /******| |*********| |******|x|*
//*|x|**[     ]*******| |*********| |******|x|*
//*|x|**| |**\ \******| |*********| |******|x|*
//*|x|**| |***\ \*****| |*********| |******|x|*
//*|x|**| |***| |****[___]*******[___]*****|x|*
//*********************************************
//
#define SWITCH(X) 1<<X


#include <asuro/asuro.h>



void main(void)		{
    Init();
    
    int pressed[]	= {0,0,0,0,0,0,0,0,0,0,0};
    int t			= 0;
    int x			= 0;
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
		
		left  = ((int)lineData[LEFT] +160)  / 7;
		right = ((int)lineData[RIGHT]+100)  / 7;
		
		a_pressed = ( switches & SWITCH(1) );
		b_pressed = ( switches & SWITCH(4) );
		c_pressed = ( switches & SWITCH(0) );
		d_pressed = ( switches & SWITCH(3) );
		e_pressed = ( switches & SWITCH(2) );
		f_pressed = ( switches & SWITCH(5) );
		
		//fahren
		
		if (left>200 ){
			left=200;
		}
		else if(left<100){
			left=0;
			StatusLED(RED);
		}
				
		if (right>200){
			(right=200);
		}
		
		else if(right<100){
			right=0;
			StatusLED(RED);
		}
			
		MotorDir(FWD,FWD);
		MotorSpeed(right,left);
		msleep(120);
						
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
		
		if(80 < right && left < 120){
			StatusLED(YELLOW);
		}
		
		if(right && left > 120){
			StatusLED(GREEN);
		}
		
		
			if(x == 3000){
				FrontLED(ON);
				x=0;
		    }
		    if(x == 15){				
				FrontLED(OFF);
				x++;
			}
			else{
				x++;
			}
			
			
		
		//Rückfahrt
		
		if (a_pressed || b_pressed > 0) {
			StatusLED(RED);
			BackLED(OFF,ON);
			msleep(1);
			BackLED(ON,OFF);
			MotorDir(RWD,RWD);
			MotorSpeed(120,120);
			msleep(600);
			MotorDir(BREAK,BREAK);
		}
		
		if (c_pressed || d_pressed > 0) {
			StatusLED(RED);
			BackLED(OFF,ON);
			msleep(1);
			BackLED(ON,OFF);
			MotorDir(RWD,RWD);
			MotorSpeed(120,60);
			msleep(600);
			MotorDir(BREAK,BREAK);
		}
		
		if (e_pressed || f_pressed > 0) {
			StatusLED(RED);
			BackLED(OFF,ON);
			msleep(1);
			BackLED(ON,OFF);
			MotorDir(RWD,RWD);
			MotorSpeed(60,120);
			msleep(600);
			MotorDir(BREAK,BREAK);
		}	
	while(lineData[LEFT] || lineData[RIGHT] > 900){
		MotorDir(BREAK,BREAK);
		pressed[t]=pressed[t]+10;
		t++;
		if(t == 10){
			MotorDir(FWD,FWD);
			MotorSpeed(pressed[t],pressed[t]);
			msleep(1);
			t=0;
			}
		
		}
	
	
	

}
	}


//**********  ****    *****    *    *              *********************
//**********   ***    ****    **    *              *********************
//**********   ***    ***    ***    ******    **************************
//*    ******   ***   **    ****    ******    **************************
//**    *****   ***   *    *****    ******    **************************
//****    ****   **       ******    ******    **************************
//******    ***   *   *    *****    ******    **************************
//********     *  **  **    ****    ******    **************************
//***********      *  ***    ***    ******    **************************
//			*****     ****    **    ******    **************************
//					  *****    *    ******    *************************			
	
