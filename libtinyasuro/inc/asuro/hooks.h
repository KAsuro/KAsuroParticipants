/**
 * @file
 *
 * \if de
 * Definiert und konfiguriert die Möglickeit, beim Aufruf interner
 * Interrupthandler zusätzliche Funktionen (Hooks) auszuführen. Diese
 * zusätzliche Funktionen werden über Funktionszeiger an die jeweiligen Module
 * übergeben.
 *
 * WICHTIG: Die hier gewählte Konfiguration wird fest einkompiliert, eine
 * nachträgliche Änderung ist bei der Verwendung einer vorkompilierten
 * Bibliothek ist nicht möglich!
 *
 * Innerhalb folgender Interrupts können zusätzliche Funktionen ausgeführt werden:
 * @li ADC-Interrupt, verwendet im AutoMeasurmentMode (sensors.h)
 * @li Switch-Interrupt, Tasterereignisse (switches.h)
 * @li Timer2-Interrupt, 36kHz-Taktquelle (time.h)
 * \endif
 *
 * \if en
 * Configures the interrupt hook feature offered by some modules. Interrupt
 * hooks provide the ability to call custom functions during execution of
 * internally used ISRs, by setting up the function pointer for this hook to
 * point to the desired function.
 *
 * IMPORTANT: The chosen configuration will be compiled into the resulting
 * object/binary file. If the tiny ASURO library is used as objectcode library,
 * the configuration will be part of the library binary and can only be changed
 * by recompiling the library.
 *
 * The interrupt handlers listet below support hooking:
 * @li ADC ISR, used by the AutoMeasurmentMode (sensors.h)
 * @li Switch ISR, triggers on bumper switch events (switches.h)
 * @li Timer2 ISR, 36kHz clock (time.h)
 * \endif
 *
 * @see sensors.h
 * @see switches.h
 * @see time.h
 *
 * @author Markus Jung
 *
 * @version 15.08.2010 \n
 *  Angelegt
 * @version 19.09.2013 \n
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

#ifndef HOOKS_H_
#define HOOKS_H_

#include <stdbool.h>

/**
 * \if de
 * Definert einen Datentyp für Hook-Funktionen.
 * \endif
 *
 * \if en
 * Defines a data type for hook functions.
 * \endif
 */
typedef void (ISRHookFunc_t)(void);

/**
 * \if de
 * De-/Aktiviert die Hook-Funktionalität für den Timer2-Interrupt.
 * \endif
 *
 * \if en
 * Enables or disables the hook feature for the timer2 interrupt.
 * \endif
 */
#define TIMER2_ISR_HOOK_ENABLE  false
/**
 * \if de
 * De-/Aktiviert die Hook-Funktionalität für den ADC-Interrupt.
 * \endif
 *
 * \if en
 * Enables or disables the hook feature for the ADC interrupt.
 * \endif
 */
#define ADC_ISR_HOOK_ENABLE     false
/**
 * \if de
 * De-/Aktiviert die Hook-Funktionalität für den Switch-Interrupt.
 * \endif
 *
 * \if en
 * Enables or disables the hook feature for the switch interrupt.
 * \endif
 */
#define SWITCH_ISR_HOOK_ENABLE  false

#endif /* HOOKS_H_ */
