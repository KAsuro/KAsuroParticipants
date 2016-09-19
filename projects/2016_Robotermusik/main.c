/**
 * Beschreibt hier was euer Programm macht!
 */

#define pp 0
#define g 196
#define c1 262
#define a1 440
#define b1 466
#define e1 330

#define c2 523
#define cis2 554
#define d2 587
#define e2 659
#define f2 698
#define fis2 740
#define g2 784
#define a2 880
#define b2 932
#define h2 988
#define c3 1047
#define d3 1175

#define l8 125
#define l16 (l8/2)
#define l4 250
#define l2 (2*l4)
#define l1 (2*l2)
#define p(X) (X+X/2)

#include <asuro/asuro.h>
#include "sound.c"

void tetris(void);
void ente(void);
void stillalive(void);

void main(void) {
	Init();
	//EncoderInit();
	//LED_CONFIGURE_ODOMETRIE();
	//ODOMETRIE_LED_ON();
	
	ente();
	msleep(1000);
	tetris();
	msleep(1000);
	stillalive();
	//msleep(1000);
	//marioIsDead();
	
	//uint16_t white[] = {0, 0};
	//uint16_t data[]  = {0, 0};
	//int turns = 0;
}

void ente(void) {
	
	uint16_t noten[]  = {c2,d2,e2,f2,g2,g2,a2,a2,a2,a2,g2,a2,a2,a2,a2,g2,f2,f2,f2,f2,e2,e2,d2,d2,d2,d2,c2};
	uint16_t laenge[] = {l4,l4,l4,l4,l2,l2,l4,l4,l4,l4,l1,l4,l4,l4,l4,l1,l4,l4,l4,l4,l2,l2,l4,l4,l4,l4,l1};
	uint16_t l = 27;
	
	uint16_t i;
	for (i = 0; i < l; i++) {
		Sound( noten[i], laenge[i], 254 );
	}
}

void tetris(void) {
	
	uint16_t noten[]  = {a2,e2,f2,g2,f2,e2,d2,d2,f2,a2,g2,f2,e2   ,f2,g2,a2,f2,d2,d2,pp,g2   ,b2,d3,c2,b2,a2   ,f2,a2,g2,f2,e2   ,f2,g2,a2,f2,d2,d2};
	uint16_t laenge[] = {l4,l8,l8,l4,l8,l8,l4,l8,l8,l4,l8,l8,p(l4),l8,l4,l4,l4,l4,l4,l2,p(l4),l8,l4,l8,l8,p(l4),l8,l8,l4,l8,p(l4),l8,l4,l4,l4,l4,l2};
	uint16_t l = 37;
	
	uint16_t i;
	for (i = 0; i < l; i++) {
		Sound( noten[i], laenge[i], 254 );
	}
}

void stillalive(void) {
	
	uint16_t noten[]  = {a1,b1, c2,f2,e2,d2,d2,c2, d2,c2,c2,c2,a1,b1, c2,f2,g2,f2,e2,d2, d2,e2,f2,f2,g2,a2, b2,b2,a2,g2,f2,g2, a2,a2,g2,f2,d2,c2, d2,f2,f2,e2,e2,fis2,fis2};
	uint16_t laenge[] = {l8,l8, l4,l4,l8,l8,l8,l8, l8,l8,l4,l4,l8,l8, l4,l4,l8,l8,l8,l8, l8,l8,l4,l4,l8,l8, l8,l8,l4,l4,l8,l8, l8,l8,l4,l4,l8,l8, l8,l8,l8,l8,l4,l8  ,l1  };
	uint16_t l = 45;
	
	uint16_t i;
	for (i = 0; i < l; i++) {
		Sound( noten[i], 2*laenge[i], 254 );
	}
}
