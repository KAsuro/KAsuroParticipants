/**
 * @file
 *
 *\if de
 * Definiert eine Schnittstelle für die Erfassung von Messwerten über den
 * AD-Wandler.
 *
 * Im Wesentlichen bietet dieses Modul drei Funktionalitäten:
 * @li Synchrones Auslesen der Batteriespannung (BatteryVoltage())
 * @li Synchrones Auslesen von Linien- und Odometriedaten (LineData() und
 *     OdometryData())
 * @li Asynchrones Auslesen von Linien- und Odometriedaten durch automatische
 *     Messwerterfassung (AutoMeasurmentMode)
 *
 * Die AutoMeasurmentMode erfasst zyklisch mit maximaler Samplegeschwindigkeit
 * die konfigurierten Kanäle, es können gleichzeitig die Daten von Liniensensor
 * und Odometriesensoren erfasst werden. Nachdem neue Werte erfasst wurden,
 * wird das entsprechende Flag (odoDataUpdated und lineDataUpdated) gesetzt.
 *
 * Der AutoMeasurmentMode erfasst Messwerte mit acht Bit Genauigkeit.
 *
 * Zusätzlich besteht die Möglichkeit, eine eigene Funktion in der ADC-ISR
 * (also nach Abschluss jeder Wandlung im AutoMeasurmentMode) auszuführen,
 * sofern diese Option in hooks.h aktiviert ist.
 *
 * @par Initialisierung:
 *      Vor der Verwendung des Moduls muss SensorInit() aufgerufen werden,
 *      ferner muss der ADC (adc.h) initialisiert werden, der AutoMeasurmentMode
 *      erfordert zudem eine globale Interruptfreigabe.
 *
 * @par Genutzte Softwaremodule:
 *      @li adc.h
 *
 * @par Genutzte Hardwareressourcen:
 *      Interruptvektoren:
 *      @li ADC
 *
 *      I/O:
 *      @li Port C: Pin 0 bis Pin 3, Pin 5
 * \endif
 *
 * \if en
 * Interface definitions for capturing sensor data using the ADC.
 *
 * This module offers essentially three functionalities:
 * @li Synchronous readout of the battery voltage (BatteryVoltage())
 * @li Synchronous readout of the line and odometry sensors (LineData() and
 *     OdometryData())
 * @li Asynchronous readout of the line and odometry sensors using the
 *     AutoMeasurmentMode
 *
 * The AutoMeasurmentMode captures all selected input channels in an alternating
 * manner using the highest possible sampling rate. Supported channels are the
 * line sensors and the odometry sensors. A flag byte (odoDataUpdated and
 * lineDataUpdated) will be set every time a measurment of the corresponding
 * input channel has been finished.
 *
 * The AutoMeasurmentMode provides a capture resulution of 8 bits.
 *
 * The AutoMeasurmentMode uses the ADC interrupt. Custom code can be called from
 * the context of this ISR if the according hook has been enabled in hooks.h.
 * The hook function will be executed every time a ADC measurment has been
 * finished.
 *
 * @par Initialization:
 *      SensorInit() must be called before using this module. The ADC (adc.h) is
 *      required to be initialized additionally. Since the AutoMeasurmentMode
 *      depends on the ADC interrupt, interrupts must be globally enabled if
 *      this feature is in use.
 *
 * @par Software interfaces in use:
 *      @li adc.h
 *
 * @par Hardware components in use:
 *      Interrupt vectors:
 *      @li ADC
 *
 *      I/O:
 *      @li Port C: Pin 0 up to pin 3, pin 5
 * \endif
 *
 * @see config.h
 * @see hooks.h
 *
 * @author Markus Jung
 *
 * @version 18.05.2010 \n
 *  Erste Version angelegt \n
 *  Makros entsprechend asuro.h angelegt
 * @version 29.05.2010 \n
 *  Interruptbetrieb in Header übernommen \n
 *  Struktur und Typdefinitionen
 * @version 15.06.2010 \n
 *  Doku
 * @version 16.08.2010 \n
 *  + Hook für ADCISR
 * @version 17.12.2011 \n
 *  Odometrie -> Odometry
 * @version 15.09.2013 \n
 *  Add BatteryVoltage() function for direct battery voltage measurment
 * @version 16.09.2013 \n
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

#ifndef SENSORS_H_
#define SENSORS_H_

#include <stdint.h>
#include "hooks.h"
#include "led.h"

#ifdef __DOXYGEN__
/**
 * \if de
 * Dient zum Elementzugriff auf die von LineData() und OdometryData()
 * erfassten Messwerte des (in Fahrtrichtung vorwärts) linken Sensors.
 * \endif
 *
 * \if en
 * Used to access the measurment of the sensor at the left hand side (relative
 * to the forward driving direction) captured from LineData() and OdometryData().
 * \endif
 */
