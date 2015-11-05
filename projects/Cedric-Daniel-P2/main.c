/**
 * Beschreibt hier was euer Programm macht!
 */

#define SWITCH(X) 1<<X
#define SLOW 500
#define FAST 100
#define TRUE 1
#define FALSE 0
#define SPEED_FAST 100
#define SPEED_SLOW 90

#include <asuro/asuro.h>

int speed_r = 0;
int speed_l = 0;
int dir_r = FWD;
int dir_l = FWD;
char mode = 'h'; // h: halt
		// w: forward
		// s: backward
		// a: left
		// d: right

void fwd();
void bwn();
void lft();
void rgt();
void stop_halt();

void main(void) {
    Init();


    while (1) {

	MotorSpeed(speed_l, speed_r);
	MotorDir(dir_l, dir_r);
        
	char switches = PollSwitch();
	switches &= 0b00111111;

	char receivedInfo[1];
	receivedInfo[0] = '0';

	SerRead(receivedInfo, 1, 0);
	switch(receivedInfo[0]){
		case 'w':
			fwd();
			break;
		case 's':
			bwd();
			break;
		case 'a':
			lft();
			break;
		case 'd':
			rgt();
			break;
		case 'h':
			stop_halt();
//			break;
        }




	StatusLED(RED);
	//SerWriteInt((int)switch_0);
	char switch_l = ( ( switches & SWITCH(0) ) || (switches & SWITCH(1)) || (switches & SWITCH(2)) );
	char switch_r = ( ( switches & SWITCH(3) ) || (switches & SWITCH(4)) || (switches & SWITCH(5)) );

	if (switch_l > 0){
		BackLED(OFF, ON);
	}
	if (switch_r > 0){
		BackLED(ON, OFF);
	}
	msleep(10);
        
    }
}

void fwd(){
    if((mode == 'h')||(mode == 'a')||(mode == 'd')){
	speed_r = SPEED_SLOW;
    	speed_l = SPEED_SLOW;
    	dir_r = FWD;
    	dir_l = FWD;
	mode = 'w';
    }else if (mode == 'w'){
	if(speed_r < 220){
	    speed_r += 10;
	    speed_l += 10;
	}
    	dir_r = FWD;
    	dir_l = FWD;
	mode = 'w';
    }else if (mode == 's'){
	stop_halt();
    }
}
void bwd(){
    if((mode == 'h')||(mode == 'a')||(mode == 'd')){
    	speed_r = SPEED_SLOW;
    	speed_l = SPEED_SLOW;
    	dir_r = BWD;
    	dir_l = BWD;
	mode = 's';
    }else if(mode == 's'){
	if(speed_r < 220){
	    speed_r += 10;
	    speed_l += 10;
	}
    	dir_r = BWD;
    	dir_l = BWD;
	mode = 's';
    }else if(mode == 'w'){
	stop_halt();
    }
}
void lft(){
    if(mode != 's'){
        speed_r = SPEED_SLOW;
    	speed_l = 0;
    }else{
    	speed_r = 0;
    	speed_l = SPEED_SLOW;
    }
    mode = 'a';
}
void rgt(){
    if(mode != 's'){
    	speed_r = 0;
    	speed_l = SPEED_SLOW;
    }else{
        speed_r = SPEED_SLOW;
    	speed_l = 0;
    }
    mode = 'd';
}
void stop_halt(){
    speed_r = 0;
    speed_l = 0;
    mode = 'h';
}
