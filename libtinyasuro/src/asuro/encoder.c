/**
 * @file
 *
 * \if de
 * Implementiert die in encoder.h spezifizierte Funktionalität.
 * \endif
 *
 * \if en
 * Implements the functionality specified in encoder.h
 * \endif
 *
 * @author Markus Jung
 *
 * @version 01.06.2010 \n
 *  Angelegt \n
 *  Erste Implementierung
 * @version 13.06.2010 \n
 *  Import von GoTurn aus der AsuroLib 2.80RC1 \n
 *  Optimierungen in GoTurn
 * @version 21.06.2010 \n
 *  + EncoderMovement* (encoderkompensierte Bewegung)
 * @version 14.08.2010 \n
 *  Internene Berechnungen der Encoder-Kalibrierung aus config.h hierher verschoben
 * @version 21.09.2013 \n
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

#include <stdlib.h>
#include <util/atomic.h>

#include <asuro/config.h>
#include <asuro/encoder.h>
#include <asuro/motor.h>
#include <asuro/sensors.h>
#include <asuro/time.h>
#include <util/math.h>

#ifdef __DOXYGEN__
/**
 * \if de
 * Wird für die Fixkommarechnung verwendet, um die Rechengenauigkeit zu erhöhen.
 * \endif
 *
 * \if en
 * Used for fixed point calculations to increase the calculation precision.
 * \endif
 */
#define ENC_DIVIDER
/**
 * \if de
 * Gibt den Multiplikationsfaktor zur Berechnung der Ticks je mm an.
 * Das Berechnungsergebnis muss noch durch ENC_DIVIDER dividiert werden
 * (Fixkommarechnung).
 * \endif
 *
 * \if en
 * The multiplier required to calculate the ticks per mm. The result has to
 * be divided by ENC_DIVIDER afterwards (fixed point math).
 * \endif
 */
#define ENC_GO_FACTOR

/**
 * \if de
 * Gibt den Multiplikationsfaktor zur Berechnung der Ticks je Grad an.
 * Das Berechnungsergebnis muss noch durch ENC_DIVIDER dividiert werden
 * (Fixkommarechnung).
 * \endif
 *
 * \if en
 * The multiplier required to calculate the ticks per degree. The result has
 * to be divided by ENC_DIVIDER afterwards (fixed point math).
 * \endif
 */
#define ENC_TURN_FACTOR
#else // !__DOXYGEN__
#define ENCODER_STATE_RIGHT   (1 << 0)
#define ENCODER_STATE_LEFT    (1 << 1)

#define ENC_DIVIDER (1UL << 16)
#define ENC_GO_FACTOR ((uint32_t) ((ENC_DIVIDER / ENC_MM_PER_TICK) + 0.5))
#define ENC_TURN_FACTOR \
    ((uint32_t) ((ENC_DIVIDER * ENC_TURN_TICKS_PER_360_DEGREE / 360) + 0.5))
#endif

#if !defined(ODO_EDGE_THRESHOLD) && defined(ODO_OFFSET)
#define ODO_EDGE_THRESHOLD ODO_OFFSET
#warning ODO_OFFSET is deprecated, use ODO_EDGE_THRESHOLD instead
#endif

/**
 * \if de
 * Die Referenzwerte für die Flankenerkennung. War die letzte Flanke
 * steigend/fallend, ist der gespeicherte Referenzwert der größte/kleinste
 * gemessene Wert seit Erkennung der Flanke.
 * \endif
 *
 * \if en
 * The reference value used for detection of falling or rising edges. If the
 * last edge has been rising/falling, the value stored here is the
 * largest/smallest value measured since detection of this edge.
 * \endif
 */
static SensorValues_t reference;
/**
 * \if de
 * Gibt an, ob die letzte Flanke steigend (1) oder fallend (0) war. Der Wert
 * der entsprechenden Seite kann über ENCODER_STATE_LEFT und ENCODER_STATE_RIGHT
 * ermittelt werden.
 * \endif
 *
 * \if en
 * Denotes if the last edge has been rising (1) or falling (0) for both left and
 * right hand side (bitmasked by ENCODER_STATE_LEFT and ENCODER_STATE_RIGHT).
 * \endif
 */
static uint8_t last_state;
volatile EncoderTicks_t encoderTicks;

/**
 * \if de
 * Sollgeschwindigkeit links (für encoderkompensierte Bewegung).
 * \endif
 *
 * \if en
 * Setpoint of the left motor (for encodercompensated movement mode).
 * \endif
 */
static uint8_t leftMovementSpeed;
/**
 * \if de
 * Sollgeschwindigkeit rechts (für encoderkompensierte Bewegung).
 * \endif
 *
 * \if en
 * Setpoint of the right motor (for encodercompensated movement mode).
 * \endif
 */
static uint8_t rightMovementSpeed;
/**
 * \if de
 * Kompensationswert für die encoderkompensierte Bewegung.
 * \endif
 *
 * \if en
 * Compensation value used for the encodercompensated movement mode.
 * \endif
 */
static int8_t deltaMovementSpeed;

void EncoderInit(void) {
    // reference 0 + last_state "high" -> adjustment to the current odometry
    // sensor value at next call of EncoderPoll()
    reference.left = 0;
    reference.right = 0;
    last_state = ENCODER_STATE_LEFT | ENCODER_STATE_RIGHT;

    EncoderReset();
    EncoderMovementReset();
}