#define LEFT
/**
 * \if de
 * Dient zum Elementzugriff auf die von LineData() und OdometryData()
 * erfassten Messwerte des (in Fahrtrichtung vorwärts) rechten Sensors.
 * \endif
 *
 * \if en
 * Used to access the measurment of the sensor at the right hand side (relative
 * to the forward driving direction) captured from LineData() and OdometryData().
 * \endif
 */
#define RIGHT
#else
#define LEFT    0
#define RIGHT   1
#endif

/**
 * \if de
 * Definiert Masken für die verschiedenen Betriebsarten des AutoMeasurmentMode.
 * \endif
 *
 * \if en
 * Defines bitmasks used to select the differen operation modes for the
 * AutoMeasurmentMode.
 * \endif
 */
typedef enum {
    /**
     * \if de
     * Kein aktiver AutoMeasurmentMode
     * \endif
     *
     * \if en
     * AutoMeasurmentMode off
     * \endif
     */
    am_off = 0,
    /**
     * \if de
     * AutoMeasurmentMode für die Odometriesensoren
     * \endif
     *
     * \if en
     * AutoMeasurmentMode capturing the odometry sensors
     * \endif
     */
    am_odo = (1 << 0),
    /**
     * \if de
     * AutoMeasurmentMode für die Liniensensoren
     * \endif
     *
     * \if en
     * AutoMeasurmentMode capturing the line sensors
     * \endif
     */
    am_line = (1 << 2)
} AutoMeasurmentMode_t;

/**
 * \if de
 * Kapselt im AutoMeasurmentMode erfasste Daten.
 * \endif
 *
 * \if en
 * A structure used to store the measurments captured in AutoMeasurmentMode.
 * \endif
 */
typedef struct {
    uint8_t right;
    uint8_t left;
} SensorValues_t;

/**
 * \if de
 * Speichert die im AutoMeasurmentMode erfassten Odometriedaten.
 * Neue Daten werden durch odoDataUpdated signalisiert, ein atomarer Zugriff auf
 * odoData kann nur bei abgeschalteten Interrupts garantiert werden.
 * \endif
 *
 * \if en
 * Stores the odometry sensor measurments captured by the AutoMeasurmentMode.
 * New data will be signaled by setting the flag byte odoDataUpdated. An atomic
 * access to odoData can only be guaranteed if interrupts are disabled globally.
 * \endif
 */
extern volatile SensorValues_t odoData;
/**
 * \if de
 * Diese Variable signalisiert, ob neue Odometrie-Daten im AutoMeasurmentMode
 * erfasst wurden.
 * \endif
 *
 * \if en
 * This flag variable is used to signal the availableness of new odometry sensor
 * measurments captured by the AutoMeasurmentMode.
 * \endif
 */
extern volatile bool odoDataUpdated;

/**
 * \if de
 * Speichert die im AutoMeasurmentMode erfassten Liniensensordaten.
 * Neue Daten werden durch lineDataUpdated signalisiert, ein atomarer Zugriff auf
 * lineData kann nur bei abgeschalteten Interrupts garantiert werden.
 * \endif
 *
 * \if en
 * Stores the line sensor measurments captured by the AutoMeasurmentMode.
 * New data will be signaled by setting the flag byte lineDataUpdated. An atomic
 * access to lineData can only be guaranteed if interrupts are disabled globally.
 * \endif
 */
extern volatile SensorValues_t lineData;
/**
 * \if de
 * Diese Variable signalisiert, ob neue Liniensensor-Daten im AutoMeasurmentMode
 * erfasst wurden.
 * \endif
 *
 * \if en
 * This flag variable is used to signal the availableness of new line sensor
 * measurments captured by the AutoMeasurmentMode.
 * \endif
 */
extern volatile bool lineDataUpdated;

#ifdef __DOXYGEN__
/**
 * \if de
 * Bietet die Möglichkeit, eine eigene Funktion bei jedem Aufruf der ADC-ISR
 * auszuführen. Die interne Funktionalität bleibt dabei erhalten, erst im Anschluss
 * an diese wird die über ADCISRHook referenzierte Funktion aufgerufen.
 *
 * Der Aufruf dieser Funktion erfolgt mit freigegebenen Interrupts!
 *
 * Ist (ADCISRHook == NULL), findet kein Aufruf statt.
 *
 * Diese Funktionalität steht nicht zur Verfügung, wenn ADC_ISR_HOOK_ENABLE
 * deaktiviert ist.
 * \endif
 *
 * \if en
 * Offers the ability to execute a custom function in addition to the internal
 * code of the ADC ISR. The function referenced by ADCISRHook will be called
 * after execution of the internal code required for the AutoMeasurmentMode.
 *
 * The function will be called from an interrupt context, but interrupts will be
 * globally enabled!
 *
 * No function call will be executed if (ADCISRHook == NULL).
 *
 * The ADC hook feature is not available if ADC_ISR_HOOK_ENABLE has been
 * disabled.
 * \endif
 */
