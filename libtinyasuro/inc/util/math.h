/**
 * @file
 *
 * \if de
 * Definiert/Implementiert verschiedene spezielle mathematische Operationen.
 * \endif
 *
 * \if en
 * Defines/implements various mathematical operations for special cases.
 * \endif
 *
 * @author Markus Jung
 *
 * @version 28.05.2010 \n
 *  Saturierte Addition und Subtraktion
 * @version 17.06.2010 \n
 *  Neues Benennungsschema
 * @version 18.06.2010 \n
 *  Doku, Modifikationen für besseren Doxygen-Support
 * @version 29.06.2010 \n
 *  Korrekturen im Inline-ASM-Code
 * @version 13.04.2013 \n
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

#ifndef MATH_H_
#define MATH_H_

#include <stdint.h>
#include "misc.h"

#if !defined(__DOXYGEN__)
static ALWAYS_INLINE CONST uint8_t add8u_sat(uint8_t x, const uint8_t y);
static ALWAYS_INLINE CONST uint8_t sub8u_sat(uint8_t x, const uint8_t y);
#else
ALWAYS_INLINE CONST uint8_t add8u_sat(uint8_t x, const uint8_t y);
ALWAYS_INLINE CONST uint8_t sub8u_sat(uint8_t x, const uint8_t y);
#endif

/**
 * \if de
 * Führt eine saturierte Addition (x += y) zwischen zwei vorzeichenlosen 8-Bit
 * Integerzahlen durch.
 *
 * Bei Verwendung des nächst größeren vorzeichenlosen Datentypen (uint16_t)
 * entspräche diese Funktion folgendem C-Code:
 * @bcode{x = (x + y <= 0xFF) ? x + y : 0xFF}
 *
 * @param x der erste Summand
 * @param y der zweite Summand
 *
 * @result die Summe x + y, sofern dabei kein Überlauf aufgetreten ist, sonst 0xFF
 * \endif
 *
 * \if en
 * Computes the sum of two unsigned 8-bit integers (x += y) by saturated
 * addition.
 *
 * The function computes the same result like the following code, if the used
 * data types are unsigned 16-bit integers:
 * @bcode{x = (x + y <= 0xFF) ? x + y : 0xFF}
 *
 * @param x first summand
 * @param y second summand
 *
 * @result the sum x + y, if no overflow occured, else 0xFF
 * \endif
 */
uint8_t add8u_sat(uint8_t x, const uint8_t y) {
    __asm (
            "add  %[x], %[y]    \n\t"
            "brcc nosat_%=      \n\t"
            "ldi  %[x], 0xff    \n\t"
            "nosat_%=:"
            : // Out
            [x] "=d" (x)
            : // In
            "[x]" (x),
            [y] "r" (y));

    return x;
}

/**
 * \if de
 * Führt eine saturierte Subtraktion (x -= y) zwischen zwei vorzeichenlosen 8-Bit
 * Integerzahlen durch.
 *
 * Bei Verwendung des nächst größeren vorzeichenbehafteten Datentypen (int16_t)
 * entspräche diese Funktion folgendem C-Code:
 * @bcode{x = (x - y >= 0) ? x - y : 0}
 *
 * @param x der Minuend
 * @param y der Subtrahend
 *
 * @result die Differenz x - y, sofern dabei kein Unterlauf aufgetreten ist,
 *         sonst 0
 * \endif
 *
 * \if en
 * Computes the difference between two unsigned 8-bit integers (x -= y) by
 * saturated subtraction.
 *
 * The function computes the same result like the following code, if the used
 * data types are unsigned 16-bit integers:
 * @bcode{x = (x - y >= 0) ? x - y : 0}
 *
 * @param x the minuend
 * @param y the subtrahend
 *
 * @result the difference x - y, if no overflow occured, else 0
 * \endif
 */
uint8_t sub8u_sat(uint8_t x, const uint8_t y) {
    __asm (
            "sub %[x], %[y]     \n\t"
            "brcc nosat_%=      \n\t"
            "clr %[x]           \n\t"
            "nosat_%=:"
            : // Out
            [x] "=r" (x)
            : // In
            "[x]" (x),
            [y] "r" (y)
    );

    return x;
}

#endif /* MATH_H_ */
