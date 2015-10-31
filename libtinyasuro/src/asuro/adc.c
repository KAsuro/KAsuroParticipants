/**
 * @file
 *
 * \if de
 * Implementiert die in adc.h spezifizierte Funktionalität.
 *
 * Die Sicherung der Kanalkonfiguration basiert darauf, dass es mindestens einen
 * ungültigen Wert für ADMUX gibt, der über ADMUX_RESERVED_VALUE konfiguriert
 * werden kann. \n
 * Beim Verlassen eines ADC_BLOCK kann __ADCBlockLeave() so feststellen, ob,
 * und wenn ja, welche Konfiguration (normale Wandlung oder Interruptmodus)
 * zurückgeschrieben werden soll.
 * \endif
 *
 * \if en
 * Implements the functionality specified in adc.h
 *
 * The mechanism used to save and restore the channel configuration relies on
 * the existence of at least one undefined/invalid value for the register ADMUX.
 * This value is defined by ADMUX_RESERVED_VALUE. \n
 * The function __ADCBlockLeave(), which is called if the program flow leaves an
 * ADC_BLOCK, uses this reserved value to detect which configuration (normal or
 * interrupt/asynchronous mode), if any, should be written back to ADMUX.
 * \endif
 *
 * @version 18.05.2010 \n
 *  Erste Teile der Funktionalität integriert \n
 *  Headerfile angelegt
 * @version 21.05.2010 \n
 *  Umgebaut, Interruptmodus implementiert
 * @version 01.06.2010 \n
 *  Bugfix bei ADCInterruptDisable
 * @version 13.06.2010 \n
 *  triggerInterruptMeasurment entfernt/direkt in ADCInterruptEnable kopiert
 * @version 14.06.2010 \n
 *  ADCInterruptEnable so umgebaut, dass es jederzeit aufgerufen werden kann
 * @version 21.06.2010 \n
 *  Bugfix __ADCBlockEnter(): Abschaltung ADIE bei aktiven Interrupts vergessen
 * @version 02.08.2010 \n
 *  ADMUX_RESERVED_VALUE anstelle der Magic-Number 0xFF eingesetzt \n
 *  ADMUX_RESERVED_VALUE ins Headerfile migriert
 * @version 03.09.2013 \n
 *  ADCInterruptEnable: No more blocking/polling loop inside ATOMIC_BLOCK
 * @version 20.10.2013 \n
 *  English translation
 *
 * @author Markus Jung
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
#include <util/atomic.h>

#include <asuro/adc.h>

/**
 * \if de
 * Dient zur Aufnahme des zuletzt gewählten Kanals im Interruptmodus, falls
 * dieser unterbrochen wird.
 * Ist der Interruptmodus inaktiv gilt:
 * @bcode{old_channel == ADMUX_RESERVED_VALUE}
 * \endif
 *
 * \if en
 * Used to store the last recently selected channel configuration used in
 * asynchronous/interrupt mode, if this mode has been interrupted.
 * The equation below holds true if the asynchronous mode is not enabled:
 * @bcode{old_channel == ADMUX_RESERVED_VALUE}
 * \endif
 */
static volatile uint8_t old_channel;

/**
 * \if de
 * Schaltet den ADC ganz aus.
 * \endif
 *
 * \if en
 * Shuts down the ADC completely.
 * \endif
 */
static inline void ADCOff(void) {
    ADCSRA = 0;
}

/**
 * \if de
 * Schaltet den ADC ab, falls die Spannungsreferenz der beiden
 * Kanalkonfigurationen verschieden ist.
 * Dadurch wird eine lange AD-Wandlung bei der nächsten Messung erzwungen.
 *
 * @param oldmux die alte Kanalkonfiguration
 * @param newmux die neue Kanalkonfiguration
 * \endif
 *
 * \if en
 * Disables the ADC if the two passed channel configurations use a different
 * voltage reference.
 * The ADC is therefore forced to perform a long AD conversion at the next
 * measurment request.
 *
 * @param oldmux the old channel configuration
 * @param newmux the new channel configuration
 * \endif
 */