extern ISRHookFunc_t *ADCISRHook;
#else // !__DOXYGEN__
#if ADC_ISR_HOOK_ENABLE
extern ISRHookFunc_t *ADCISRHook;
#endif
#endif

/**
 * \if de
 * Initialisiert die Messwerterfassung, den internen Status für den
 * AutoMeasurmentMode sowie die Analog-Signaleingänge für Odometrie-,
 * Linienfolge- und Batteriespannungsmessungen.
 * \endif
 *
 * \if en
 * Initializes the internal structures of this module and configures the
 * signal input pins used to capture sensor input from the odometry and line
 * sensors as well as the battery voltage.
 * \endif
 */
extern void SensorInit(void);
/**
 * \if de
 * Erfasst die Werte der Liniensensoren synchron (blockierend).
 *
 * @param data ein Zeiger auf ein Array der Länge zwei, data[LEFT] speichert
 *             die Daten des linken Liniensensors, data[RIGHT] die des rechten.
 * \endif
 *
 * \if en
 * Captures a measurment of the line sensors in a synchronous (blocking) manner.
 *
 * @param data a pointer to an array of size two. data[LEFT] will contain the
 *             measurment of the left line sensor, data[RIGHT] the measurment of
 *             the right one.
 * \endif
 */
extern void LineData(uint16_t * const data);
/**
 * \if de
 * Erfasst die Werte der Odometriesensoren synchron (blockierend).
 *
 * @param data ein Zeiger auf ein Array der Länge zwei, data[LEFT] speichert
 *             die Daten des linken Odometriesensors, data[RIGHT] die des rechten.
 * \endif
 *
 * \if en
 * Captures a measurment of the odometry sensors in a synchronous (blocking)
 * manner.
 *
 * @param data a pointer to an array of size two. data[LEFT] will contain the
 *             measurment of the left odometry sensor, data[RIGHT] the
 *             measurment of the right one.
 * \endif
 */
extern void OdometryData(uint16_t * const data);
/**
 * @see OdometryData()
 *
 * \if de
 * @deprecated Wird im Zuge der Schnittstellenvereinheitlichung durch
 *             OdometryData() ersetzt.
 * \endif
 *
 * \if en
 * @deprecated Replaced by OdometryData() for interface consolidation purposes.
 * \endif
 */
extern void OdometrieData(uint16_t * const data);
/**
 * \if de
 * Erfasst die aktuelle Batteriespannung synchron (blockierend). Der ermittelte
 * Spannungswert wird in 1/10 Volt angegeben, ein Wert von 42 entsprich damit
 * einer Batteriespannung von 4,2V. Nicht berücksichtigt wird der Status der
 * Steckbrücke JP1, ist diese nicht gesetzt liegt die tatsächliche
 * Batteriespannung um ca. 0,6V höher.
 *
 * Es stehen zwei Messmethoden zur Verfügung, die Auswahl erfolgt in config.h,
 * ebenso kann die gemessene Spannung dort mit einem Kalibrierfaktor angepasst
 * werden.
 *
 * @return die gemessene Batteriespannung in 1/10V
 * \endif
 *
 * \if en
 * Measures the current battery voltage in a synchronous (blocking) manner. The
 * measured voltage will be represented in units of 1/10 volt, a value of 42
 * equals therefore a battery voltage of 4,2V. The state of jumper JP1 is not
 * incorporated into the measurment, the actual battery voltage will be about
 * 0,6V higher as measured if the jumper is not set.
 *
 * This function implements two different measurment methods which can be
 * selected and calibrated in config.h.
 *
 * @return the measured battery voltage in 1/10V units
 * \endif
 */
extern uint8_t BatteryVoltage(void);

/**
 * \if de
 * Konfiguriert/aktiviert den AutoMeasurmentMode.
 *
 * Im AutoMeasurmentMode können die Linien- und Odometriesensoren zyklisch
 * (mit maximaler Samplerate) asynchron ausgelesen werden. Ist die Erfassung
 * eines Sensorblocks abgeschlossen, wird dies über die entsprechende Variable
 * signalisiert.
 *
 * Der AutoMeasurmentMode erfordert eine globale Interruptfreigabe!
 *
 * @param mode der zu aktivierende AutoMeasurmentMode, kann am_off, am_odo,
 *             am_line oder (am_odo | am_line) sein.
 * \endif
 *
 * \if en
 * Configures/enables the AutoMeasurmentMode.
 *
 * The AutoMeasurmentMode captures measurments of the line and odometry sensors
 * at the highes possible sampling rate. The measurments are performed in an
 * alternating manner (odometry right and left, line right and left). Flag
 * bytes signal a finished measurment of a capture channel (odometry or line).
 *
 * The AutoMeasurmentMode requires interrupts to be globally enabled!
 *
 * @param mode the desired AutoMeasurmentMode, valid values are am_off, am_odo,
 *             am_line or (am_odo | am_line).
 * \endif
 */
extern void SensorConfigAutomode(const AutoMeasurmentMode_t mode);

#endif /* SENSORS_H_ */
