/**
 * @file
 *
 * \if de
 * Definiert die Schnittstelle für die Verwendung des AD-Wandlers. Diese
 * ermöglicht es, in fast jedem Kontext AD-Wandlungen durchzuführen ohne dabei
 * andere Programmteile in der Verwendung des ADC zu stören.
 *
 * Die hier spezifizierten Funktionen sehen zwei Verwendungsschemata für den
 * AD-Wandler vor, nämlich einen asynchronen und einen synchronen Betrieb.
 *
 * Im synchronen Modus müssen alle Operationen in einen ADC_BLOCK eingeschlossen
 * sein. Der synchrone Modus kann in jedem Kontext (also auch in ISRs) verwendet
 * werden, dies wird durch ADC_BLOCK abgesichert.\n
 * Eine verschachtelte (auch über mehrere Funktionen hinweg) Verwendung von
 * ADC_BLOCK ist ohne Probleme möglich.
 *
 * Synchrone Operationen:
 * @li ADCSelectChannel()
 * @li ADCMeasure()
 *
 * Ein Beispiel für die Verwendung des synchronen Modus:
 * @bcode{
 * ADC_BLOCK {
 *      ADCSelectChannel(ADC_BAT);
 *      ADCMeasure();
 *      foo = ADC; // foo sei von einem beliebigen numerischen Typ
 *      ADCSelectChannel(ADC_LEFT_ADJUST(ADC_ODO_LEFT));
 *      ADCMeasure();
 *      // etc.
 * }
 * }
 *
 * Im asynchronen Modus wird eine Wandlung gestartet, ohne auf das Ende der
 * Wandlung zu warten. Stattdessen wird bei Wandlungsende die ADC-ISR aufgerufen.
 * Der asynchrone Modus kann jederzeit aktiviert werden, ist gerade ein ADC_BLOCK
 * aktiv, wird die neue Kanalkonfiguration bei Verlassen des letzten ADC_BLOCK
 * übernommen.
 *
 * Die Verwendung des asynchronen Modus erfordert es, dass der
 * ADC-Interruptvektor verwendet wird. Im Normalfall geschieht dies durch
 * die Sensorauswertung (sensors.h).
 *
 * Asynchrone Operationen:
 * @li ADCInterruptEnable()
 * @li ADCInterruptDisable()
 *
 * Der asynchrone Modus erfordert eine globale Interruptfreigabe!
 *
 * @par Initialisierung:
 *      Vor der Verwendung dieses Moduls muss ADCInit() aufgerufen werden.
 *
 * @par Genutzte Hardwareressourcen:
 *      Module:
 *      @li ADC (ohne ADC-Interruptvektor)
 *
 *      I/O:
 *      @li Port C: Pin 0 bis Pin 5 (Analoger Signaleingang)
 * \endif
 *
 * \if en
 * Defines an interface for using the AD converter. It supports AD conversions
 * in almost every possible context, including ISRs, without disturbance of
 * other program components.
 *
 * The interface supports two ways of using the ADC, a synchronous mode and an
 * asynchronous mode.
 *
 * The synchronous mode requires all operations to be enclosed in a ADC_BLOCK.
 * It can be used in every possible context (including ISRs), ADC_BLOCK takes
 * care to cover all those situations. \n
 * A nested usage of ADC_BLOCK (including called functions) is supported, too.
 *
 * Synchronous operations:
 * @li ADCSelectChannel()
 * @li ADCMeasure()
 *
 * An example for using the ADC in synchronous mode:
 * @bcode{
 * ADC_BLOCK {
 *      ADCSelectChannel(ADC_BAT);
 *      ADCMeasure();
 *      foo = ADC; // foo is some numerical variable
 *      ADCSelectChannel(ADC_LEFT_ADJUST(ADC_ODO_LEFT));
 *      ADCMeasure();
 *      // etc.
 * }
 * }
 *
 * The asynchronous mode does, in contrast to the synchronous mode, not
 * wait/block until the conversion has been finished. The function calls return
 * after starting a conversion and the ADC interrupt will be triggered as soon
 * the conversion has been finished. The asynchronous mode can be enabled in
 * every possible context but the channel configuration update will be deferred
 * if anADC_BLOCK is active. The changes will be applied after leaving the last
 * active ADC_BLOCK.
 *
 * Using the asynchronous mode implies the usage of the ADC interrupt. The
 * AutoMeasurmentMode (sensors.h) can be utilized in most use cases, manual
 * usage of the asynchronous mode requires the implementation of a custom ADC
 * ISR or of some custom ADC hook code if sensors.h is used for other purposes.
 *
 * Asynchronous operations:
 * @li ADCInterruptEnable()
 * @li ADCInterruptDisable()
 *
 * The asynchronous mode requires interrupts to be globally enabled!
 *
 * @par Initialization:
 *      The function ADCInit() is required to be called before using this module.
 *
 * @par Hardware resources in use:
 *      Components:
 *      @li ADC (but not the ADC interrupt vector)
 *
 *      I/O:
 *      @li Port C: Pin 0 up to pin 5 (analogue signal input)
 * \endif
 *
 * @author Markus Jung
 *
 * @version 18.05.2010 \n
 *  Angelegt, Grundstruktur definiert
 * @version 20.05.2010 \n
 *  Definitionen umstrukturiert
 * @version 28.05.2010 \n
 *  Funktionen für den Interruptbetrieb eingefügt
 * @version 14.06.2010 \n
 *  Doku \n
 *  ADC_LEFT_ADJUST modifiziert
 * @version 02.08.2010 \n
 *  ADMUX_RESERVED_VALUE hierher migriert
 * @version 02.09.2013 \n
 *  Reduce ADC clock frequency: Prescaler 32 -> 64
 * @version 15.09.2013 \n
 *  Add support for ADC channel VBG
 * @version 20.10.2013 \n
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

#ifndef ADC_H_
#define ADC_H_

#include <stdint.h>
#include <avr/io.h>
#include "../util/misc.h"

/**
 * \if de
 * Initialisiert den ADC-Wandler und die Routinen für dessen Ansteuerung.
 * \endif
 *
 * \if en
 * Initializes the ADC and the functions for using it.
 * \endif
 */