void EncoderPoll(void) {
    bool updated;
    SensorValues_t newValues;

    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        updated = odoDataUpdated;
        odoDataUpdated = false;
        newValues = odoData;
    }

    if (updated) {
        uint8_t last_state_buf = last_state;

        uint8_t mask = ENCODER_STATE_LEFT;
        uint8_t new = newValues.left;
        uint8_t ref = reference.left;
        uint16_t ticks = encoderTicks.left;

        do {
            if (last_state_buf & mask) { // last edge: low->high
                if (ref < new) { // increase high value
                    ref = new;
                } else if ((uint8_t) (ref - new) > ODO_EDGE_THRESHOLD) { // falling edge
                    ref = new;
                    last_state_buf &= ~mask;
                    ticks++;
                }
            } else { // last edge: high->low
                if (ref > new) { // decrease low value
                    ref = new;
                } else if ((uint8_t) (new - ref) > ODO_EDGE_THRESHOLD) { // rising edge
                    ref = new;
                    last_state_buf |= mask;
                    ticks++;
                }
            }

            if (mask == ENCODER_STATE_LEFT) {
                // writeback LEFT, load RIGHT
                reference.left = ref;
                encoderTicks.left = ticks;
                new = newValues.right;
                ref = reference.right;
                ticks = encoderTicks.right;
            }
        } while (--mask > 0); // LEFT:(1 << 1): 2 -> RIGHT:(1 << 0): 1 -> 0
        // writeback RIGHT
        reference.right = ref;
        encoderTicks.right = ticks;

        last_state = last_state_buf;
    }
}

void EncoderReset(void) {
    encoderTicks.left = 0;
    encoderTicks.right = 0;
}

void GoTurn(int16_t distance, int16_t degree, uint8_t speed) {
    int16_t tick_count;
    uint8_t l_speed = speed;
    uint8_t r_speed = speed;

    if (distance != 0) { // either Go (distance) ...
        tick_count = (((uint16_t) abs(distance)) * ENC_GO_FACTOR) / ENC_DIVIDER;

        if (distance < 0)
            MotorDir(RWD, RWD);
        else
            MotorDir(FWD, FWD);
    } else { // ... or Turn (degree)
        tick_count = (((uint16_t) abs(degree)) * ENC_TURN_FACTOR) / ENC_DIVIDER;

        if (degree < 0)
            MotorDir(RWD, FWD);
        else
            MotorDir(FWD, RWD);
    }

    EncoderReset();
    MotorSpeed(l_speed, r_speed);

    while (tick_count > 0) {
        const EncoderTicks_t encoder = encoderTicks;

        tick_count -= encoder.left;

        if (encoder.left > encoder.right) {
            if ((l_speed > speed) || (r_speed > 244)) {
                l_speed -= 10;
            } else {
                r_speed += 10;
            }
        }

        if (encoder.left < encoder.right) {
            if ((r_speed > speed) || (l_speed > 244)) {
                r_speed -= 10;
            } else {
                l_speed += 10;
            }
        }

        EncoderReset(); // reset encoder counter after each iteration
        MotorSpeed(l_speed, r_speed);

        // sleep for 1ms without missing any new sensor measurments
        for (const uint8_t start = tick36kHz; (uint8_t) (tick36kHz - start) < 36;) {
            sleepUntilInterrupt();
            EncoderPoll();
        }
    }

    MotorSpeed(0, 0); // stop controlling the motors
}

void EncoderMovementReset(void) {
    leftMovementSpeed = 0;
    rightMovementSpeed = 0;
    deltaMovementSpeed = 0;
}

void EncoderMovementSetSpeed(const uint8_t left, const uint8_t right) {
    leftMovementSpeed = left;
    rightMovementSpeed = right;
}

void EncoderMovementPoll(void) {
    const EncoderTicks_t encoder = encoderTicks;
    EncoderReset();

    const uint8_t s_left = leftMovementSpeed;
    const uint8_t s_right = rightMovementSpeed;
    int8_t s_delta = deltaMovementSpeed;

    /*
     * Assumption: encoderTicks < 255 ( -> less than 50cm since last call)
     *
     * Compensate speed across both motors. The product ticks * opposite speed
     * should be equal. (E_l / E_r = S_l / S_r)
     */
    const int16_t deltaEnc = (encoder.left * s_right)
            - (encoder.right * s_left);

    if (deltaEnc < 0) { // left motor is too slow
        s_delta += ENCODER_MOVEMENT_STEP;
        if (s_delta > ENCODER_MOVEMENT_DELTA_LIMIT) {
            s_delta = ENCODER_MOVEMENT_DELTA_LIMIT;
        }
    } else if (deltaEnc > 0) { // right motor is too slow
        s_delta -= ENCODER_MOVEMENT_STEP;
        if (s_delta < -ENCODER_MOVEMENT_DELTA_LIMIT) {
            s_delta = -ENCODER_MOVEMENT_DELTA_LIMIT;
        }
    }

    deltaMovementSpeed = s_delta;

    uint8_t motorLeft;
    uint8_t motorRight;

    if (s_delta >= 0) { // add left, subtract right
        motorLeft = add8u_sat(s_left, s_delta);
        motorRight = sub8u_sat(s_right, s_delta);
    } else { // and vice versa: substract left, add right
        // the singularity -128 is eliminated by the chosen control limits
        uint8_t abs_delta = -s_delta;

        motorLeft = sub8u_sat(s_left, abs_delta);
        motorRight = add8u_sat(s_right, abs_delta);
    }

    MotorSpeed(motorLeft, motorRight);
}
