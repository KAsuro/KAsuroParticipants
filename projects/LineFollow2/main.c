/**
 *	Der ASURO soll einer Linie folgen
 *  und bei anstoßen andersrum weiterfahren
 **/


#define SWITCH(X) 1<<X

#include <asuro/asuro.h>

void main(void) {
    Init();
	FrontLED(1);
	msleep(500);
	MotorDir(FWD, FWD);
	char zero_pressed = 0;
	char one_pressed = 0;
	char two_pressed = 0;
	char three_pressed = 0;
	char four_pressed = 0;
	char five_pressed = 0;
	
	
	unsigned int data[2];
	LineData(data);
	unsigned int white = (data[0] + data[1])/2 - 50; 
	StatusLED(0);
	msleep(3000);
	unsigned int black = (data[0] + data[1])/2 + 100;
	/**int i = 0;
	int linienEnde = 0;*//
	StatusLED(1);
	msleep(100);
	int linie = 1;
	
	while(1){
		LineData(data);
		char switches = PollSwitch() & PollSwitch();
		switches &= 0b00111111;
		zero_pressed = (switches & SWITCH(0));
		one_pressed = (switches & SWITCH(1));
		two_pressed = (switches & SWITCH(2));
		three_pressed = (switches & SWITCH(3));
		four_pressed = (switches & SWITCH(4));
		five_pressed = (switches & SWITCH(5));
		
		if(data[0] < data[1]){
			MotorSpeed(0, 100);
			BackLED(1, 0);
		} else if(data[0] > data[1]){
			MotorSpeed(100, 0);
			BackLED(0, 1);
		}else if(zero_pressed || one_pressed || two_pressed || three_pressed || four_pressed || five_pressed){
			MotorDir(RWD, RWD);
			MotorSpeed(70, 100);
			msleep(800);
			
			MotorDir(RWD, RWD);
			MotorSpeed (100, 0);
			BackLED(1, 1);
			msleep(480);
			
			linie = 1;
			
			do{
				if((data[0]+data[1])/2 < black){
					linie = 0;
					msleep(5);
				}
				if(data[0] < white && data[1] < white){
					msleep(5);
					linie = 0;
				}
				MotorDir (FWD, FWD);
				MotorSpeed (65, 130);
				BackLED(0, 0);
				msleep(5);
			}while(linie);
		}
		/**if((data[0]+data[1])/2 > white){
			i++;
		}else{
			i = 0;
		}
		
		if(i == 200){linienEnde = 1;}
		
		while(linienEnde){
			MotorDir(FWD, RWD);
			MotorSpeed(70, 100);
			msleep(5);
			if((data[0]+data[1])/2 < black){
					linienEnde = 0;
					msleep(5);
				}
			if(data[0] < white && data[1] < white){
					msleep(5);
					linienEnde = 0;
			}
		}**/
	}
}
