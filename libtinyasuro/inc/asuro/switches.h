/**
 * @file
 *
 * \if de
 * Definiert die Schnittstelle für die Taster-Auswertung.
 *
 * Neben der Möglichkeit, den Taster-Status via PollSwitch() abzufragen, kann
 * eine interruptbasierte Überwachung der Ereignisse "mindestens ein Taster
 * wurde betätigt" und "alle Taster wurden losgelassen" aktiviert werden. Bei
 * Verwendung dieser Funktion müssen Interrupts global freigeschaltet sein.
 *
 * Zusätzlich besteht die Möglichkeit, eine eigene Funktion in der Switch-ISR
 * (also bei den oben genannten Ereignissen) auszuführen, sofern diese Option in
 * hooks.h aktiviert ist.
 *
 * @par Initialisierung:
 *      Vor der Verwendung des Moduls muss SwitchInit() aufgerufen werden,
 *      ferner muss der ADC (adc.h) initialisiert werden, die interruptbasierte
 *      Tasterüberwachung erfordert ferner eine globale Interruptfreigabe.
 *
 * @par Genutzte Softwaremodule:
 *      @li adc
 *
 * @par Genutzte Hardwareressourcen:
 *      Module:
 *      @li Externer Interrupt 1 (INT1)
 *
 *      Interruptvektoren:
 *      @li INT1
 *
 *      I/O:
 *      @li Port C: Pin 4 (als ADC4)
 *      @li Port D: Pin 3
 * \endif
 *
 * \if en
 * Interface definitions for reading out the state of the bumper switches.
 *
 * In addition to the direct readout function PollSwitch(), this interface also
 * offers an interrupt based observation for two switch events: "at least one
 * switch has been pressed" and "all switches have been released". Interrupts
 * must be globally enabled to use this feature.
 *
 * A custom function can be called by the switch ISR (which will be executed
 * if one of the previously mentioned events occurs), if this functionality has
 * been enabled in hooks.h.
 *
 * @par Initialization:
 *      Prior using this module, SwitchInit() has to be called. The ADC (adc.h)
 *      has to be initalized and to use the switch interrupt observation
 *      feature, interrupts have to be enabled globally.
 *
 * @par Software interfaces in use:
 *      @li adc
 *
 * @par Hardware resources in use:
 *      Components:
 *      @li External interrupt 1 (INT1)
 *
 *      Interrupt vectors:
 *      @li INT1
 *
 *      I/O:
 *      @li Port C: Pin 4 (as ADC4)
 *      @li PORT D: Pin 3
 * \endif
 *
 * @see hooks.h
 *
 * @author Markus Jung
 *
 * @version 18.05.2010 \n
 *  Angelegt \n
 *  Makros entsprechend asuro.h angelegt
 * @version 24.05.2010 \n
 *  Teilweiser Umbau der Defines
 * @version 15.06.2010 \n
 *  Doku \n
 *  Interne Makros aus dem Header ins Sourcefile verschoben
 * @version 26.06.2010 \n
 *  Doku-Anpassung PollSwitch
 * @version 16.08.2010 \n
 *  + Hook für SwitchISR
 * @version 14.09.2013 \n
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

#ifndef SWITCHES_H_
#define SWITCHES_H_

#include <stdbool.h>
#include <stdint.h>
#include "hooks.h"

#ifdef __DOXYGEN__
/**
 * \if de
 * Bitmaske für den Taster 1.
 * \endif
 *
 * \if en
 * Bitmask for bumper switch 1.
 * \endif
 */
#define SWITCH_1
/**
 * \if de
 * Bitmaske für den Taster 2.
 * \endif
 *
 * \if en
 * Bitmask for bumper switch 2.
 * \endif
 */
#define SWITCH_2
/**
 * \if de
 * Bitmaske für den Taster 3.
 * \endif
 *
 * \if en
 * Bitmask for bumper switch 3.
 * \endif
 */
#define SWITCH_3
/**
 * \if de
 * Bitmaske für den Taster 4.
 * \endif
 *
 * \if en
 * Bitmask for bumper switch 4.
 * \endif
 */
#define SWITCH_4
/**
 * \if de
 * Bitmaske für den Taster 5.
 * \endif
 *
 * \if en
 * Bitmask for bumper switch 5.
 * \endif
 */
#define SWITCH_5
/**
 * \if de
 * Bitmaske für den Taster 6.
 * \endif
 *
 * \if en
 * Bitmask for bumper switch 6.
 * \endif
 */
#define SWITCH_6
#else
#define SWITCH_1 (1 << 5)
#define SWITCH_2 (1 << 4)
#define SWITCH_3 (1 << 3)
#define SWITCH_4 (1 << 2)
#define SWITCH_5 (1 << 1)
#define SWITCH_6 (1 << 0)
#endif

