/**
 * @file
 *
 * \if de
 * Definiert die Schnittstelle für die Verwendung des Timer2 als gemeinsame
 * Zeitbasis und Taktquelle.
 *
 * Es werden Funktionen definiert, die es ermöglichen, den Prozessor für einen
 * definierten Zeitraum oder bis zum nächsten Interrupt in den Idle-Mode zu
 * versetzen (unter Verwendung der Funktionalität aus avr/sleep.h).
 *
 * Außerdem wird die Möglichkeit geboten, die Taktausgabe für die
 * IR-Schnittstelle ein- und auszuschalten.
 *
 * Zusätzlich besteht die Möglichkeit, eine eigene Funktion in der mit 36kHz
 * aufgerufenen Timer2-ISR auszuführen, sofern diese Option in hooks.h aktiviert
 * ist.
 *
 * @par Wichtiger Hinweis:
 *      Für eine korrekte Funktion des Moduls müssen Interrupts global
 *      freigeschaltet sein, Ausnahmen sind für kurze Zeitspannen zulässig,
 *      können aber die Genauigkeit negativ beeinflussen.
 *
 * @par Initialisierung:
 *      Vor der Verwendung des Moduls muss Timer2Init() aufgerufen werden, zudem
 *      wird eine globale Interruptfreigabe benötigt.
 *
 * @par Genutzte Hardwareressourcen:
 *      Module:
 *      @li Timer2
 *
 *      Interruptvektoren:
 *      @li TIMER2_OVF
 *
 *      I/O:
 *      @li Port B: Pin 3 (36kHz-Träger der IR-Schnittstelle)
 * \endif
 *
 * \if en
 * Interface definitions to use timer2 as common clock source and time
 * reference.
 *
 * This interface defines functions to put the microcontroller into idle mode
 * for either a defined timespan or until the next interrupt occurs. The
 * functionality in avr/sleep.h is used for this purpose.
 *
 * The carrier clock for the IR communications interface can be controlled
 * using this interface.
 *
 * Timer2 is operating with a frequency of 36kHz, custom code can be executed
 * by an ISR of timer2 if this feature is enabled in hooks.h.
 *
 * @par Important:
 *      This module requires interrupts to be enabled globally. Short term
 *      exceptions may be permittable but can have a negative effect on the
 *      accuracy of the provided clock.
 *
 * @par Initialization:
 *      Timer2Init() must be called before using this module. Interrupts must be
 *      enabled globally.
 *
 * @par Hardware resources in use:
 *      Components:
 *      @li Timer2
 *
 *      Interrupt vectors:
 *      @li TIMER2_OVF
 *
 *      I/O:
 *      @li Port B: Pin 3 (36kHz carrier signal for IR communications)
 * \endif
 *
 * @see hooks.h
 *
 * @author Markus Jung
 *
 * @version 23.05.2010 \n
 *  Schnittstellendefinition entsprechend asuro.h
 * @version 28.05.2010 \n
 *  Zählervariable zugänglich gemacht
 * @version 02.06.2010 \n
 *  sleepUntilInterrupt implementiert
 * @version 15.06.2010 \n
 *  Doku
 * @version 26.06.2010 \n
 *  Alias-Makros für sleep, msleep und sleepUntilInterrupt hinzugefügt
 *  (zwecks konsistentem Namensschema)
 * @version 16.08.2010 \n
 *  + Hook für Timer2ISR
 * @version 13.09.2013 \n
 *  English documentation
 * @version 29.10.2013 \n
 *  Document interrupt safety issues of TIMER2_CLOCK_OUT_ENABLE() and
 *  TIMER2_CLOCK_OUT_DISABLE()
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

#ifndef TIME_H_
#define TIME_H_

#include <stdbool.h>
#include <stdint.h>
#include "hooks.h"
#include "../util/misc.h"

#ifdef __DOXYGEN__
/** \if de
 * Alias-Makro für sleep().
 * \endif
 *
 * \if en
 * Alias macro for sleep().
 * \endif
 */
