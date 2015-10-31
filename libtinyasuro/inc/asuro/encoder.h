/**
 * @file
 *
 * \if de
 * Definiert eine Schnittstelle für die Auswertung der Odometriesensoren,
 * sowie der Navigation basierend auf diesen Daten.
 *
 * Bei der Auswertung der Odometriedaten werden die Farbwechsel der
 * Encoderscheiben gezählt. Die Einheit "Tick" bezeichnet dabei einen
 * Farbwechsel (also eine erkannte Signalflanke).
 *
 * Soll eine Auswertung der Odometriemesswerte stattfinden, während GoTurn()
 * nicht verwendet wird, muss EncoderPoll() regelmäßig aufgerufen werden,
 * die Aufruffrequenz sollte sich dabei an der Samplingrate des ADC orientieren.
 * Als Anhaltspunkt dafür können auch die Statusflags von sensors.h dienen.
 *
 * @par Wichtiger Hinweis:
 *      Alle Funktionen basieren auf der Verwendung der Variable encoderTicks.
 *      Um deren korrekte Ausführung sicherzustellen muss eine "parallele"
 *      Ausführung dieser Funktionen (etwa durch eine ISR die neben dem
 *      Hauptprogramm ebenfalls Funktionen aus diesem Modul verwendet)
 *      verhindert werden. Gleiches gilt für direkte Zugriffe auf encoderTicks.
 *
 * Als Alternative zu MotorSpeed() (motor.h) wird der "encoderkompensierte
 * Bewegungsmodus" angeboten. Dieser kompensiert (mechanische) Unterschiede
 * zwischen linker und rechter Antriebsseite und so sorgt dafür, dass das
 * Verhältnis der Raddrehzahlen dem der Sollgeschwindigkeiten entspricht.
 *
 * @par Initialisierung:
 *      Vor der Verwendung dieses Moduls muss EncoderInit() aufgerufen werden. \n
 *      Wenn Odometriedaten erfasst werden sollen, muss der AutoMeasurmentMode
 *      für die Odometriesensoren aktiviert werden, ebenso müssen die
 *      Odometriesensoren selbst aktiviert werden.
 *
 * @par
 *      Beispielinitialisierung:
 * @bcode{
 * EncoderInit();
 * LED_CONFIGURE_ODOMETRY(); // led.h
 * SensorInit(); // sensors.h
 * SensorConfigAutomode(am_odo); // ebenfalls sensors.h
 * }
 *
 * @par Genutzte Softwaremodule:
 *      @li motor.h
 *      @li sensors.h
 *      @li time.h
 *
 * Die Implementierung von GoTurn() basiert auf der gleichnamigen Funktion der
 * Autoren Andun, Sternthaler und stochri aus der AsuroLib Version 2.80RC1.
 * \endif
 *
 * \if en
 * Interface definitions for odometry sensor data analysis and navigation
 * based on those sensor data.
 *
 * The data provided by the odometry sensors is analysed for color transitions
 * of the encoder discs (called sensor markers in the ASURO manual). Those
 * color transitions will be counted to measure distances. The unit "tick"
 * denotes one color transition (which means one detected rising or falling edge
 * of the sensor signal)
 *
 * The analysis of the odometry sensor data is done by EncoderPoll(). Blocking
 * functions like GoTurn() call EncoderPoll() internally to gather the required
 * movement informations. If other code depends on those distance informations,
 * EncoderPoll() needs to be called periodically. For best results, the polling
 * frequency should be based to the sampling rate of the ADC, the status flags
 * of sensors.h can be used as reference.
 *
 * @par Important note:
 *      All functions expect to have exclusive access to the variable
 *      encoderTicks. To ensure proper operation, they must not be called in
 *      parallel, for example by calling EncoderPoll() from an ISR while
 *      GoTurn() or EncoderMovementPoll() has been called by the main program.
 *      The same applies if encoderTicks is accessed directly.
 *
 * This module offers an alternative to MotorSpeed() (motor.h), the
 * "encodercompensated movement mode". This mode compensates (mechanical)
 * difference between the left and right power train and ensures the desired
 * speed ratio between the two wheels.
 *
 * @par Initialization:
 *      Befure using this module, EncoderInit() has to be called first. \n
 *      Gathering movmement information from the odometry sensors requires to
 *      enable the AutoMeasurmentMode for those sensors. The odometry sensors
 *      themselves need to be activated, too.
 *
 * @par
 *      Initialization example:
 * @bcode{
 * EncoderInit();
 * LED_CONFIGURE_ODOMETRY(); // led.h
 * SensorInit(); // sensors.h
 * SensorConfigAutomode(am_odo); // ebenfalls sensors.h
 * }
 *
 * @par Software modules in use:
 *      @li motor.h
 *      @li sensors.h
 *      @li time.h
 *
 * The implementation of GoTurn() is based on the equally named function from
 * the authors Andun, Sternthaler and stochri and has been taken from the
 * AsuroLib version 2.80RC1.
 * \endif
 *
 * @see config.h
 * @see sensors.h
 *
 * @version 01.06.2010 \n
 *  Angelegt \n
 *  Erstimplementierung
 * @version 13.06.2010 \n
 *  Import von GoTurn aus der AsuroLib 2.80RC1 \n
 *  Optimierungen in GoTurn \n
 *  Doku GoTurn
 * @version 14.06.2010 \n
 *  Doku vervollständigt
 * @version 21.06.2010 \n
 *  + EncoderMovement* (encoderkompensierte Bewegung)
 * @version 15.08.2010 \n
 *  + Kompatibilitätsmakros Go() und Turn()
 * @version 17.12.2011 \n
 *  Odometrie -> Odometry
 * @version 10.09.2013 \n
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

#ifndef ENCODER_H_
#define ENCODER_H_

#include <stdint.h>

#ifdef __DOXYGEN__
/**
 * \if de
 * Entspricht einem Aufruf von GoTurn(distance, 0, speed).
 * \endif
 *
 * \if en
 * An alias for GoTurn(distance, 0, speed).
 * \endif
 */
