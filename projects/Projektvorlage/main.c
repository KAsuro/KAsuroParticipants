/**
 * Beschreibt hier was euer Programm macht!
 */

#define SWITCH(X) 1<<X
#define SLOW 1000
#define FAST 100

#include <asuro/asuro.h>

void main(void) {
    Init();

    int state = 0;
    int interval = FAST;
    char a_pressed = 0;
    char b_pressed = 0;
    
    while (1) {
        char switches = PollSwitch();
        switches &= 0b00111111;

        if (state) {
            BackLED(ON, OFF);
            state = 0;
        } else {
            BackLED(OFF, ON);
            state = 1;
        }

        a_pressed = ( switches & SWITCH(0) );
        b_pressed = ( switches & SWITCH(5) );

        if (a_pressed > 0) {
            interval = SLOW;
        }
        if (b_pressed > 0) {
            interval = FAST;
        }
        msleep(interval);
    }
}
