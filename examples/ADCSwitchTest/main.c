/**
 * @file
 *
 * \if de
 * Ein einfaches Testprojekt zur Überprüfung der Tasterwerte. Es gibt alle 
 * 500ms den aktuellen Analogwert der Taster über die UART/IR-Schnittstelle 
 * aus. \n
 * Vergleichswerte gibt es im AsuroWiki unter:
 * http://www.asurowiki.de/pmwiki/pmwiki.php/Main/Tasten
 * \endif
 *
 * \if en
 * Simple test project for switch diagnostic purposes. It prints the current
 * analog value of the switches to the UART/IR-interface. \n
 * Values for comparisons can be found in the AsuroWiki at:
 * http://www.asurowiki.de/pmwiki/pmwiki.php/Main/Tasten
 * \endif 
 *
 * @author Markus Jung
 *
 * @version 16.12.2011 \n
 *  First Version
 */
#include <stdint.h>
#include <asuro/asuro.h>
#include <util/misc.h>

MAIN void main(void) {
    Init();
    
    PORTD |= (1 << PD3);
    DDRD |= (1 << DDD3);
    
    SerWrite("Switch ADC Test\n", 16);
    
    for (;;) {
    	uint16_t adcVal;
    	
    	ADC_BLOCK {
    		ADCSelectChannel(ADC_SWITCH);
      		ADCMeasure();
      		adcVal = ADC;
    	}
    	
    	char asString[6]; // XXXX\r\n
    	uint8_t i = sizeof(asString) - 1;
    	
		asString[i--] = '\r';
		asString[i--] = '\n';
		
		do {
			asString[i] = '0' + (adcVal % 10);
			adcVal = adcVal / 10;
		} while (i-- != 0);
    	
    	SerWrite(asString, sizeof(asString));
    	
		msleep(500);
    };
}