extern void ADCInit(void);

/**
 * \if de
 * Wählt einen der verfügbaren ADC-Kanäle für nachfolgende Messungen aus.
 *
 * Diese Funktion darf nur innerhalb eines ADC_BLOCK aufgerufen werden!
 *
 * @param channel der zu wählende ADC-Kanal
 * \endif
 *
 * \if en
 * Selects one of the available ADC channels for subsequent measurments.
 *
 * This function must not be called outside of an ADC_BLOCK!
 *
 * @param channel the ADC channel to select
 * \endif
 */
extern void ADCSelectChannel(const uint8_t channel);
/**
 * \if de
 * Startet eine AD-Wandlung unter Verwendung des zuletzt ausgewählten ADC-Kanals
 * durch und wartet auf deren Beendigung.
 *
 * Das Ergebnis der Wandlung steht in den ADC-Registern ADCH und ADCL bzw. ADC(W).
 *
 * Diese Funktion darf nur innerhalb eines ADC_BLOCK aufgerufen werden!
 * \endif
 *
 * \if en
 * Triggers one AD conversion using the last recently selected ADC channel and
 * waits until the conversion has been finished.
 *
 * The conversion result can be obtained from the ADC registers ADCH and ADCL
 * respective ADC(W).
 *
 * This function must not be called outside of an ADC_BLOCK!
 * \endif
 */
extern void ADCMeasure(void);

/**
 * \if de
 * Startet eine asynchrone AD-Wandlung des gewählten Kanals,
 * nach deren Abschluss der ADC-Interruptvektor angesprungen wird.
 *
 * Der asynchrone Modus kann innerhalb der ADC-ISR durch einen erneuten Aufruf
 * dieser Funktion fortgesetzt, oder mittels ADCInterruptDisable() beendet werden.
 * Findet der Aufruf statt, während gerade eine asynchrone Wandlung läuft, wird
 * deren Ergebnis verworfen und der neue Kanal angewählt.
 *
 * Wird diese Funktion innerhalb eines ADC_BLOCK aufgerufen, wird der asynchrone
 * Modus erst bei verlassen des ADC_BLOCK aktiviert.
 *
 * Der asynchrone Modus erfordert eine globale Interruptfreigabe!
 *
 * @param channel der zu wählende ADC-Kanal
 * \endif
 *
 * \if en
 * Triggers an asynchronous AD conversion using the passed channel. The ADC-ISR
 * will be called after the conversion has been finished.
 *
 * The asynchronous ADC mode operates in a one-shot way. To do further
 * asynchronous measurments, this function can simply be called again from the
 * ADC-ISR. ADCInterruptDisable() must be called to finish the asynchronous mode.
 * If ADCInterruptEnable is called while another conversion is running, this
 * particular conversion will be aborted and the new channel configuration will
 * be applied.
 *
 * If used inside an ADC_BLOCK, the the asynchronous mode will be enabled as
 * soon this ADC_BLOCK has been left.
 *
 * Interrupts are required to be globally enabled for the asynchronous mode!
 *
 * @param channel the ADC channel to select
 * \endif
 */
