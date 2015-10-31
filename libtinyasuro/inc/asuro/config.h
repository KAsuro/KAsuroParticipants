/**
 * @file
 *
 * \if de
 * Beinhaltet die Konfiguration für alle Module.
 *
 * Odometrie/Encoder: \n
 * Die gegebenen Konfigurationswerte sind auf Reflektorscheiben mit zwölf
 * Segmenten abgestimmt, die Übersetzung Reflektorscheibe -> Rad beträgt 5:1.
 * \endif
 *
 * \if en
 * Contains the configuration for all software modules.
 *
 * Odometry/encoder: \n
 * The default values are adjusted for encoder discs having twelve segmens
 * printed at them, and for a gear transmission ration encoder:wheel = 5:1.
 * \endif
 *
 * @see encoder.h
 * @see sensors.h
 * @see switches.h
 *
 * @author Markus Jung
 *
 * @version 08.06.2010 \n
 *  Grundkonfiguration \n
 *  Neue Odometrie-Kalibrierung
 * @version 15.06.2010 \n
 *  Doku
 * @version 15.06.2010 \n
 *  Baudraten-Konfiguration hierher verschoben
 * @version 14.08.2010 \n
 *  Internene Berechnungen der Encoder-Kalibrierung nach encoder.c verschoben
 * @version 15.09.2013 \n
 *  Add configuration for the battery voltage measurment in sensors.[h|c] \n
 *  Change SWITCH_FACTOR into a floating point value
 * @version 22.09.2013 \n
 *  English translation
 * @version 09.02.2014 \n
 *  Support baudrate configuration in Config.mk
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

#ifndef CONFIG_H_
#define CONFIG_H_

#include <stdbool.h>
#include <stdint.h>

/**
 * \if de
 * Setzt die in Config.mk konfigurierte Baudrate außer Kraft. Statt dessen wird
 * der weiter unten mit BAUD konfigurierte Wert verwendet.
 * \endif
 *
 * \if en
 * Overrides the baudrate value configured in Config.mk. The value BAUD
 * (configured below) will be used instead.
 * \endif
 */
#define OVERRIDE_BAUD false

// do not edit, automagic enable/disable for doxygen
#ifdef __DOXYGEN__
#define BAUD
#endif
// do not edit, override mechanism
#if OVERRIDE_BAUD
#undef BAUD
#endif

#ifndef BAUD
/**
 * \if de
 * Legt die Baudrate der seriellen (Infrarot) Schnittstelle fest, wenn dies
 * nicht durch das Makefile (siehe Config.mk) geschieht.
 * \endif
 *
 * \if en
 * Configures the baud rate of the serial (infrared) interface. The value is
 * only used if the symbol BAUD has not been set by the Makefile (see Config.mk).
 * \endif
 */
#define BAUD 2400
#endif

/**
 * \if de
 * Der Kalibrierfaktor für die Tasterauswertung.
 * Der Wert liegt typischerweise zwischen 61 und 64,
 * im DLR-Quelltext wurde ein Wert von 63 vorgegeben.
 * \endif
 *
 * \if en
 * The switch calibration factor. Typical values are 61 up to 64, the DLR source
 * code uses a value of 63.
 * \endif
 */
#define SWITCH_FACTOR 63.0f

/**
 * \if de
 * Wählt die für die Batteriespannungsmessung verwendete Methode aus. Zur
 * Verfügung steht die externe Spannungsmessung die den Spannungsteiler aus den
 * Widerständen R12 und R13 nutzt, sowie eine auf der internen Referenzspannung
 * basierende Variante.
 *
 * Die externe Methode erfordert weniger Rechenzeit als die interne Variante,
 * ist aber unkalibriert aufgrund von Fertigungsabweichungen bei den verbauten
 * Widerständen tendenziell ungenauer als die interne Variante. Außerdem kann
 * die externe Messung nicht mit den neueren Mikrocontrollern
 * (ATmega88, ATmega168 etc.) verwendet werden.
 * \endif
 *
 * \if en
 * Selects the method used to measure the battery voltage. Availabe are two
 * methods, an external one using the voltage divider (resistors R12 and R13)
 * of the ASURO, as well as an internal one using the internal reference voltage.
 *
 * The external method is computed slightly faster than the internal one, but
 * might be less precise in comparison to the internal method because of factory
 * deviations of the resistors. The external measurment method is not available
 * if newer microcontrollers (ATmega88, ATmega168 etc.) are used.
 * \endif
 */
