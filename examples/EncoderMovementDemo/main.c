/**
 * @file
 *
 * \if de
 * Ein Demonstratiosprojekt das mögliche Anwendungen des encoderkompensierten
 * Bewegungsmodus zeigt. Sowohl EncoderPoll() als auch EncoderMovementPoll
 * werden regelmäßig von einem Interrupt abgefragt. Um konkurrierende Zugriffe
 * zu vermeiden, ist jeder Aufruf von EncoderMovementSetSpeed() in einen
 * atomaren Block gekapselt.
 * \endif
 *
 * \if en
 * A project which demonstrates how to use the encodercompensated
 * movement mode. Both EncoderPoll() and EncoderMovementPoll() are called
 * periodically from an ISR. All calls to EncoderMovementSetSpeed() are
 * encapsulated into atomic blocks to prevent concurrency issues.
 * \endif
 *
 * @author Markus Jung
 *
 * @version 14.09.2010 \n
 *  Angelegt
 * @version 16.12.2011 \n
 *  Implementation and testing
 * @version 17.12.2011 \n
 *  Odometrie -> Odometry
 * @version 02.09.2013 \n
 *  More explanations regarding EncoderMovementPoll()
 * @version 10.02.2014 \n
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

#include <stdbool.h>
#include <avr/interrupt.h>
#include <util/atomic.h>

#include <asuro/asuro.h>
#include <util/misc.h>

#define ASURO_WIDTH 105 // measured between the wheel centers (in mm)
#define INNER_SPEED 100 // speed of the inner wheel, constant
#define OUTER_SPEED(radius) (INNER_SPEED * (1 + (float) ASURO_WIDTH / radius))
// speed of the outer wheel required for a circle with the passed radius (in mm)

static void Timer0IntEnable(void) {
    TIMSK |= (1 << TOIE0);
}

static void Timer0Init(void) {
    // 1kHz-Tick
    TCCR0 = (1 << CS01) | (1 << CS00); // F_CPU/64
}

MAIN void main(void) {
    Init();

    // Enable odometry
    LED_CONFIGURE_ODOMETRY();
    // Configure auto measurment mode
    SensorInit();
    SensorConfigAutomode(am_odo);
    // Setup encoder control
    EncoderInit();
    EncoderMovementReset();
    // Initialize 1kHz tick timer
    Timer0Init();
    Timer0IntEnable();

    // Set drive direction
    MotorDir(FWD, FWD);

    for (;;) {
        // Circle with approx. 10cm diameter
        ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
            EncoderMovementSetSpeed(INNER_SPEED, OUTER_SPEED(100));
        }
        msleep(5000);

        // Circle with approx. 25cm diameter
        ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
            EncoderMovementSetSpeed(INNER_SPEED, OUTER_SPEED(250));
        }
        msleep(3000);

        // Tightening spiral
        for (uint8_t i = 250; i >= 80; i -= 10) {
            ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
                EncoderMovementSetSpeed(INNER_SPEED, OUTER_SPEED(i));
            }
            msleep(500);
        }

        // Straight line
        ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
            EncoderMovementSetSpeed(125, 125);
        }
        msleep(3000);
    }
}

ISR(TIMER0_OVF_vect) {
    static bool ledBlink = false;
    static uint8_t i = 100;

    TCNT0 = 0xFF - 125 + 1; // Creates a 1kHz tick

    EncoderPoll(); // Detects "ticks" from raw ADC values

    if (--i == 0) {
        i = 100;

        // controls motor speed based on detected "ticks"
        // called with a frequency of 10Hz (1kHz / 100), to allow the ASURO
        // to cover some distance before evaluating the encoder data to measure
        // and adjust the driving speed.
        EncoderMovementPoll();

        // Nice blinking status led ;)
        StatusLED(ledBlink ? GREEN : YELLOW);
        ledBlink = !ledBlink;
    }
}
