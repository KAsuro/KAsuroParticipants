/**
 * @file
 *
 * \if de
 * Eine Reimplementierung des ASURO-Selbsttests unter Verwendung der Tiny ASURO
 * Library. Die Demo-Funktionen wurde nicht implementiert.
 * \endif
 *
 * \if en
 * A reimplementation ofh the ASURO self test functionality using te tiny ASURO
 * library instad of the original (DLR) one. The additional demo functions
 * have not been implemented.
 * \endif
 *
 * @author Markus Jung
 *
 * @version 29.08.2010 \n
 *  Implementierung
 * @version 30.08.2010 \n
 *  Doku
 * @version 17.12.2011 \n
 *  Odometrie -> Odometry
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

#include <asuro/asuro.h>
#include <util/misc.h>
#include "selftest.h"

/**
 * \if de
 * Führt den Selbsttest in einer Endlossschleife aus.
 * \endif
 *
 * \if en
 * Runs the self test in an endless loop.
 * \endif
 */
MAIN void main(void) {
    Init();

    for (;;) {
        SerWrite("\r\n-- ASURO Testing --\r\n", 23);

        SerWrite("LED Test\r\n", 10);
        LEDTest();

        SerWrite("Line Test\r\n", 11);
        LineTest();

        SerWrite("Switch Test\r\n", 13);
        SwitchTest();

        SerWrite("Odometry Test\r\n", 16);
        OdometryTest();

        SerWrite("Motor Test\r\n", 12);
        MotorTestLeft();
        MotorTestRight();
        MotorTestBoth();

        SerWrite("Serial Test\r\n", 13);
        SerialTest();
    };
}