/**
 * \if de
 * Signalisiert ein erkanntes Tasterereignis.
 * \endif
 *
 * \if en
 * Signals if switch event has been detected.
 * \endif
 */
extern volatile bool switch_event;

#ifdef __DOXYGEN__
/**
 * \if de
 * Bietet die Möglichkeit, eine eigene Funktion bei jedem Aufruf der Switch-ISR
 * auszuführen. Die interne Funktionalität bleibt dabei erhalten.
 *
 * Der Aufruf erfolgt aus dem Kontext eines Interrupts!
 *
 * Ist (SwitchISRHook == NULL), findet kein Aufruf statt.
 *
 * Diese Funktionalität steht nicht zur Verfügung, wenn SWITCH_ISR_HOOK_ENABLE
 * deaktiviert ist.
 * \endif
 *
 * \if en
 * Offers the ability to execute a custom function in addition to the internal
 * code of the switch ISR.
 *
 * The function will be called from an interrupt context!
 *
 * No function call will be executed if (SwitchISRHook == NULL).
 *
 * The switch hook feature is not available if SWITCH_ISR_HOOK_ENABLE has been
 * disabled.
 * \endif
 */
extern ISRHookFunc_t *SwitchISRHook;
#else  // !__DOXYGEN__
#if SWITCH_ISR_HOOK_ENABLE
extern ISRHookFunc_t *SwitchISRHook;
#endif
#endif

/**
 * \if de
 * Initialisiert die Taster-Auswertung. Die Interruptüberwachung wird dabei
 * nicht aktiviert.
 * \endif
 *
 * \if en
 * Initializes the switch readout functionality. The switch interrupt
 * observation feature will not be enabled during the initialization.
 * \endif
 */
extern void SwitchInit(void);

/**
 * \if de
 * Erfasst den genauen Tasterstatus und gibt diesen zurück.
 *
 * Zur bequemeren Auswertung der Taster können die Makros SWITCH_1 bis SWITCH_6
 * verwendet werden.
 *
 * Während der Ausführung von PollSwitch() sollte der SwitchInterrupt deaktiviert
 * sein, da dieser sonst ausgelöst wird.
 *
 * @return der Status der einzelnen Taster, je Taster ein Bit, von Bit 0
 *         (Taster 6) bis Bit 5 (Taster 0).
 * \endif
 *
 * \if en
 * Reads/returns the state of all bumper switches fused into a single byte.
 *
 * The macros SWITCH_1 to SWITCH_6 are provided for a convenient access of the
 * bits representing the according bumper switch.
 *
 * The switch interrupt observation feature should be disabled if PollSwitch()
 * is called, otherwise the readout process will trigger the switch interrupt.
 *
 * @return the state of the bumper switches, represented by one bit per switch,
 *         starting with bit 0 (switch 6) up to bit 5 (switch 0)
 * \endif
 */
extern uint8_t PollSwitch(void);

/**
 * \if de
 * Aktiviert die Interruptüberwachung der Taster.
 * Es werden zwei Ereignisse erkannt:
 * @li Das Betätigen von (mindestens) einem Taster. Werden weitere Taster
 *     gedrückt, wird dafür kein Ereignis ausgelöst.
 * @li Das Loslassen des letzten betätigten Tasters.
 *
 * Wenn ein solches Ereignis erkannt wurde, wird switch_event gesetzt und die
 * Interruptüberwachung deaktiviert.
 *
 * Ein Aufruf von PollSwitch() löst den SwitchInterrupt ebenfalls aus, die
 * Interruptüberwachung der Schalter sollte daher, wenn PollSwitch() ausgeführt
 * wird, vorübergehend deaktivert werden.
 *
 * Diese Funktion erfordert eine globale Interruptfreigabe!
 *
 * switch_event wird beim Aufruf dieser Funktion NICHT zurückgesetzt.
 * \endif
 *
 * \if en
 * Enables the switch interrupt observation feature.
 * Two different events will be detected and trigger the switch interrupt:
 * @li Pressing at least one switch. Only the first pressed switch triggers the
 *     interrupt, subsequent pressed switches will not trigger another interrupt
 *     until all switches have been released.
 * @li Releasing the last switch beeing pressed.
 *
 * If one of those events has been detected, switch_event will be set and the
 * interrupt observation will be disabled.
 *
 * Calling PollSwitch() triggers the switch interrupt, too. Disabling the
 * switch interrupt observation feature temporary before calling PollSwitch() is
 * recommended therefore.
 *
 * This function requires interrupts to be globally enabled.
 *
 * switch_event will NOT be reset by this function.
 * \endif
 */
extern void SwitchInterruptEnable(void);
/**
 * \if de
 * Deaktiviert die Interruptüberwachung der Taster.
 * \endif
 *
 * \if en
 * Disables the switch interrupt observation feature.
 * \endif
 */
extern void SwitchInterruptDisable(void);

#endif /* SWITCHES_H_ */