#define Go(distance, speed)
/**
 * \if de
 * Entspricht einem Aufruf von GoTurn(0, degree, speed).
 * \endif
 *
 * \if en
 * An alias for GoTurn(0, degree, speed);
 * \endif
 */
#define Turn(degree, speed)
#else
#define Go(distance, speed) GoTurn(distance, 0, speed)
#define Turn(degree, speed) GoTurn(0, degree, speed)
#endif

/**
 * \if de
 * Kapselt die gezählten Ticks des linken und rechten Odometriesensors.
 * \endif
 *
 * \if en
 * The data structure encapsulating the tick counters for both the left and the
 * right hand side odometry sensor.
 * \endif
 */
typedef struct {
    uint16_t left;
    uint16_t right;
} EncoderTicks_t;

/**
 * \if de
 * Die zum Zählen der Ticks verwendete Variable, je nach Zugriffsart und
 * Aufrufkontext von EncoderPoll() (ISR!) ist unter Umständen ein atomarer
 * Zugriff erforderlich.
 * \endif
 *
 * \if en
 * The variable used to count the movement ticks. Depending on the way how
 * EncoderPoll (ISR!) is called, it might be necessary to access this struct
 * in an atomic way.
 * \endif
 */
extern volatile EncoderTicks_t encoderTicks;

/**
 * \if de
 * Initialisiert das Encoder-Modul und setzt den internen Status zurück.
 * \endif
 *
 * \if en
 * Initializes the encoder module and resets its internal state.
 * \endif
 */
