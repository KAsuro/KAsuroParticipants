/**
 * @file
 *
 * \if de
 * Implementiert die in uart.h spezifizierte Funktionalität.
 * \endif
 *
 * \if en
 * Implements the functionality specified in uart.h
 * \endif
 *
 * @author Markus Jung
 *
 * @version 17.05.2010 \n
 *  Funktionalität aus asuro.c verbessert implementiert, unter anderem: \n
 *  @li Keine dumme Warteschleife am Ende von SerWrite
 *  @li Verwendung von setbaud.h
 * @version 15.06.2010 \n
 *  Baudraten-Konfiguration nach config.h verlegt
 * @version 28.08.2010 \n
 *  Bugfix in SerRead(): Rückgabewert bei Abbruch passte nicht zur Dokumentation
 * @version 05.09.2013 \n
 *  English documentation
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
#include <avr/io.h>

#include <asuro/config.h> // Must be included ahead of util/setbaud.h
#include <util/setbaud.h> // util/setbaud.h is part of the avr-libc!
#include <asuro/time.h>
#include <asuro/uart.h>

#ifdef __DOXYGEN__
/** U(S)ART TX-Pin */
#define IR_TX_OUT
#else
#define IR_TX_OUT   (1 << PD1)
#endif

void SerInit(void) {
    UCSRA = 0x00;
    UCSRB = 0x00;
    UCSRC = (1 << URSEL) | (1 << UCSZ1) | (1 << UCSZ0); // 8N1

    UBRRL = UBRRL_VALUE;
    UBRRH = UBRRH_VALUE;

    // Initialize TX pin to input, low (IR LED off, power saving)
    // The USART controls the TX pin completely during transmission
    DDRD &= ~IR_TX_OUT;
    PORTD &= ~IR_TX_OUT;
}

uint8_t SerRead(char * const data, const uint8_t length, uint16_t timeout) {
    bool timedOut = false;
    uint8_t i = 0;
    uint8_t tick36kHzBuf = tick36kHz;

    UCSRB = (1 << RXEN);

    do {
        if (UCSRA & (1 << RXC)) {
            data[i++] = UDR;
        }

        const uint8_t tick36kHzUpdate = tick36kHz;

        if ((tick36kHzBuf != tick36kHzUpdate) && (timeout != 0)) {
            tick36kHzBuf = tick36kHzUpdate;
            timedOut = (--timeout == 0);
        }
    } while ((i < length) && !timedOut);

    UCSRB = 0; // Receiver off

    return i;
}
void SerWrite(char * data, uint8_t length) {
    uint8_t i = 0;

    TIMER2_CLOCK_OUT_ENABLE(); // 36kHz carrier signal on
    UCSRB = (1 << TXEN);

    while (length > 0) {
        if (UCSRA & (1 << UDRE)) { // Buffer is empty, write next byte to it
            UDR = data[i++];
            length--;
        }
    }

    // Wait until transmission has been completed, clear TXC flag afterwards
    while (!(UCSRA & (1 << TXC)))
        ;
    UCSRA |= (1 << TXC);

    UCSRB = 0; // USART transmitter off
    TIMER2_CLOCK_OUT_DISABLE(); // 36kHz carrier signal off
}

