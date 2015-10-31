/**
 * @file
 *
 * \if de
 * Implementiert die in motor.h spezifizierte Funktionalität.
 * \endif
 *
 * \if en
 * Implements the functionality specified in motor.h
 * \endif
 *
 * @author Markus Jung
 *
 * @version 17.05.2010 \n
 *  Funktionen aus asuro.c verbessert implementiert
 * @version 31.05.2010 \n
 *  Interruptfest gemacht
 * @version 15.06.2010 \n
 *  Interne Makros aus dem Header ins Sourcefile verschoben
 * @version 03.10.2010 \n
 *  Verwendung der Makros RIGHT_DIR und LEFT_DIR auch als Masken in MotorDir()
 * @version 17.09.2013 \n
 *  English translation
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

#include <util/atomic.h>

#include <asuro/motor.h>

#ifdef __DOXYGEN__
/**
 * \if de
 * PWM-Ausgänge zur Geschwindigkeitsregelung
 * \endif
 *
 * \if en
 * PWM outputs used to control the driving speed.
 * \endif
 */
#define PWM
/**
 * \if de
 * Ausgänge zur Richtungsauswahl rechts
 * \endif
 *
 * \if en
 * Outputs used to configure the direction of the right motor
 * \endif
 */
#define RIGHT_DIR
/**
 * \if de
 * Ausgänge zur Richtungsauswahl links
 * \endif
 *
 * \if en
 * Outputs used to configure the direction of the left motor
 * \endif
 */
#define LEFT_DIR
#else // !__DOXYGEN__
#define PWM       ((1 << PB1) | (1 << PB2))
#define RIGHT_DIR ((1 << PB4) | (1 << PB5))
#define LEFT_DIR  ((1 << PD4) | (1 << PD5))
#endif

void MotorInit(void) {
    DDRB |= LEFT_DIR | PWM;
    DDRD |= RIGHT_DIR;

    // 8 bit PWM using OC1A & OC1B
    TCCR1A = (1 << WGM10) | (1 << COM1A1) | (1 << COM1B1);
    // Timer1 CLK: F_CPU/8
    TCCR1B = (1 << CS11);
    // Turn off all interrupts of Timer1
    TIMSK &= ~((1 << TICIE1) | (1 << OCIE1A) | (1 << OCIE1B) | (1 << TOIE1));

    MotorDir(FWD,FWD);
    MotorSpeed(0,0);
}

void MotorDir(const uint8_t left_dir, const uint8_t right_dir)
{
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        PORTD = (PORTD & ~LEFT_DIR) | left_dir;
        PORTB = (PORTB & ~RIGHT_DIR) | right_dir;
    }
}

void MotorSpeed(const uint8_t left_speed, const uint8_t right_speed) {
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        OCR1AH = 0; // highbyte = 0, used for both write operations
        OCR1AL = left_speed;
        OCR1BL = right_speed;
    }
}