extern void EncoderInit(void);
/**
 * \if de
 * Prüft ob neue Odometriedaten erfasst wurden und Wertet diese aus.
 *
 * Diese Funktion sollte periodisch aufgerufen werden, um zu verhindern dass
 * Messwerte "verpasst" werden. Der Aufruf von EncoderPoll() aus einer ISR
 * kann Probleme bei der Verwendung anderer encoder-basierten Funktionen
 * verursachen.
 *
 * Wird die Odometrieauswertung nicht benötigt, muss diese Funktion nicht
 * aufgerufen werden.
 * \endif
 *
 * \if en
 * Checks if new odometry data have been captured and analyses them.
 *
 * This function should be called periodically, new measurments might be missed
 * otherwise. Calling EncoderPoll() from an ISR can cause issues if other
 * encoder-based functions are used, too.
 *
 * There is no need to call this function if the odometry measurments are not
 * required.
 * \endif
 */
extern void EncoderPoll(void);
/**
 * \if de
 * Setzt den Zählerstand von encoderTicks zurück.
 * \endif
 *
 * \if en
 * Resets the counters of encoderTicks.
 * \endif
 */
extern void EncoderReset(void);

/**
 * \if de
 * Fährt eine bestimmte Strecke ODER dreht mit vorgegebenem Winkel
 * unter Berücksichtigung der vorgegebenen Geschwindigkeit.
 *
 * Nachdem die/der vorgegebene Strecke/Winkel zurückgelegt wurde, schaltet
 * GoTurn() die Motorgeschwindigkeit auf 0, ohne dass die Richtungsvorgabe
 * modifiziert wird.
 *
 * Während GoTurn() aktiv ist, darf EncoderPoll() nicht aufgerufen werden (etwa
 * durch ISRs), die Auswertung der Odometriedaten erfolgt automatisch.
 *
 * @warning Die Funktion blockiert bis das vorgegebene Fahrziel erreicht wurde.
 *          Damit GoTurn() neue Odometriedaten erhält, müssen Interrupts während
 *          des Aufrufs freigeschaltet sein, eine Verwendung innerhalb einer
 *          "gewöhnlichen" ISR ist also nicht möglich!
 *
 * Basiert auf GoTurn() aus der AsuroLib 2.80RC1.
 *
 * @param distance die zu fahrende Strecke in mm, es muss gelten:
 *                 @bcode{INT16_MAX < abs(distance) / ENC_MM_PER_TICK}
 *                 Das Vorzeichen bestimmt die Fahrtrichtung,
 *                 ein Wert von 0 wechselt in den Rotationsmodus.
 * @param degree der Rotationswinkel in Grad, es muss gelten:
 *               @bcode{ENC_TURN_TICKS_PER_360_DEGREE / 360 * abs(degree) < INT16_MAX}
 *               Das Vorzeichen bestimmt die Drehrichtung,
 *               positive Werte führen zu einer Rotation im Uhrzeigersinn.
 * @param speed Geschwindigkeitsvorgabe, es muss gelten: @bcode{10 <= speed <= 255}
 *              Laut ASURO-Manual setzt sich der ASURO ab einem Wert von ca. 60
 *              in Bewegung.
 * \endif
 *
 * \if en
 * Drives a specified distance OR turns by a given angle, with respect to a
 * defined speed value.
 *
 * The motor speed is set to 0 by GoTurn() after driving/turning the specified
 * distance/angle, without changing the configured motor directions.
 *
 * EncoderPoll() must not be called while GoTurn() is active (for example from
 * an interrupt context), the encoders will be monitored by GoTurn() itself.
 *
 * @warning This function returns not until the specified target distance/angle
 *          has been reached. GoTurn() only gets the required sensor input
 *          if interrupts are enabled globally to keep AutoMeasurmentMode
 *          working. GoTurn() can not be used within a "regular" ISR for this
 *          reason.
 *
 * Based on GoTurn() from AsuroLib 2.80RC1
 *
 * @param distance the distance to drive, specified in mm. Valid range:
 *                 @bcode{INT16_MAX < abs(distance) / ENC_MM_PER_TICK}
 *                 The sign denotes the driving direction, a value of 0 switches
 *                 into rotation mode.
 * @param degree the angle to turn, specified in degrees. Valid range:
 *               @bcode{ENC_TURN_TICKS_PER_360_DEGREE / 360 * abs(degree) < INT16_MAX}
 *               The sign denotes the turn direction, the positive turn
 *               direction is clockwise.
 * @param speed the desired driving speed. Valid range: @bcode{10 <= speed <= 255}
 *              According to the ASURO manual, the ASURO starts to move at
 *              speed values of about 60.
 * \endif
 *
 * @author stochri
 * @author Andun
 * @author Sternthaler
 * @author Markus Jung
 */
