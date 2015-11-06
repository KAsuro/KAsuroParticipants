/**
 * This project is contributed under BEER-Licence
 * 
 * Klingonian:
 * jInmol linienfolger. puS latlh je features such as singing pagh vabDot epilepsy Qap tu'lu'.
 * 
 * Maori:
 * Tenei kaupapa ko ha tokotaha muimui raina. He maha ngā āhuatanga rerehua pērā i te waiata mahi haurangi ranei.
 * 
 * Zulu:
 * Le phrojekthi ungumlandeli line. Kunezinhlobo eziningana izici fancy ezifana ukucula noma umsebenzi sokuwa.
 */
#include <asuro/asuro.h>
#include <avr/interrupt.h>

#define SWITCH(X) 1<<X
#define FAST 200
#define SLOW 80

#define FS 31250 // controlled by timer2 8Mhz/256

#define A4		440
#define B4		466
#define H4		493
#define C5		523
#define DES5	554
#define D5		587
#define ES5		622
#define E5		659
#define F5		698
#define GES5	740
#define G5		783
#define AS5		830
#define A5		880
#define B5		932
#define H5		987
#define C6		1046
#define DES6	1108
#define D6		1174
#define ES6 	1244
#define E6		1318
#define F6		1396
#define GES6	1479
#define G6		1567
#define AS6		1661
#define A6		1760
#define B6		1864
#define H6		1975
#define C7		2093
#define DES7	2217
#define D7		2349
#define ES7		2489
#define E7		2637
#define F7		2793
#define GES7	2959
#define G7		3135
#define AS7		3322
#define A7		3520
#define H7		3729
#define B7		3951

#define HALF 300
#define FULL 600
#define _OFF 0
#define STOP 0

uint16_t duck_melody[]={
	C5,HALF,
	D5,HALF,
	E5,HALF,
	F5,HALF,
	G5,FULL,
	_OFF,HALF,
	G5,FULL,
	_OFF,HALF,
	A5,HALF,
	_OFF,HALF,
	A5,HALF,
	_OFF,HALF,
	A5,HALF,
	_OFF,HALF,
	A5,HALF,
	_OFF,HALF,
	G5,FULL,
	_OFF,HALF,
	A5,HALF,
	_OFF,HALF,
	A5,HALF,
	_OFF,HALF,
	A5,HALF,
	_OFF,HALF,
	A5,HALF,
	_OFF,HALF,
	G5,FULL,
	_OFF,HALF,

	F5,HALF,
	_OFF,HALF,
	F5,HALF,
	_OFF,HALF,
	F5,HALF,
	_OFF,HALF,
	F5,HALF,
	_OFF,HALF,
	E5,FULL,
	_OFF,HALF,
	E5,FULL,
	_OFF,HALF,

	D5,HALF,
	_OFF,HALF,
	D5,HALF,
	_OFF,HALF,
	D5,HALF,
	_OFF,HALF,
	D5,HALF,
	_OFF,HALF,

	C5,FULL,
	_OFF,HALF,

	STOP,STOP
};


void main(void) {
    Init();
    
    FrontLED(ON);
    int i=0;
    int linienEnde = 0;
    int fast = 200;
    
    unsigned int stData = 0, schwarz = 0;
    //Weißwert festlegen
    unsigned int data[2];
    LineData(data);
    stData = data[LEFT] + data[RIGHT] + 50;
    
    StatusLED(GREEN);
    msleep(3000);
    StatusLED(YELLOW);
    
    //Schwarzwert setzen
    LineData(data);
    schwarz = data[LEFT] + data[RIGHT];
    
    msleep(1000);
    StatusLED(GREEN);
    
    
    MotorSpeed(fast,fast);
    StatusLED(YELLOW);
    
    while(1){
		
		
		LineData(data);
		
		if(data[LEFT] > data[RIGHT]){
			MotorDir(FWD,FREE);
			BackLED(ON,OFF);
			
		}else if(data[RIGHT] > data[LEFT]){
			MotorDir(FREE,FWD);
			BackLED(OFF,ON); ;
		}
		
		if(data[LEFT]+data[RIGHT]>stData){
			StatusLED(RED);
			i++;
		}else{
			i=0;
			StatusLED(YELLOW);
		}
		
		if(i>100){
			linienEnde = 1;
		}
		
		while(linienEnde){
			MotorDir(FWD,RWD);
			LineData(data);
			
			if(data[LEFT]+data[RIGHT] < schwarz+100){
				MotorDir(BREAK,BREAK);
				msleep(50);
				
				while(linienEnde){
					MotorSpeed(80,80);
					MotorDir(RWD,FWD);
					LineData(data);
					
					if(data[LEFT] + data[RIGHT] < schwarz+100){
							linienEnde = 0;
					}
				}
				MotorSpeed(fast,fast);
			}
		}
		
		Kollision();
	}
}

