/**
 * @file
 *
 * \if de
 * Implementiert die in led.h spezifizierte Funktionalität.
 * \endif
 *
 * \if en
 * Implements the functionality specified in led.h
 * \endif
 *
 * @author Markus Jung
 *
 * @version 17.05.2010 \n
 *  Implementiert entsprechend der Funktionalität aus asuro.c
 * @version 22.05.2010 \n
 *  Optimierung der einzelnen Funktionen auf Geschwindigkeit und Größe
 * @version 17.12.2011 \n
 *  Odometrie -> Odometry
 * @version 18.09.2013 \n
 *  English translation
 * @version 10.02.2014 \n
 *  Add deprecation warning to BackLED
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

#include <asuro/led.h>
#include <util/misc.h>

void LEDInit(void) {
    DDRB |= GREEN_LED;
    DDRD |= FRONT_LED | ODOMETRY_LED | RED_LED;

    FRONT_LED_OFF();
    LED_CONFIGURE_BACK();
    BackLEDFast(OFF);
    StatusLED(GREEN);
}

void StatusLED(const uint8_t color) {
    GREEN_LED_OFF();
    RED_LED_OFF();

    if (color & GREEN) {
        GREEN_LED_ON();
    }

    if (color & RED) {
        RED_LED_ON();
    }
}

void FrontLED(const uint8_t status) {
    if (status) {
        FRONT_LED_ON();
    } else {
        FRONT_LED_OFF();
    }
}

void BackLEDFast(const uint8_t leds) {
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        if (leds > 0) {
            LED_CONFIGURE_BACK();
        }

        PORTC = (PORTC & ~BACK_LED_BOTH) | leds;
    }
}

DEPRECATED("Please use BackLEDFast instead.")
void BackLED(const uint8_t left, const uint8_t right) {
    BackLEDFast((left ? BACK_LED_LEFT : 0) | (right ? BACK_LED_RIGHT : 0));
}
