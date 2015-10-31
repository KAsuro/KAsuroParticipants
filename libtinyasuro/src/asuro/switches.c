/**
 * @file
 *
 * \if de
 * Implementiert die in switches.h spezifizierte Funktionalität.
 * \endif
 *
 * \if en
 * Implements the functionality specified in switches.h
 * \endif
 *
 * @author Markus Jung
 *
 * @version 18.05.2010 \n
 *  Erste Version aus sensors.c kopiert \n
 *  Interruptbetrieb implementiert
 * @version 24.05.2010 \n
 *  Teilweiser Umbau der Defines
 * @version 31.05.2010 \n
 *  Auf neuen ADC-Betriebsmodus umgestellt
 * @version 08.06.2010 \n
 *  Neue Berechnungsformel für PollSwitch
 * @version 15.06.2010 \n
 *  sleep()-Aufruf in PollSwitch eliminiert \n
 *  Interne Makros aus dem Header ins Sourcefile verschoben \n
 *  switch_event auf reine Flankenerkennung umgebaut, PollSwitch darauf angepasst
 * @version 26.06.2010 \n
 *  PollSwitch versucht nicht mehr, zusätzliche Flanken zu unterdrücken,
 *  dieser Modus wird nicht mehr weiter unterstüzt.
 * @version 16.08.2010 \n
 *  + Hook für SwitchISR
 * @version 22.01.2013 \n
 *  Bugfix: Falsches Makro für SwitchISRHook ersetzt
 * @version 14.09.2013 \n
 *  English translation
 * @version 15.09.2013 \n
 *  Add support for floating point SWITCH_FACTOR values
 * @version 10.02.2014 \n
 *  Extend working range for 16-bit PollSwitch calculation slightly
 */
/*
 * The contents of this file are subject to the terms of the Common Development
 * and Distribution License Version 1.0 (the License).
 * You may not use this file except in compliance with the License.
 *
 * If not included, you can obtain a copy of the License at
 * http://www.sun.com/cddl/cddl.html
 *
 * When distributing Covered Code, include this CDDL Header Notice in each file
 * and include the provided License file.
 */

#include <avr/io.h>
#include <util/atomic.h>

#include <asuro/adc.h>
#include <asuro/config.h>
#include <asuro/switches.h>
#include <util/misc.h>

#define SWITCHES            (1 << PD3)
#define SWITCH_ON()         PORTD |= SWITCHES
#define SWITCH_OFF()        PORTD &= ~SWITCHES
#define SWITCH_OUTPUT()     DDRD |= SWITCHES
#define SWITCH_INPUT()      DDRD &= ~SWITCHES
#define SWITCH_INTF_CLEAR() GIFR &= (1 << INTF1) // clears the flag if set

volatile bool switch_event;

#if SWITCH_ISR_HOOK_ENABLE
    ISRHookFunc_t *SwitchISRHook;
#endif

void SwitchInit(void) {
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        GICR &= ~(1 << INT1); // switch interrupt (INT1) off

        uint8_t mcucr = MCUCR;

        mcucr |= (1 << ISC10); // trigger interrupt on rising or falling edge
        mcucr &= ~(1 << ISC11);

        MCUCR = mcucr;
    }

    DDRC &= ~(1 << DDC4); // configure measurment input to input, low
    PORTC &= (1 << PC4);

    SWITCH_INPUT();
    SWITCH_OFF();
    switch_event = false;
}

uint8_t PollSwitch(void) {
    uint16_t value = 0;

    SWITCH_ON();
    SWITCH_OUTPUT();

    ADC_BLOCK {
        ADCSelectChannel(ADC_SWITCH);

        for (uint8_t i = 0; i < 4; i++) { // 4x oversampling
            ADCMeasure();
            value += ADC;
        }
    }

    SWITCH_INPUT();
    SWITCH_OFF();

    value /= 4; // divide by four, filter out some noise

    /*
     * Formula used by DLR/Jan Grewe, uses slow floating point calculations.
     *
     * return ((unsigned char) (((1024.0 / (float) i - 1.0)) * 63.0 + 0.5));
     */
    /*
     * Formula used by the ASURO library extended by some Roboternetz.de users
     * Two 32 bit divisions, one 32 bit multiplication and two 32 bit additions.
     *
     * return ((10240000UL / (long) value - 10000UL) * SWITCH_FACTOR + 5000UL)
     *         / 10000;
     */

    /*
     * Custom formula, tries to use 16 bit calculations if possible
     * One N bit multiplication, one N bit bitshift (division by two), one N bit
     * division, one N bit addition and one 8 bit addition.
     *
     * Floating point calculations/comparisons are not supported by the C
     * preprocessor for #if conditionals. The implementation relies therefore on
     * the compilers ability to optimize the dead code away.
     */
    if ((uint32_t) (SWITCH_FACTOR * 1024UL) + (1023 / 2) > UINT16_MAX) {
        return ((uint8_t) (((uint32_t) (1024UL * SWITCH_FACTOR) + value / 2) / value))
                - (uint8_t) SWITCH_FACTOR;
    } else {
        return ((uint8_t) (((uint16_t) (1024U * SWITCH_FACTOR) + value / 2) / value))
                - (uint8_t) SWITCH_FACTOR;
    }
}

void SwitchInterruptEnable(void) {
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        SWITCH_INTF_CLEAR(); // clear old interrupt flag
        GICR |= (1 << INT1); // enable interrupt
    }
}

void SwitchInterruptDisable(void) {
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        GICR &= ~(1 << INT1); // disable interrupt
    }
}

ISR(INT1_vect)
{
    switch_event = true;

    // SwitchInterruptDisable();
    // disabled function call to the equivalent function since GCC does not
    // inline it and produces a lot of stack push/pop operations.
    GICR &= ~(1 << INT1); // disable interrupt

#if SWITCH_ISR_HOOK_ENABLE
    if (SwitchISRHook != NULL) {
        SwitchISRHook();
    }
#endif
}
