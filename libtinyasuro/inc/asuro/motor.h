/**
 * @file
 *
 * \if de
 * Definiert die Schnittstelle zur Ansteuerug der Motoren.
 *
 * @par Initialisierung:
 *      Vor der Verwendung dieses Moduls muss MotorInit() aufgerufen werden.
 *
 * @par Genutzte Hardwareressourcen:
 *      Module:
 *      @li Timer1 (inklusive beider Output Compare Units)
 *
 *      I/O (Steuersignale):
 *      @li Port B: Pin 1, Pin 2, Pin 4, Pin 5
 *      @li Port D: Pin 4, Pin 5
 * \endif
 *
 * \if en
 * Interface definitions for controlling the motors.
 *
 * @par Initialization:
 *      Calling MotorInit() is required before using this module.
 *
 * @par Hardware ressources in use:
 *      Components:
 *      @li Timer1 (including both output compare units)
 *
 *      I/O:
 *      @li Port B: Pin 1 and 2, pin 4 and 5
 *      @li Port D: Pin 4 and 5
 * \endif
 *
 * @author Markus Jung
 *
 * @version 27.05.2010 \n
 *  Schnittstellendefinition entsprechend asuro.h adaptiert
 * @version 15.06.2010 \n
 *  Doku \n
 *  Interne Makros aus dem Header ins Sourcefile verschoben
 * @version 13.09.2010 \n
 *  Makro "BWD" als Alias für "RWD" eingebaut
 * @version 05.01.2011 \n
 *  Doku für "BWD"
 * @version 17.09.2013 \n
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

#ifndef MOTOR_H_
#define MOTOR_H_

#include <stdint.h>
#include <avr/io.h>

#ifdef __DOXYGEN__
/**
 * \if de
 * Fahrtrichtung vorwärts
 * \endif
 *
 * \if en
 * Drive direction: forward
 * \endif
 */
#define FWD
/**
 * \if de
 * Fahrtrichtung rückwärts
 * \endif
 *
 * \if en
 * Drive direction: backward
 * \endif
 */
#define BWD
/**
 * \if de
 * Alias für BWD
 *
 * @deprecated Wurde nur aus Kompatibilitätsgründen aufgenommen
 * \endif
 *
 * \if en
 * Alias for BWD
 *
 * @deprecated This alias has only been added for compatibility reasons
 * \endif
 *
 * @see RWD
 */
#define RWD
/**
 * \if de
 * Bremsen
 * \endif
 *
 * \if en
 * Drive direction: stop/break
 * \endif
 */
#define BREAK
/**
 * \if de
 * Freilauf
 * \endif
 *
 * \if en
 * Drive direction: free-wheeling
 * \endif
 */
#define FREE
#else
// These macros work for both left and right hand side since the same control
// pins are used at port B (right) and D (left)
#define FWD     (1 << PB5)  // == (1 << PD5)
#define BWD     (1 << PB4)  // == (1 << PD4)
#define RWD     BWD         // Alias for BWD
#define BREAK   0x00
#define FREE    ((1 << PB4) | (1 << PB5)) // == ((1 << PD4) | (1 << PD5))
#endif

/**
 * \if de
 * Initialisiert die Motor-Ansteuerung.
 * Der Timer zur PWM-Erzeugung wird initialisiert, die Fahrtgeschwindigkeit wird
 * auf Null, die Fahrtrichtung beider Motoren auf vorwärts konfiguriert.
 * \endif
 *
 * \if en
 * Initializes the motor control.
 * This function configures the timer used to generate the PWM signal for both
 * motors, sets the driving direction to forward and the driving speed to zero.
 * \endif
 */
extern void MotorInit(void);
/**
 * \if de
 * Setzt die Fahrtrichtung der beiden Motoren.
 *
 * Als Fahrtrichtung/-Modus stehen zur Auswahl: FWD, RWD/BWD, BREAK und FREE.
 *
 * @param left_dir die Fahrtrichtung des linken Motors
 * @param right_dir die Fahrtrichtung des rechten Motors
 * \endif
 *
 * \if en
 * Configures the driving direction for both motors.
 *
 * Available choices are: FWD, RWD/BWD, BREAK and FREE.
 *
 * @param left_dir the driving direction for the left motor
 * @param right_dir the driving direction for the right motor
 * \endif
 */
extern void MotorDir(const uint8_t left_dir, const uint8_t right_dir);
/**
 * \if de
 * Setzt die Fahrtgeschwindigkeit beider Motoren.
 *
 * Laut ASURO-Manual bewegt sich der ASURO ab Werten von ca. 60, der Wert
 * UINT8_MAX (255) entspricht der Höchstgeschwindigkeit.
 *
 * @param left_speed die Geschwindigkeit des linken Motors
 * @param right_speed die Geschwindigkeit des rechte Motors
 * \endif
 *
 * \if en
 * Configures the driving speed for both motors.
 *
 * According to the ASURO manual, the ASURO starts to move for speed values of
 * about 60, the value UINT8_MAX (255) represents the top speed.
 *
 * @param left_speed the speed of the left motor
 * @param right_speed the speed of the right motor
 * \endif
 */
extern void MotorSpeed(const uint8_t left_speed, const uint8_t right_speed);

#endif /* MOTOR_H_ */
