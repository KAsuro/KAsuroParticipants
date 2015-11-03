/**
 * @file
 *
 * \if de
 * Definiert eine Schnittstelle zur Kommunikation über die serielle (Infrarot-)
 * Schnittstelle.
 *
 * @par Wichtiger Hinweis:
 *      Es kann nicht gleichzeitig gesendet und/oder empfangen werden. In keiner
 *      Konstellation ist ein "paralleler" Aufruf von SerRead() und/oder
 *      SerWrite() (etwa durch eine ISR) zulässig.
 *
 * @par Initialisierung:
 *      Vor der Verwendung des Moduls muss SerInit() aufgerufen werden, zudem
 *      muss eine Initialisierung von Timer2 (time.h) erfolgen.
 *
 * @par Genutzte Softwaremodule:
 *      @li time.h
 *
 * @par Genutzte Hardwareressourcen:
 *      Module:
 *      @li U(S)ART
 *
 *      I/O:
 *      @li Port D: Pin 0 (RX), Pin 1 (TX)
 * \endif
 *
 * \if en
 * Interface definitions for the serial/infrared interface.
 *
 * @par Important:
 *      The infrared interface works in simplex mode only, it is not possible
 *      to send and/or receive in the same time. Concurrent calls to SerRead()
 *      and/or SerWrite() (for example from an ISR) are therefore not permitted.
 *
 * @par Initialization:
 *      SerInit() must be called before using this module. Additionally timer2
 *      (time.h) must have been initialized before usage.
 *
 * @par Software interfaces in use:
 *      @li time.h
 *
 * @par Hardware resources in use:
 *      Components:
 *      @li U(S)ART
 *
 *      I/O:
 *      @li Port D: Pin 0 (RX), Pin 1 (TX)
 * \endif
 *
 * @author Markus Jung
 *
 * @version 23.05.2010 \n
 *  Schnittstellendefinition entsprechend asuro.h
 * @version 16.05.2010 \n
 *  Doku
 * @version 04.09.2013 \n
 *  English documentation
 * @version 29.10.2013 \n
 *  Clarify documentation regarding parallel use of SerRead() and SerWrite()
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

#ifndef UART_H_
#define UART_H_

/**
 * \if de
 * Initialisiert die serielle Schnittstelle.
 * Nach Aufruf dieser Funktion können Daten über die serielle Schnittstelle
 * übertragen werden, vorausgesetzt Timer2 wurde initialisiert (time.h)
 * \endif
 *
 * \if en
 * Initializes the serial communication interface.
 * After execution of this function, data can be transmitted using the serial
 * interface, if timer2 has been initialized, too.
 * \endif
 */
extern void SerInit(void);

/**
 * \if de
 * Liest eine bestimmte Menge von Daten von der seriellen Schnittstelle und
 * legt diese sequenziell ab.
 *
 * Der Parameter timeout legt fest, wie lange die Funktion maximal blockiert,
 * sollte also nach der vorgegebenen Zeit nicht die gewünschte Anzahl Bytes
 * empfangen worden sein, kehrt die Funktion zurück.\n
 * Als Zeitbasis dient der 36kHz-Takt aus dem Modul time.h.
 *
 * Ein timeout-Wert von 0 deaktiviert die Timeout-Behandlung/Erkennung, die
 * Funktion kehrt dann also erst zurück, nachdem die unter length angegebene
 * Anzahl Bytes empfangen wurde.
 *
 * Zurückgegeben wird die Anzahl der empfangen Bytes, ein Rückgabewert
 * kleiner als length bedeutet dass ein Timeout aufgetreten ist.
 *
 * @par Wichtiger Hinweis:
 *      Zu jedem Zeitpunkt darf maximal ein Aufruf von SerRead() oder SerWrite()
 *      aktiv sein, insbesondere ist eine "parallele" Ausführung nicht zulässig. \n
 *      Während der Übertragung müssen Interrupts global freigegeben sein.
 *
 * @param data ist ein Zeiger auf die erste zu beschreibende Speicherstelle
 * @param length die Anzahl der einzulesenden Bytes
 * @param timeout spezifiziert einen Timeout für die Empfangsoperation, ein
 *                Wert von 0 deaktivert den Timeout.
 * @return die Anzahl der empfangenen Bytes
 * \endif de
 *
 * \if en
 * Reads a defined amount of data from the serial interface and stores it
 * sequentially into memory.
 *
 * The parameter "timeout" limits the time this function waits for incoming
 * data, the function will return if either the requested amount of bytes has
 * been received or the timeout limit has been reached. \n
 * The time base for "timeout" is the 36kHz clock provided by time.h
 *
 * A timeout value of 0 disables the timeout handling. In this case the function
 * will not return until "length" bytes have been received.
 *
 * The return value of this function is the acutal number of received bytes, if
 * this number is less than "length", a timeout has occured.
 *
 * @par Important:
 *      At most one call to either SerRead() or SerWrite() is allowed to be
 *      active at any time. Concurrent calls are not allowed. \n
 *      Interrupts must be globally enabled during transmission.
 *
 * @param data is a pointer to the memory location to write to
 * @param length the number of bytes to read
 * @param timeout the timeout for the receive operation, measured in 36kHz
 *                ticks. A value of 0 disables the timeout handling.
 * @return the number of bytes actually read
 * \endif en
 */
extern uint8_t SerRead(char * const data, const uint8_t length,
        uint16_t timeout);

/**
 * \if de
 * Überträgt übergebene Daten byteweise über die serielle Schnittstelle und
 * kehrt nach dem Abschluss des Sendevorganges zurück.
 *
 * Während der Übertragung wird der IR-Sender aktiviert, danach wieder
 * abgeschaltet.
 *
 * @par Wichtiger Hinweis:
 *      Zu jedem Zeitpunkt darf maximal ein Aufruf von SerRead() oder SerWrite()
 *      aktiv sein, insbesondere ist eine "parallele" Ausführung nicht zulässig. \n
 *      Während der Übertragung müssen Interrupts global freigegeben sein.
 *
 * @param data ist ein Zeiger auf das erste der zu übertragenden Bytes
 * @param length gibt an wieviele Bytes übertragen werden sollen
 * \endif
 *
 * \if en
 * Transmits the passed data bytewise using the serial interface and returns
 * as soon the transmission has been finished.
 *
 * The IR transmitter will be enabled during transmission and disabled
 * afterwards.
 *
 * @par Important:
 *      At most one call to either SerRead() or SerWrite() is allowed to be
 *      active at any time. Concurrent calls are not allowed. \n
 *      Interrupts must be globally enabled during transmission.
 *
 * @param data is a pointer to the first byte to transmit
 * @param length the number of bytes to send
 * \endif
 */
extern void SerWrite(char * data, uint8_t length);
extern void SerWriteInt(int value);

#endif /* UART_H_ */
