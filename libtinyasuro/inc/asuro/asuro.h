/**
 * @file
 *
 * \if de
 * Bindet die Header-Files aller wichtigen Module ein und definiert eine
 * gemeinsame Initialisierungsroutine.
 * \endif
 *
 * \if en
 * Includes the headers of all important modules and defines a common
 * initialization routine.
 * \endif
 *
 * @author Markus Jung
 *
 * @version 13.06.2010 \n
 *  Aus Kompatibilitäts- und Bequemlichkeitsgründen angelegt, bindet alle
 *  anderen wichtigen Header-Files ein.
 * @version 14.06.2010 \n
 *  Doku
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

#ifndef ASURO_H
#define ASURO_H

#include "adc.h"
#include "config.h"
#include "encoder.h"
#include "led.h"
#include "motor.h"
#include "sensors.h"
#include "switches.h"
#include "time.h"
#include "uart.h"

/**
 * \if de
 * Initialisiert alle wichtigen Module.
 *
 * Initialisiert werden:
 * @li 36kHz-Takt (time.h)
 * @li serielle Schnittstelle (uart.h),
 * @li Antriebssystem (motor.h)
 * @li Status-LEDs (led.h)
 * @li ADC (adc.h)
 * @li Tasterauswertung (switches.h)
 *
 * Außerdem werden Interrupts global aktiviert.
 * \endif
 *
 * \if en
 * Initializes all important modules.
 *
 * To name them:
 * @li 36kHz clock (time.h)
 * @li Serial/infrared communications interface (uart.h)
 * @li Motor control (motor.h)
 * @li Status LEDs (led.h)
 * @li ADC (adc.h)
 * @li Switch readout (switches.h)
 *
 * Additionally, interrupts are enabled globally.
 * \endif
 */
void Init(void);

#endif /* ASURO_H */