static inline void disableADCOnRefChange(const uint8_t oldmux,
        const uint8_t newmux) {
    if ((oldmux & ADC_REF_MASK) != (newmux & ADC_REF_MASK)) {
        ADCOff();
    }
}

void ADCInit(void) {
    old_channel = ADMUX_RESERVED_VALUE;
    ADCSRA = ADCSRA_BASE_VALUE;
}

uint8_t __ADCBlockEnter(void) {
    const uint8_t _sreg = SREG;

    cli();
    const uint8_t _admux = ADMUX;
    const uint8_t _adcsra = ADCSRA & ((1 << ADEN) | (1 << ADIE));

    uint8_t result = ADMUX_RESERVED_VALUE;

    if (_adcsra) { // ADC is already enabled
        if (_adcsra & (1 << ADIE)) {
            // interrupt asynchronous mode
            old_channel = _admux;
            ADCSRA = ADCSRA_BASE_VALUE | (1 << ADEN); // ADIE off, ADC left on
        } else {
            // interrupt a "normal" AD conversion
            result = _admux;
        }
    }

    SREG = _sreg; // restore interrupt flag before blocking/polling loop

    while (ADCSRA & (1 << ADSC))
        ; // wait until the conversion has been finished

    return result;
}

void __ADCBlockLeave(uint8_t stored) {
    uint8_t adcsra_restore = ADCSRA_BASE_VALUE | (1 << ADEN) | (1 << ADSC) | (1 << ADIF);
    // sets ADC state to: on, start (foreign) conversion, clear interrupt flag
    const uint8_t _sreg = SREG;

    cli();

    if (stored == ADMUX_RESERVED_VALUE) {
        stored = old_channel;
        // interrupted asynchronous mode, re-enable ADC interrupt
        adcsra_restore |= (1 << ADIE);
    }

    if (stored == ADMUX_RESERVED_VALUE) {
        adcsra_restore = 0; // ADC off
    } else {
        disableADCOnRefChange(ADMUX, stored);
        ADMUX = stored;
    }

    ADCSRA = adcsra_restore;
    SREG = _sreg;
}

void ADCSelectChannel(const uint8_t channel) {
    // enforce "long" conversion if reference voltage changes
    disableADCOnRefChange(ADMUX, channel);
    ADCSRA = ADCSRA_BASE_VALUE | (1 << ADEN); // ADC on
    ADMUX = channel;
}

void ADCMeasure(void) {
    ADCSRA |= (1 << ADSC); // do not take care of ADIF since ADIE should be off

    while (ADCSRA & (1 << ADSC))
        ; // wait until the conversion has been finishied
}

void ADCInterruptEnable(const uint8_t channel) {
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        const uint8_t _adcsra = ADCSRA;

        if (!(_adcsra & (1 << ADEN)) || (_adcsra & (1 << ADIE))) {
            // ADC is off or asynchronous mode is already enabled

            disableADCOnRefChange(ADMUX, channel);

            if (ADCSRA & (1 << ADSC)) { // interrupt running conversion, if any
                ADCOff();
            }

            ADMUX = channel;
            ADCSRA = ADCSRA_BASE_VALUE | (1 << ADEN) | (1 << ADSC)
                    | (1 << ADIF) | (1 << ADIE);
            // enable ADC, start conversion, enable interrupt and clear old ADIF
        }

        // would normally be an "else", but this way is faster
        // At least one ADC_BLOCK ist active, store desired channel
        // configuration until the ADC_BLOCK is left.
        old_channel = channel;
    }
}

void ADCInterruptDisable(void) {
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        if (ADCSRA & (1 << ADIE)) {
            ADCOff();
        }

        old_channel = ADMUX_RESERVED_VALUE;
    }
}