#define Sleep
/** \if de
 * Alias-Makro für msleep().
 * \endif
 *
 * \if en
 * Alias macro for msleep().
 * \endif
 */
#define Msleep
/** \if de
 * Alias-Makro für sleepUntilInterrupt().
 * \endif
 *
 * \if en
 * Alias macro for sleepUntilInterrupt().
 * \endif
 */
#define SleepUntilInterrupt
#else
#define Sleep               sleep
#define Msleep              msleep
#define SleepUntilInterrupt sleepUntilInterrupt
#endif

/**
 * \if de
 * Eine Zählvariable die im 36kHz-Takt (um eins) inkrementiert wird.
 * \endif
 *
 * \if en
 * A counting variable incremented from the 36kHz clock.
 * \endif
 */
extern volatile uint8_t tick36kHz; // counts from 0 to 255

#ifdef __DOXYGEN__
/**
 * \if de
 * Bietet die Möglichkeit, eine eigene Funktion bei jedem Aufruf der Timer-ISR
 * auszuführen. Die interne Funktionalität bleibt dabei erhalten.
 *
 * Da die Timer-ISR mit einer Frequenz von 36kHz aufgerufen wird, sollte diese
 * Funktion möglichst kurz sein.
 *
 * Der Aufruf erfolgt aus dem Kontext eines Interrupts!
 *
 * Ist (Timer2ISRHook == NULL), findet kein Aufruf statt.
 *
 * Diese Funktionalität steht nicht zur Verfügung, wenn TIMER2_ISR_HOOK_ENABLE
 * deaktiviert ist.
 * \endif
 *
 * \if en
 * Offers the ability to execute a custom function in addition to the internal
 * code of the timer ISR.
 *
 * Since the ISR will be executed with a frequency of 36kHz, the called function
 * should be as short as possible.
 *
 * The function will be called from an interrupt context!
 *
 * No function call will be executed if (Timer2ISRHook == NULL).
 *
 * The timer hook feature is not available if TIMER2_ISR_HOOK_ENABLE has been
 * disabled.
 * \endif
 */
extern ISRHookFunc_t *Timer2ISRHook;
#else  // !__DOXYGEN__
#if TIMER2_ISR_HOOK_ENABLE
extern ISRHookFunc_t *Timer2ISRHook;
#endif
#endif

/**
 * \if de
 * Initialisiert das Modul und den als Zeitbasis und Taktgenerator für die
 * Infrarotschnittstelle verwendeten Timer2.
 *
 * Nach der Initialisierung ist die Taktausgabe für die Infrarotschnittstelle
 * deaktiviert.
 * \endif
 *
 * \if en
 * Initializes the module and timer2 which is used as time reference and clock
 * source for the IR communications interface.
 *
 * The clock output for the IR interface will be in a disabled state after the
 * initialization has been completed.
 * \endif
 */
extern void Timer2Init(void);

/**
 * \if de
 * Aktiviert die Ausgabe des 36kHz-Signals für die Infrarotschnittstelle.
 *
 * @par Wichtiger Hinweis:
 * Dieses Makro ist nur eingeschänkt interruptfest. Es darf nicht "parallel" zu
 * TIMER2_CLOCK_OUT_DISABLE() ausgeführt werden.
 * \endif
 *
 * \if en
 * Enables the 36kHz carrier signal for the IR communications interface.
 *
 * @par Important:
 * This macro is interrupt-safe except for one restriction. It must not be
 * executed in parallel to TIMER2_CLOCK_OUT_DISABLE().
 * \endif
 */
#ifdef __DOXYGEN__
#define TIMER2_CLOCK_OUT_ENABLE()
#else
#define TIMER2_CLOCK_OUT_ENABLE() \
    do { /* Fast PWM, BOTTOM: Clear, CompMatch: Set */ \
        TCCR2 |= (1 << WGM20) | (1 << WGM21) | (1 << COM20) | (1 << COM21); \
    } while (false)