extern void ADCInterruptEnable(const uint8_t channel);
/**
 * \if de
 * Beendet den asynchronen Modus oder verhindert dass er nach Verlassen eines
 * ADC_BLOCK wieder aktiviert wird.
 * Ist kein ADC_BLOCK aktiv wird der AD-Wandler abgeschaltet.
 * \endif
 *
 * \if en
 * Disables the asynchronous mode or prevents its activation after leaving an
 * ADC_BLOCK.
 * The ADC will be disabled if no ADC_BLOCK is active.
 * \endif
 */
extern void ADCInterruptDisable(void);

#ifndef __DOXYGEN__
/**
 * @internal
 *
 * \if de
 * Definiert einen reservierten Wert, der dazu verwendet wird, eine ungültige/
 * nicht vorhandene Kanalkonfiguration zu signalisieren.
 *
 * <b>Dieser Wert darf kein gültiger Wert für das Register ADMUX sein!</b>
 * \endif
 *
 * \if en
 * Defines a reserved value used to signal an invalid ADC channel configuration.
 *
 * <b>This value must not be a valid value of the register ADMUX!</b>
 * \endif
 */
#define ADMUX_RESERVED_VALUE 0xFF
/**
 * @internal
 *
 * \if de
 * Wird intern beim Eintritt in einen ADC_BLOCK aufgerufen.
 * Ein evtl. aktiver asynchroner Modus wird ebenso unterbrochen wie andere
 * aktive Wandlungen.
 *
 * Wurde eine aktive Wandlung unterbrochen, wird ein Wert ungleich
 * ADMUX_RESERVED_VALUE zurückgegeben.
 *
 * @return die bisher gültige Kanalkonfiguration
 * \endif
 *
 * \if en
 * Will be called internally before entering an ADC_BLOCK.
 * The asynchronous mode, if active, will be interrupted, as well as any other
 * active AD conversion.
 *
 * The return value will not equal to ADMUX_RESERVED_VALUE if an AD conversion
 * has been interrupted.
 *
 * @return the previously active channel configuration
 * \endif
 */
extern uint8_t __ADCBlockEnter(void);
/**
 * @internal
 *
 * \if de
 * Wird intern beim Verlassen eines ADC_BLOCK aufgerufen.
 * War zuvor eine andere Wandlung aktiv, wird diese wieder angestoßen,
 * wurde der asynchrone Modus unterbrochen (und zwischenzeitlich nicht beendet),
 * wird dieser wieder aktiviert.
 *
 * @param stored die beim Eintritt in den ADC_BLOCK gesicherte Kanalkonfiguration
 * \endif
 *
 * \if en
 * Will be called internally before leaving an ADC_BLOCK.
 * If a AD conversion has been active before entering the ADC_BLOCK, it will be
 * started again. If the asynchronous conversion mode has been interrupted by
 * the ADC_BLOCK, it will be enabled again, if it has not been disabled since
 * then.
 *
 * @param stored the channel configuration which has been saved before entering
 *               the ADC_BLOCK
 * \endif
 */
extern void __ADCBlockLeave(uint8_t stored);
/**
 * @internal
 *
 * \if de
 * Dient intern nur zu dem Zweck, die Schnittstelle des cleanup-Handlers
 * anzupassen, __ADCBlockLeave() wird dann mit dem referenzierten Parameter
 * aufgerufen.
 *
 * @param ptr ein Zeiger auf die an ADCBlockLeave() zu übergebende
 *            Kanalkonfiguration
 * \endif
 *
 * \if en
 * Only used to adjust the interface of the cleanup handler, __ADCBlockLeave()
 * will be called with the referenced parameter as argument.
 *
 * @param ptr a pointer to the ADC channel configuration which will be passed to
 *            ADCBlockLeave()
 * \endif
 */
static ALWAYS_INLINE void __ADCBlockLeaveByPtr(const uint8_t * const ptr) {
    __ADCBlockLeave(*ptr);
}
#endif

