/**
 * Der ASURO soll möglichst lebendig wirken. Viele Tiere suchen das Licht,
 * da es Wärme oder Orientierung verspricht. So suchen Mücken und wechsel-
 * warme Tiere die Sonne, bzw. künstliche Lampen. Der ASURO ist jedoch die
 * Mücke 2.0, da er, wenn er gegen etwas stößt, dies nicht sofort vergisst,
 * sondern versucht dem Hindernis auszuweichen.
 * Ist der ASURO nah genug am Licht, zeigt er seine Freude durch
 * abwechselndes Blinken der Lampen.
 */

#define GESCHW 100
#define FAULT 200

#include <asuro/asuro.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

inline int lineCorrection(int ldata)
{
	// only use for right line data (in my case)
	float x = ldata / 1024.0;
	return ldata + FAULT * 4 * x * (x - 1);
}

inline void check_stossen() {
	uint8_t tasterstatus01 = PollSwitch()&0b00111111;
	uint8_t tasterstatus02 = PollSwitch()&0b00111111;
	
	uint8_t tasterstatus = tasterstatus01 & tasterstatus02;
	
	if (tasterstatus > 0) {
		MotorDir(BWD,BWD);
		MotorSpeed(128,128);
		msleep(2000);
		MotorDir(FWD,FWD);
		
		int pieps_counter = 0;
		
		while (pieps_counter < 2) {
			MotorSpeed(50,50);
			msleep(500);
			pieps_counter+=1;
		}
		
		msleep(5);
		
		int zufallsrichtung = rand() % 2;
		
		if (zufallsrichtung == 0) {
			MotorSpeed(100,0);
		} else {
			MotorSpeed(0,100);
		}
		msleep(2000);
	}
}

void blinken(int freq) {
	
	int state = 0;
	int blink_haeufigkeit = 6;
	int zaehler = 0;
	
	while (zaehler<blink_haeufigkeit) {

        if (state) {
            BackLED(ON, OFF);
            state = 0;
        } else {
            BackLED(OFF, ON);
            state = 1;
        }
        
        zaehler++;
        msleep(freq);
	}
	
}

void main(void) {
    Init();
    
    float frequency = GESCHW;
    MotorDir(FWD, FWD); // BREAK, FREE, RWD
    
    while(1) {
	
		unsigned int data[2];
		LineData(data);
		data[1] = lineCorrection(data[1]);
		
		if ((data[0]+data[1])/2 > 1000) {
			
			MotorSpeed(0,0);
			
			frequency = (2048/((double)data[LEFT]+(double)data[RIGHT]))*GESCHW;
		
			blinken(frequency);
		
		} else  {
			
			check_stossen();
			
			uint8_t rm_speed = ((double) data[LEFT]/1024*(2048/(data[0]+data[1])))*100+60;
			uint8_t lm_speed = ((double) data[RIGHT]/1024*(2048/(data[0]+data[1])))*100+60;
			
			if (data[0]>data[1]) {
				BackLED(ON,OFF);
				MotorSpeed(85,rm_speed);
			} else {
				BackLED(OFF,ON);
				MotorSpeed(lm_speed,85);
			}
			
			msleep(5);
		
		}
		
	}
}