#endif

/**
 * \if de
 * Deaktiviert die Ausgabe des 36kHz-Signals für die Infrarotschnittstelle.
 *
 * @par Wichtiger Hinweis:
 * Dieses Makro ist nur eingeschränkt interruptfest. Es darf nicht "parallel"
 * zu TIMER2_CLOCK_OUT_ENABLE() ausgeführt werden.
 * \endif
 *
 * \if en
 * Disables the 36kHz carrier signal for the IR communications interface.
 *
 * @par Important:
 * This macro is interrupt-safe except for one restriction. It must not be
 * executed in parallel to TIMER2_CLOCK_OUT_ENABLE().
 * \endif
 */
#ifdef __DOXYGEN__
#define TIMER2_CLOCK_OUT_DISABLE()
#else
#define TIMER2_CLOCK_OUT_DISABLE() \
    do { /* PWM off */ \
        TCCR2 &= ~((1 << WGM20) | (1 << WGM21) | (1 << COM20) | (1 << COM21)); \
    } while (false)
#endif

/**
 * \if de
 * Hält den Programmfluss für die vorgegebene Anzahl 36kHz-Takte an und schickt
 * den Prozessor währendessen in den Idle-Mode.
 *
 * Alle Interrupts bleiben aktiv und werden ausgeführt, ledliglich der
 * aufrufende Programmpfad wird unterbrochen.
 *
 * Wenn diese Funktion aufgerufen wird, geht sie davon aus, dass Interrupts
 * global freigeschaltet sind - wenn nicht findet die globale Interruptfreigabe
 * vor Eintritt in den Idle-Mode statt.
 *
 * @param timer36kHz_ticks die Anzahl der 36kHz-Takte die der Prozessor
 *                         "schlafen" soll
 * \endif
 *
 * \if en
 * Delays the execution for the specified amount of time, measured in 36kHz
 * ticks. The microcontroller will halted (idle mode) during this time.
 *
 * All interrupt requests remain active and will be executed, the calling
 * function itself remains halted.
 *
 * This function expects interrupts to be globally enabled if it gets called.
 * If not, the global interrupt enable bit will be set before entering the sleep
 * mode and remain in this state after the function returns.
 *
 * @param timer36kHz_ticks the number of 36kHz ticks the microcontroller will
 *                         be halted.
 * \endif
 */
extern void sleep(uint8_t timer36kHz_ticks);
/**
 * \if de
 * Schickt den Prozessor in den Idle-Mode und kehrt zurück, nachdem ein
 * Interrupt ausgelöst wurde.
 *
 * Wenn diese Funktion aufgerufen wird, geht sie davon aus, dass Interrupts
 * global freigeschaltet sind - wenn nicht findet die globale Interruptfreigabe
 * vor Eintritt in den Idle-Mode statt.
 * \endif
 *
 * \if en
 * Puts the microcontroller into idle mode until the next interrupt occurs.
 *
 * This function expects interrupts to be globally enabled if it gets called.
 * If not, the global interrupt enable bit will be set before entering the sleep
 * mode and remain in this state after the function returns.
 * \endif
 */
extern void sleepUntilInterrupt(void);

/**
 * \if de
 * Verhält sich wie sleep(), verwendet als Zeitbasis aber Millisekunden
 *
 * @param ms die Anzahl der Millisekunden die der Prozessor "schlafend"
 *           verbringen soll
 * \endif
 *
 * \if en
 * Behaves like sleep() but uses milliseconds as timebase.
 *
 * @param ms the time the microcontroller will be halted, measured in
 *           milliseconds
 * \endif
 */
#ifdef __DOXYGEN__
void msleep(uint16_t ms);
#else
static ALWAYS_INLINE void msleep(uint16_t ms);
#endif

void msleep(uint16_t ms) {
    for (; ms > 0; ms--) {
        sleep(36);
    }
}

#endif /* TIME_H_ */