/**
 * \if de
 * Markiert einen Block mit exklusivem Zugriff auf den AD-Wandler.
 * Bei Betreten des Blocks werden aktive Wandlungen (synchron oder asynchron)
 * unterbrochen und ihre Konfiguration gesichert.
 * Bei Verlassen des Blocks werden unterbrochene Wandlungen fortgesetzt bzw.
 * der ADC deaktiviert falls keine Wandlung mehr ansteht.
 * \endif
 *
 * \if en
 * Denotes a block with exclusive access to the ADC.
 * Active conversions (synchronous as well as asynchronous) will be interrupted
 * if an ADC_BLOCK is entered, while the previously active ADC channel
 * configuration is backed up.
 * While the block is left, the saved channel configuration will be restored and
 * the interrupted conversion will be restarted. The ADC will be disabled if no
 * other conversion is pending.
 * \endif
 */
#ifdef __DOXYGEN__
#define ADC_BLOCK
#else
#define ADC_BLOCK \
    for(uint8_t __stored ON_CLEANUP(__ADCBlockLeaveByPtr) = __ADCBlockEnter(), \
        __loop = 1; __loop; __loop = 0)
#endif

#ifdef __DOXYGEN__
/**
 * \if de
 * ADC-Kanal 0.
 * \endif
 *
 * \if en
 * ADC channel 0.
 * \endif
 */
#define ADC_CH_0
/**
 * \if de
 * ADC-Kanal 1.
 * \endif
 *
 * \if en
 * ADC channel 1.
 * \endif
 */
#define ADC_CH_1
/**
 * \if de
 * ADC-Kanal 2.
 * \endif
 *
 * \if en
 * ADC channel 2.
 * \endif
 */
#define ADC_CH_2
/**
 * \if de
 * ADC-Kanal 3.
 * \endif
 *
 * \if en
 * ADC channel 3.
 * \endif
 */
#define ADC_CH_3
/**
 * \if de
 * ADC-Kanal 4.
 * \endif
 *
 * \if en
 * ADC channel 4.
 * \endif
 */
#define ADC_CH_4
/**
 * \if de
 * ADC-Kanal 5.
 * \endif
 *
 * \if en
 * ADC channel 5.
 * \endif
 */
#define ADC_CH_5
/**
 * \if de
 * ADC-Spannungsreferenz VBG.
 * \endif
 *
 * \if en
 * ADC voltage reference VBG.
 * \endif
 */
#define ADC_CH_VBG
/**
 * \if de
 * Eine Maske über alle Kanalauswahl-Bits.
 * \endif
 *
 * \if en
 * A bitmask covering all bits used for ADC channel selection.
 * \endif
 */
#define ADC_CH_MASK

/**
 * \if de
 * ADC-Referenzspannung: VCC, entkoppelt.
 * \endif
 *
 * \if en
 * ADC reference voltage: VCC, decoupled.
 * \endif
 */
#define ADC_REF_VCC_DECOUPLED
/**
 * \if de
 * ADC-Referenzspannung: VBG, entkoppelt.
 * \endif
 *
 * \if en
 * ADC reference voltage: VBG, decoupled.
 * \endif
 */
#define ADC_REF_VBG_DECOUPLED
/**
 * \if de
 * Eine Maske über alle Referenzspannungs-Bits.
 * \endif
 *
 * \if en
 * A bitmask covering all bits used for ADC voltage reference selection.
 * \endif
 */
#define ADC_REF_MASK

/**
 * \if de
 * ADC-Kanalkonfiguration für die Batteriespannungsmessung (Kanal 5, AREF = VBG).
 * \endif
 *
 * \if en
 * ADC channel configuration for battery voltage measurment
 * (channel 5, AREF = VBG).
 * \endif
 */
#define ADC_BAT
/**
 * \if de
 * ADC-Kanalkonfiguration für die interne Batteriespannungsmessung ohne externen
 * Spannungsteiler (Kanal VBG, AREF = VCC).
 * \endif
 *
 * \if en
 * ADC channel configuration for internal battery voltage measurment without
 * external voltage divider (channel VBG, AREF = VBG).
 * \endif
 */
#define ADC_BAT_INTERNAL
/**
 * \if de
 * ADC-Kanalkonfiguration für die Tasterauswertung (Kanal 4, AREF = VCC).
 * \endif
 *
 * \if en
 * ADC channel configuration for switch readout (channel 4, AREF = VCC).
 * \endif
 */
#define ADC_SWITCH
/**
 * \if de
 * ADC-Kanalkonfiguration für den linken Linienfolgesensor (Kanal 3, AREF = VCC).
 * \endif
 *
 * \if en
 * ADC channel configuration for the left line sensor (channel 3, AREF = VCC).
 * \endif
 */
