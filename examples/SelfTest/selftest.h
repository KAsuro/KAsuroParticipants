/**
 * @file
 *
 * \if de
 * Definiert die Schnittstelle für alle Selbsttest-Teilfunktionen in Anlehnung
 * an die jeweiligen Funktionen aus der "originalen" ASURO-Bibliothek.
 * \endif
 *
 * \if en
 * Defines the interface of all self test functions, similar to the ones of the
 * "original" ASURO library.
 * \endif
 *
 * @author Markus Jung
 *
 * @version 28.08.2010 \n
 *  Angelegt
 * @version 24.10.2013 \n
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

#ifndef SELFTEST_H_
#define SELFTEST_H_

#include <asuro/asuro.h>

/**
 * \if de
 * Empfängt je ein Byte und sendet dessen um eins inkrementierten Wert zurück.
 * Dabei wird nicht länger als 15 Sekunden auf eingehende Daten gewartet,
 * während des Empfangs ist die Status-LED Grün, während des Sendens Rot.
 * \endif
 *
 * \if en
 * Receives one byte and transmits its value incremented by one. This function
 * waits at most 15 seconds for incoming data. The status LED is lit green while
 * waiting for incoming data and red while the return value is transmitted.
 * \endif
 */
void SerialTest(void);

/**
 * \if de
 * Testet 15 Sekunden lang alle Taster des ASURO, bei Betätigung eines Tasters
 * wird ein zugeordneter Aktor eingeschaltet.
 *
 * Schalter 1 -> Status-LED (Grün) \n
 * Schalter 2 -> Status-LED (Rot) \n
 * Schalter 3 -> Linienfolge-LED \n
 * Schalter 4 -> Back-LED links \n
 * Schalter 5 -> Back-LED rechts \n
 * Schalter 5 -> Motor links
 * \endif
 *
 * \if en
 * Tests all switches for 15 seconds. Pushing a switch triggers an action
 * assigned to it.
 *
 * Switch 1 -> Status LED (green) \n
 * Switch 2 -> Status LED (red) \n
 * Switch 3 -> Line following LED \n
 * Switch 4 -> Left back LED\n
 * Switch 5 -> Right back LED\n
 * Switch 5 -> Left motor
 * \endif
 */
void SwitchTest(void);

/**
 * \if de
 * Testet die Fototransistoren an der Unterseite des ASURO für zehn Sekunden,
 * wird der linke/rechte Fototransistor angeleuchtet, wird die linke/rechte
 * Back-LED eingeschaltet.
 * \endif
 *
 * \if en
 * Tests the phototransistor at the down side of the ASURO for 10 seconds.
 * Illuminating the left/right phototransistors will light up the left/right
 * back LED.
 * \endif
 */
void LineTest(void);

/**
 * \if de
 * Testet die Odometrie-Sensoren für 15 Sekunden, wird der linke Sensor
 * aktiviert wird die Status-LED grün, beim rechten Sensor rot.
 * \endif
 *
 * \if en
 * Tests the odometry sensors for 15 seconds. Activating the left/right sensor
 * lights up the status LED green/red.
 * \endif
 */
void OdometryTest(void);

/**
 * \if de
 * Testet den rechten Motor für zehn Sekunden indem dieser zuerst auf volle
 * Geschwindigkeit vorwärts und dann rückwärts beschleunigt wird.
 * \endif
 *
 * \if en
 * Tests the right motor for 10 seconds by first increasing its speed to full
 * forward an then full backward speed.
 * \endif
 */
void MotorTestRight (void);
/**
 * \if de
 * Testet den linken Motor für zehn Sekunden indem dieser zuerst auf volle
 * Geschwindigkeit vorwärts und dann rückwärts beschleunigt wird.
 * \endif
 *
 * \if en
 * Tests the left motor for 10 seconds by first increasing its speed to full
 * forward an then full backward speed.
 * \endif
 */
void MotorTestLeft (void);
/**
 * \if de
 * Testet beide Motoren für zehn Sekunden indem diese zuerst auf volle
 * Geschwindigkeit vorwärts und dann rückwärts beschleunigt werden.
 * \endif
 *
 * \if en
 * Tests both motors for 10 seconds by first increasing their speed to full
 * forward an then full backward speed.
 * \endif
 */
void MotorTestBoth (void);
/**
 * \if de
 * Testet alle LEDs des ASURO für je drei Sekunden. Reihenfolge:
 * @li Status-LED grün
 * @li Status-LED rot
 * @li Front-LED
 * @li Back-LED links
 * @li Back-LED rechts
 * @li Alle LEDs gemeinsam
 * \endif
 *
 * \if en
 * Tests all LEDs by turning them on for each 3 seconds in the follow order:
 * @li Green status LED
 * @li Red status LED
 * @li Front LED
 * @li Left back LED
 * @li Right back LED
 * @li All LEDs
 * \endif
 */
void LEDTest (void);

#endif /* SELFTEST_H_ */
