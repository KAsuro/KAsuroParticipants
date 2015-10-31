/**
 * @file
 *
 * \if de
 * Ein einfaches Testprojekt das den Tasterzustand alle 500 ms einliest und 
 * über die UART/IR-Schnittstelle ausgibt.
 * \endif
 *
 * \if en
 * Simple test project which polls all switches every 500 ms and prints their
 * state to the UART/IR-interface.
 * \endif 
 *
 * @author Markus Jung
 *
 * @version 14.12.2011 \n
 *  First Version
 */
#include <stdint.h>
#include <asuro/asuro.h>
#include <util/misc.h>

MAIN void main(void) {
    Init();
    
    SerWrite("Switch Test\n", 12);
    
    for (;;) {
    	uint8_t swState = PollSwitch();
    	char asBits[12]; // 0bXXXXXXXX\r\n
    	uint8_t i = 0;
    	
    	asBits[i++] = '0';
    	asBits[i++] = 'b';
		for (; i < 2 + 8; i++) {
			asBits[i] = ((swState & 0x80) != 0) ? '1' : '0';
			swState <<= 1;
		}
		asBits[i++] = '\r';
		asBits[i++] = '\n';
    	
    	SerWrite(asBits, sizeof(asBits));
    	
		msleep(500);
    };
}
