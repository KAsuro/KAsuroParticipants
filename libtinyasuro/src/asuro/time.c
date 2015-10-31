/**
 * @file
 *
 *\if de
 * Implementiert die in time.h spezifizierte Funktionalität.
 * \endif
 *
 * \if en
 * Implements the functionality specified in time.h
 * \endif
 *
 * @author Markus Jung
 *
 * @version 17.05.2010 \n
 *  Grundfunktionalität aus asuro.c mit folgenden Zusätzen reimplementiert: \n
 *  @li Umstellung auf 36kHz-Timerfrequenz
 *  @li Möglichkeit zum Ein/Ausschalten des 36kHz-Ausgabesignals
 *  @li Powerdown für sleep
 *
 *  msleep implementiert
 * @version 02.08.2010 \n
 *  sleepUntilInterrupt implementiert
 * @version 16.08.2010 \n
 *  + Hook für Timer2ISR
 * @version 13.09.2013 \n
 *  English documentation
 * @version 02.09.2013 \n
 *  More detailed comments regarding the timer operation
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

#include <stddef.h>
#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>

#include <asuro/time.h>

volatile uint8_t tick36kHz;

#if TIMER2_ISR_HOOK_ENABLE
    ISRHookFunc_t *Timer2ISRHook;
#endif

void Timer2Init(void) {
    // A 36kHz square wave signal matches F_CPU / 222
    // Operation mode: Fast PWM, range 0 - 221 (T = 222)
    // 50% duty cycle square wave -> Compare match at TCNT2 = T/2 = 111
    // Manually added offset: 256 - 222 = 34 cycles

    ASSR = 0x00; // synchronous operation
    TCCR2 = (1 << CS20); // T2_CLK: F_CPU
    TIMSK |= (1 << TOIE2); // overflow interrupt
    OCR2 = 111 + 34; // compare match at T/2 + offset
    TIMER2_CLOCK_OUT_DISABLE();

    // configure PB3 (36kHz carrier output) as output, low
    DDRB |= (1 << DDB3);
    PORTB &= ~(1 << PB3);
}

/*
 * Modified timer operation, pre-initialized with a value of 34.
 * Effective timer frequency: f = F_CPU / (256 - 34) = 36,036 kHz
 * Equivalent TOP value: 222 (0xDE)
 *
 * Updating the TCNT2 register requires three clock cycles/instructions, which
 * results in a total offset value of +37.
 */
ISR(TIMER2_OVF_vect)
{
    uint8_t temp_reg;

    __asm volatile( // TCNT2 += 37;
            "in %[SUB_REG], %[TIMER_REG]            \n\t"
            "subi %[SUB_REG], %[OFFSET_2COMP]       \n\t"
            "out %[TIMER_REG], %[SUB_REG]"
            : /* out operands */
              [SUB_REG] "=&d" (temp_reg)
            : /* in operands */
              [TIMER_REG] "I" (_SFR_IO_ADDR(TCNT2)),
              [OFFSET_2COMP] "i" (-37)
    );

    tick36kHz++;

#if TIMER2_ISR_HOOK_ENABLE
    if (Timer2ISRHook != NULL) {
        Timer2ISRHook();
    }
#endif
}

void sleep(uint8_t timer36kHz_ticks) {
    // Interrupts have to be enabled to wake up the microcontroller from sleep
    // mode, which legitimates the usage of cli/sei in this case
    cli();
    set_sleep_mode(SLEEP_MODE_IDLE | _SLEEP_ENABLE_MASK); // prepare/unlock
    sei();

    timer36kHz_ticks += tick36kHz;
    while (tick36kHz != timer36kHz_ticks) {
        sleep_cpu();
    }

    cli();
    sleep_disable();
    sei();
}

void sleepUntilInterrupt(void) {
    cli();
    set_sleep_mode(SLEEP_MODE_IDLE | _SLEEP_ENABLE_MASK); // prepare/unlock
    sei();

    sleep_cpu();

    cli();
    sleep_disable();
    sei();
}
