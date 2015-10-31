/**
 * @file
 *
 * \if de
 * Implementiert die in asuro.h spezifizierte Initialisierungsroutine.
 * \endif
 *
 * \if en
 * Implements the initialization function specified in asuro.h
 * \endif
 *
 * @author Markus Jung
 *
 * @version 13.06.2010 \n
 *  Neue Initialisierungsroutine
 * @version 14.06.2010 \n
 *  Neue Doku
 * @version 23.09.2013 \n
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

#include <avr/interrupt.h>

#include <asuro/asuro.h>

void Init(void) {
    // The serial interface has already been initialized from the bootloader
    // SerInit();

    Timer2Init();
    MotorInit();
    LEDInit();

    ADCInit();
    SwitchInit();

    sei();
}
