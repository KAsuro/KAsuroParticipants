/**
 * @file
 *
 * \if de
 * Implementiert die Selbsttest-Teilfunktionen.
 * \endif
 *
 * \if en
 * Implements the self test functions.
 * \endif
 *
 * @author Markus Jung
 *
 * @version 28.08.2010 \n
 *  Angelegt
 * @version 29.08.2010 \n
 *  Vollständige Implementation
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

#include <stdint.h>
#include "selftest.h"

void SerialTest(void) {
    uint8_t countdown = 150; // x100ms = at most 15s

    do {
        char buf;

        StatusLED(GREEN);
        if (SerRead(&buf, 1, 3600) != 0) { // wait at most 100ms
            buf++;
            sleep(2); // USB transceivers have some echo filtering -> short delay
        } else {
            buf = 'T';
            countdown--;
        }

        StatusLED(RED);
        SerWrite(&buf, 1);
    } while (countdown > 0);

    StatusLED(OFF);
}

void SwitchTest(void) {
    MotorDir(FWD, BREAK);
    LED_CONFIGURE_BACK();

    for (uint16_t i = 0; i < 1500; i++) {
        const uint8_t switches = PollSwitch();

        StatusLED(((switches & SWITCH_1) ? GREEN : OFF)
                | ((switches & SWITCH_2) ? RED : OFF));
        FrontLED((switches & SWITCH_3) ? ON : OFF);
        BackLEDFast(((switches & SWITCH_4) ? BACK_LED_LEFT : OFF)
                | ((switches & SWITCH_5) ? BACK_LED_RIGHT : OFF));
        MotorSpeed((switches & SWITCH_6) ? 200 : 0, 0);

        msleep(10);
    }

    StatusLED(OFF);
    FrontLED(OFF);
    BackLEDFast(OFF);
    MotorSpeed(0, 0);
    MotorDir(BREAK, BREAK);
}

void LineTest(void) {
    LED_CONFIGURE_BACK();
    StatusLED(GREEN);

    uint16_t data[2];

    for (uint16_t i = 0; i < 1000; i++) {
        LineData(data);
        BackLEDFast((data[LEFT] > 400 ? BACK_LED_LEFT : OFF)
                | (data[RIGHT] > 400 ? BACK_LED_RIGHT : OFF));
        msleep(10);
    }

    BackLEDFast(OFF);
    StatusLED(OFF);
}

void OdometryTest(void) {
    FrontLED(ON);

    uint16_t data[2];

    for (uint16_t i = 0; i < 1500; i++) {
        OdometryData(data);
        StatusLED(((data[LEFT] < 512) ? GREEN : OFF)
                | ((data[RIGHT] < 512) ? RED : OFF));
        msleep(10);
    }

    StatusLED(OFF);
    FrontLED(OFF);
}

void MotorTestRight(void) {
    LED_CONFIGURE_BACK();
    BackLEDFast(BACK_LED_BOTH);

    MotorDir(BREAK, FWD);
    for (uint8_t loops = 0; loops < 2; loops++) {
        for (uint8_t speed = 0; speed < 255; speed++) {
            MotorSpeed(0, speed);
            msleep(10);
        }
        for (uint8_t speed = 254; speed > 0; speed--) {
            MotorSpeed(0, speed);
            msleep(10);
        }
        MotorDir(BREAK, RWD);
    }

    MotorDir(BREAK, BREAK);
    MotorSpeed(0, 0);
    BackLEDFast(OFF);
}

void MotorTestLeft(void) {
    LED_CONFIGURE_BACK();
    BackLEDFast(BACK_LED_BOTH);

    MotorDir(FWD, BREAK);
    for (uint8_t loops = 0; loops < 2; loops++) {
        for (uint8_t speed = 0; speed < 255; speed++) {
            MotorSpeed(speed, 0);
            msleep(10);
        }
        for (uint8_t speed = 254; speed > 0; speed--) {
            MotorSpeed(speed, 0);
            msleep(10);
        }
        MotorDir(RWD, BREAK);
    }

    MotorDir(BREAK, BREAK);
    MotorSpeed(0, 0);
    BackLEDFast(OFF);
}

void MotorTestBoth(void) {
    LED_CONFIGURE_BACK();
    BackLEDFast(BACK_LED_BOTH);

    MotorDir(FWD, FWD);
    for (uint8_t loops = 0; loops < 2; loops++) {
        for (uint8_t speed = 0; speed < 255; speed++) {
            MotorSpeed(speed, speed);
            msleep(10);
        }
        for (uint8_t speed = 254; speed > 0; speed--) {
            MotorSpeed(speed, speed);
            msleep(10);
        }
        MotorDir(RWD, RWD);
    }

    MotorDir(BREAK, BREAK);
    MotorSpeed(0, 0);
    BackLEDFast(OFF);
}

void LEDTest(void) {
    StatusLED(GREEN);
    msleep(3000);
    StatusLED(RED);
    msleep(3000);
    StatusLED(OFF);
    FrontLED(ON);
    msleep(3000);
    FrontLED(OFF);
    LED_CONFIGURE_BACK();
    BackLEDFast(BACK_LED_LEFT);
    msleep(3000);
    BackLEDFast(BACK_LED_RIGHT);
    msleep(3000);
    GREEN_LED_ON();
    RED_LED_ON();
    FrontLED(ON);
    BackLEDFast(BACK_LED_BOTH);
    msleep(3000);
    GREEN_LED_OFF();
    RED_LED_OFF();
    FrontLED(OFF);
    BackLEDFast(OFF);
}