#ifdef __AVR_ATmega8__
#define INTERNAL_BATTERY_MEASURMENT false
#else
// de: Automatische Auswahl der internen Messung, nicht verändern!
// en: Automatic selection of the internal measurment mode, do not edit!
#define INTERNAL_BATTERY_MEASURMENT true
#endif

/**
 * \if de
 * Ein Kalibrierfaktor für die gemessene Batteriespannung. Er wird mit dem
 * unkalibrierten Ergebnis multipliziert, für zu große Messwerte sollte ein
 * Wert kleiner 1.0 verwendet werden, für zu kleine Messwerte entsprechend ein
 * größerer Messwert.
 * Der Kalibrierfaktor der beiden Messmethoden kann sich unterscheiden, je nach
 * ausgewählter Methode sollte der entsprechende Faktor bei Bedarf angepasst
 * werden.
 * \endif
 *
 * \if en
 * A factor for calibrating the measured battery voltage. It will be
 * multiplicated by the uncalibrated measurment value. A value too large can be
 * calibrated by using a factor less than 1.0, and vice versa.
 * The calibration factor required for the two measurment methods can be
 * different. Depending on the chosen measurment method the corresponding factor
 * should be adjusted, if necessary.
 * \endif
 */
#if INTERNAL_BATTERY_MEASURMENT
// de: Kalibrierfaktor für die interne Batteriespannungsmessung
// en: Calibration factor for the internal battery voltage measurment mode
#define BATTERY_CALIBRATION_FACTOR 1.0f
#else
// de: Kalibrierfaktor für die externe Batteriespannungsmessung
// en: Calibration factor for the external battery voltage measurment mode
#define BATTERY_CALIBRATION_FACTOR 1.0f
#endif


/**
 * \if de
 * Definiert den minimalen Unterschied zwischen Hell- und Dunkelwert, ab dem
 * die Auswertung der Odometriedaten eine Flanke erkennt.
 * \endif
 *
 * \if en
 * Defines the minimum bright/dark difference which denotes a rising or falling
 * edge of the odometry data.
 * \endif
 */
#define ODO_EDGE_THRESHOLD 30

/**
 * \if de
 * Gibt an wieviele mm mit einem Tick zurückgelegt werden.
 * \endif
 *
 * \if en
 * Denotes the distance covered by one tick, in mm.
 * \endif
 */
#define ENC_MM_PER_TICK  2.016f

/**
 * \if de
 * Gibt an wieviele Ticks für eine 360°-Drehung benötigt werden.
 * \endif
 *
 * \if en
 * Denotes the number of ticks required for a 360° turn.
 * \endif
 */
#define ENC_TURN_TICKS_PER_360_DEGREE  151.6f

/**
 * \if de
 * Gibt die Grenze für den Korrekturwert im encoderkompensierten Bewegungsmodus
 * vor. Sollte die Abweichung vom Sollwert mit diesem Wert nicht kompensiert
 * werden können, verbleibt eine Restabweichung.
 *
 * Es muss gelten:
 * @bcode{0 < ENCODER_MOVEMENT_DELTA_LIMIT < INT8_MAX - ENCODER_MOVEMENT_STEP}
 * \endif
 *
 * \if en
 * Defines the maximum absolute value of the correction value used in
 * encodercompensated movement mode. If the setpoint speeds can not be reached
 * by a correction value within the limit, a control deviation will remain.
 *
 * Valid range:
 * @bcode{0 < ENCODER_MOVEMENT_DELTA_LIMIT < INT8_MAX - ENCODER_MOVEMENT_STEP}
 * \endif
 */
#define ENCODER_MOVEMENT_DELTA_LIMIT 100
/**
 * \if de
 * Gibt an, um wieviel der Korrekturwert im encoderkompensierten Bewegungsmodus
 * in einer Iteration verändert wird.
 * \endif
 *
 * \if en
 * Defines the increment used to increase/decrease the correction value during
 * each control loop step/iteration.
 * \endif
 */
#define ENCODER_MOVEMENT_STEP 5

#endif /* CONFIG_H_ */