void Kollision(void){
	char switches = '0';
	char zero = '0';
    char one = '0';
    char two = '0';
    char three = '0';
    char four = '0';
    char five = '0';
    
	switches = PollSwitch() & PollSwitch() & 0b00111111;
	
	zero = switches & SWITCH(1);
	one = switches & SWITCH(2);
	two = switches & SWITCH(1);
	three = switches & SWITCH(2);
	four = switches & SWITCH(1);
	five = switches & SWITCH(2);

	if(zero || one || two || three || four || five){
		MotorSpeed(0,0);
		StereoSound(duck_melody,duck_melody);
		Lichtorgel();
	}
}

void Lichtorgel(void){
	MotorDir(FWD,BWD);
	MotorSpeed(255,255);
	for(int q = 0; q < 15; q++){
		StatusLED(GREEN);
		msleep(50);
		BackLED(ON,OFF);
		msleep(50);
		StatusLED(RED);
		msleep(50);
		BackLED(OFF,ON);
		msleep(50);
		StatusLED(YELLOW);
		msleep(50);
		FrontLED(ON);
		msleep(50);
		FrontLED(OFF);
	}
	
	MotorDir(BREAK,BREAK);
	MotorSpeed(0,0);
	
	while(1){
		StatusLED(GREEN);
		msleep(50);
		BackLED(ON,OFF);
		msleep(50);
		StatusLED(RED);
		msleep(50);
		BackLED(OFF,ON);
		msleep(50);
		StatusLED(YELLOW);
		msleep(50);
		FrontLED(ON);
		msleep(50);
		FrontLED(OFF);
	}
}

void StereoSound(uint16_t *noten1, uint16_t *noten2)
{
	uint16_t index1=0,index2=0;
	uint16_t timer1,timer2;
	uint16_t phase1,phase2,angle1,angle2;
	uint8_t dir1=FWD,dir2=FWD;
	uint8_t speed1,speed2;
	
	FrontLED(OFF);
	BackLED(OFF,OFF);
	StatusLED(OFF);
	
	angle1=(uint32_t)noten1[index1++]*65536/FS;
	timer1=noten1[index1++]*(FS/1000);
	if(angle1==0)speed1=0;
	else speed1=255;

	angle2=(uint32_t)noten2[index2++]*65536/FS;
	timer2=noten2[index2++]*(FS/1000);
	if(angle2==0)speed2=0;
	else speed2=255;
	MotorSpeed(speed1,speed2);
	
	cli(); // stop all interrupts
	while(timer1!=0)
	{
		timer1--;
		timer2--;

		if(timer1==0) 
		{
			angle1=(uint32_t)noten1[index1++]*65536/FS;
			timer1=noten1[index1++]*(FS/1000);
			if(angle1==0)speed1=0;
			else speed1=255;
					
			MotorSpeed(speed1,speed2);
		}
		
		if(timer2==0) 
		{
			angle2=(uint32_t)noten2[index2++]*65536/FS;
			timer2=noten2[index2++]*(FS/1000);
			if(angle2==0)speed2=0;
			else speed2=255;			
							
			MotorSpeed(speed1,speed2);
		}
		phase1+=angle1; 
		
		if(phase1&0x8000) dir1=FWD;
		else dir1=RWD;
		
		phase2+=angle2; 
		
		if(phase2&0x8000) dir2=FWD;
		else dir2=RWD;
		MotorDir(dir1,dir2);
		
		// sync with Timer2 ( 31250Hz )
		while(!(TIFR&(1<<TOV2)));
		TIFR|=TIFR&(1<<TOV2);
	}
	MotorSpeed(0,0); 
	sei(); // enable all interrupts
}