extern void GoTurn(int16_t distance, int16_t degree, uint8_t speed);

/**
 * \if de
 * Setzt den Status des encoderkompensierten Bewegungsmodus zurück.
 * \endif
 *
 * \if en
 * Resets the state of the encodercompensated movement mode.
 * \endif
 */
extern void EncoderMovementReset(void);
/**
 * \if de
 * Setzt die Sollgeschwindigkeiten für den encoderkompensierten Bewegungsmodus.
 *
 * @param left die Sollgeschwindigkeit des linken Motors
 * @param right die Sollgeschwindigkeit des rechten Motors
 * \endif
 *
 * \if en
 * Configures the desired speed for the encodercompensated movement mode.
 *
 * @param left setpoint for the left motor
 * @param right setpoint for the right motor
 * \endif
 */
extern void EncoderMovementSetSpeed(const uint8_t left, const uint8_t right);
/**
 * \if de
 * Regelt die Geschwindigkeit für den encoderkompensierten Bewegungsmodus
 * entsprechend der mit EncoderMovementSetSpeed() gesetzten Sollwerte. Weicht
 * das Verhältnis der Encoderwerte links/rechts von dem der Geschwindigkeiten
 * ab, wird der zu schnelle Motor abgebremst, der zu langsame beschleunigt.
 *
 * Für die Regelung werden Encoderdaten benötigt, es muss daher gewährleistet
 * sein, dass EncoderPoll() mit einer ausreichend hohen Frequenz aufgerufen wird.
 * Beim Aufruf von EncoderMovementPoll() werden die Encoder-Zähler mittels
 * EncoderReset() zurückgesetzt.
 *
 * Diese Funktion sollte abhängig von der Fahrtgeschwindigkeit mit einer Frequenz
 * von 10Hz bis 100Hz aufgerufen werden, bei zu hoher Frequenz werden zu wenige
 * Ticks von den Encoderscheiben erfasst. Eine zu niedrige Aufruffrequenz kann
 * zu Fehlverhalten führen, da die Funktion annimmt, dass weniger als 255 
 * encoderTicks (etwa 50 cm) seit dem letzten Aufruf zurückgelegt wurden.
 * \endif
 *
 * \if en
 * Controls the motor speeds in encodercompensated movement mode according to
 * the setpoints configured by EncoderMovementSetSpeed(). This mode tries to
 * maintain a left/right speed ratio which is the same as the ratio of the two
 * setpoints. This is done by speeding up the motor which has been to slow while
 * slowing down the other one.
 *
 * Encoder data are required for the control task, EncoderPoll() must be
 * therefore called periodically at a sufficiently high frequency.
 * As part of EncoderMovementPoll(), the encoder counters are reset using
 * EncoderReset().
 *
 * This function should be called at a frequency of about 10Hz to 100Hz,
 * depending on the desired drive speed. At the one hand, calling the function
 * too often results in a worse control behaviour because the driven
 * distance respectively the measured encoder ticks is/are too low to make a
 * good control decision. But at the other hand the internal computations are
 * expecting the tick counters in encoderTicks to contain values less or equal
 * to 255, which corresponds to a driven distance of about 50 cm. Failing to
 * meet this requirement can lead to undefined behaviour.
 * \endif
 *
 * @see config.h: ENCODER_MOVEMENT_DELTA_LIMIT
 * @see config.h: ENCODER_MOVEMENT_STEP
 */
extern void EncoderMovementPoll(void);

#endif /* ENCODER_H_ */