#define ADC_IR_LEFT
/**
 * \if de
 * ADC-Kanalkonfiguration für den rechten Linienfolgesensor (Kanal 2, AREF = VCC).
 * \endif
 *
 * \if en
 * ADC channel configuration for the right line sensor (channel 2, AREF = VCC).
 * \endif
 */
#define ADC_IR_RIGHT
/**
 * \if de
 * ADC-Kanalkonfiguration für den linken Odometriesensor (Kanal 1, AREF = VCC).
 * \endif
 *
 * \if en
 * ADC channel configuration for the left odometry sensor (channel 1, AREF = VCC).
 * \endif
 */
#define ADC_ODO_LEFT
/**
 * \if de
 * ADC-Kanalkonfiguration für den rechten Odometriesensor (Kanal 0, AREF = VCC).
 * \endif
 *
 * \if en
 * ADC channel configuration for the right odometry sensor (channel 0, AREF = VCC).
 * \endif
 */
#define ADC_ODO_RIGHT
#else
#define ADC_CH_0    0
#define ADC_CH_1    (1 << MUX0)
#define ADC_CH_2    (1 << MUX1)
#define ADC_CH_3    ((1 << MUX1) | (1 << MUX0))
#define ADC_CH_4    (1 << MUX2)
#define ADC_CH_5    ((1 << MUX2) | (1 << MUX0))
#define ADC_CH_VBG  ((1 << MUX3) | (1 << MUX2) | (1 << MUX1))
#define ADC_CH_MASK ((1 << MUX3) | (1 << MUX2) | (1 << MUX1) | (1 << MUX0))

#define ADC_REF_VCC_DECOUPLED   (1 << REFS0)
#define ADC_REF_VBG_DECOUPLED   ((1 << REFS1) | (1 << REFS0))
#define ADC_REF_MASK            ADC_REF_VBG_DECOUPLED

#define ADC_BAT             (ADC_CH_5 | ADC_REF_VBG_DECOUPLED)
#define ADC_BAT_INTERNAL    (ADC_CH_VBG | ADC_REF_VCC_DECOUPLED)
#define ADC_SWITCH          (ADC_CH_4 | ADC_REF_VCC_DECOUPLED)
#define ADC_IR_LEFT         (ADC_CH_3 | ADC_REF_VCC_DECOUPLED)
#define ADC_IR_RIGHT        (ADC_CH_2 | ADC_REF_VCC_DECOUPLED)
#define ADC_ODO_LEFT        (ADC_CH_1 | ADC_REF_VCC_DECOUPLED)
#define ADC_ODO_RIGHT       (ADC_CH_0 | ADC_REF_VCC_DECOUPLED)
#endif

/**
 * \if de
 * Modifiziert die übergebene Kanalkonfiguration so,
 * dass die AD-Wandlungsergebnisse zum MSB (links) hin ausgerichtet werden.
 *
 * @see Datasheet, ADMUX-Flag ADLAR
 *
 * @param ch die zu modifizierende Kanalkonfiguration
 * @return die Kanalkonfiguration mit gesetztem ADLAR-Bit
 * \endif
 *
 * \if en
 * Changes the passed channel configuration to make the AD conversion results
 * flushed to the MSB (left) adjusted.
 *
 * @see Datasheed, flag ADLAR in register ADMUX
 *
 * @param ch the channel configuration to modify
 * @return the channel configuration with ADLAR bit set
 * \endif
 */
#ifdef __DOXYGEN__
#define ADC_LEFT_ADJUST(ch)
#else
#define ADC_LEFT_ADJUST(ch) ((ch) | (1 << ADLAR))
#endif

/**
 * \if de
 * Gibt die aktuell gesetzte ADC-Kanalkonfiguration zurück.
 * \endif
 *
 * \if en
 * Returns the currently active ADC channel configuration.
 * \endif
 */
#ifdef __DOXYGEN__
#define ADC_GET_CHANNEL()
#else
#define ADC_GET_CHANNEL() ADMUX
#endif

/**
 * \if de
 * Der Basiswert für alle Zuweisungen an ADCSRA, die ADC-Frequenz wird hier
 * auf F_CPU / 64 festgesetzt.
 * \endif
 *
 * \if en
 * The default value used for all assignments to the register ADCSRA, it
 * configures the ADC clock to F_CPU / 64.
 * \endif
 */
#ifdef __DOXYGEN__
#define ADCSRA_BASE_VALUE
#else
#define ADCSRA_BASE_VALUE ((1 << ADPS2) | (1 << ADPS1))
#endif

#endif /* ADC_H_ */
