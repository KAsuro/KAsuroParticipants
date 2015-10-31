/**
 * @file
 *
 * \if de
 * Definiert die Schnittstelle für die Ansteuerung der LEDs.
 *
 * @par Initialisierung:
 *      Vor der Verwendung dieses Moduls muss LEDInit() aufgerufen werden.
 *
 * @par Genutzte Hardwareressourcen:
 *      I/O (Steuersignale):
 *      @li Port B: Pin 0
 *      @li Port C: Pin 0, Pin 1
 *      @li Port D: Pin 2, Pin 6, Pin 7
 * \endif
 *
 * \if en
 * Interface definitions for controlling the LEDs.
 *
 * @par Initialization:
 *      Before using this module, LEDInit() has to be called.
 *
 * @par Hardware ressources in use:
 *      I/O:
 *      @li Port B: Pin 0
 *      @li Port C: Pin 0 and 1
 *      @li Port D: Pin 2, 6 and 7
 * \endif
 *
 * @author Markus Jung
 *
 * @version 22.05.2010 \n
 *  Schnittstellendefinition entsprechend asuro.h angelegt
 * @version 14.06.2010 \n
 *  Doku
 * @version 21.06.2010 \n
 *  Doku von FRONT_LED korrigiert
 * @version 17.12.2011 \n
 *  Odometrie -> Odometry
 * @version 18.09.2013 \n
 *  English translation
 * @version 29.10.2013 \n
 *  Fix interrupt safety issues and document the remaining ones
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

#ifndef LED_H_
#define LED_H_

#include <stdbool.h>
#include <stdint.h>

#ifdef __DOXYGEN__
/**
 * \if de
 * Konstante zum Abschalten der Status-, Back- und Front-LEDs.
 * \endif
 *
 * \if en
 * A constant used to switch off the status, back and front LEDs.
 * \endif
 */
#define OFF
/**
 * \if de
 * Konstante zum Einschalten der Status-, Back- und Front-LEDs.
 * \endif
 *
 * \if en
 * A constant used to switch on the status, back and front LEDs.
 * \endif
 */
#define ON

/**
 * \if de
 * Konstante zum Umschalten der Status-LED auf grün.
 * \endif
 *
 * \if en
 * A constant used to change the color of the status LED to green.
 * \endif
 */
#define GREEN
/**
 * \if de
 * Konstante zum Umschalten der Status-LED auf rot.
 * \endif
 *
 * \if en
 * A constant used to change the color of the status LED to red.
 * \endif
 */
#define RED
/**
 * \if de
 * Konstante zum Umschalten der Status-LED auf gelb.
 * \endif
 *
 * \if en
 * A constant used to change the color of the status LED to yellow.
 * \endif
 */
#define YELLOW

/**
 * \if de
 * Intern verwendete Bitmaske für die grüne Status-LED.
 * \endif
 *
 * \if en
 * Internally used bitmask addressing the green status LED.
 * \endif
 */
#define GREEN_LED
/**
 * \if de
 * Intern verwendete Bitmaske für die rote Status-LED.
 * \endif
 *
 * \if en
 * Internally used bitmask addressing the red status LED.
 * \endif
 */
#define RED_LED
/**
 * \if de
 * Intern verwendete Bitmaske für die Linienfolge-LED.
 * \endif
 *
 * \if en
 * Internally used bitmask addressing the line following LED.
 * \endif
 */
#define FRONT_LED
/**
 * \if de
 * Intern verwendete Bitmaske für die Odometrie-LEDs.
 * \endif
 *
 * \if en
 * Internally used bitmask addressing the odometry LEDs.
 * \endif
 */
#define ODOMETRY_LED
/**
 * \if de
 * Alias für ODOMETRY_LED.
 *
 * @deprecated Wird im Zuge der Schnittstellenvereinheitlichung durch
 *             ODOMETRY_LED ersetzt
 * \endif
 *
 * \if en
 * Alias for ODOMETRY_LED.
 *
 * @deprecated Replaced by ODOMETRY_LED for interface consolidation purposes.
 * \endif
 *
 * @see ODOMETRY_LED
 */
#define ODOMETRIE_LED
/**
 * \if de
 * Bitmaske für die rechte Back-LED.
 * \endif
 *
 * \if en
 * Bitmask addressing the right back LED.
 * \endif
 */
#define BACK_LED_RIGHT
/**
 * \if de
 * Bitmaske für die linke Back-LED.
 * \endif
 *
 * \if en
 * Bitmask addressing the left back LED.
 * \endif
 */
#define BACK_LED_LEFT
/**
 * \if de
 * Bitmaske für beide Back-LEDs.
 * \endif
 *
 * \if en
 * Bitmask addressing both back LEDs.
 * \endif
 */
#define BACK_LED_BOTH

/**
 * \if de
 * Makro zum Einschalten der grünen Status-LED.
 * \endif
 *
 * \if en
 * A macro to turn on the green status LED.
 * \endif
 */
#define GREEN_LED_ON()
/**
 * \if de
 * Makro zum Ausschalten der grünen Status-LED.
 * \endif
 *
 * \if en
 * A macro to turn off the green status LED.
 * \endif
 */
#define GREEN_LED_OFF()
/**
 * \if de
 * Makro zum Einschalten der roten Status-LED.
 * \endif
 *
 * \if en
 * A macro to turn on the red status LED.
 * \endif
 */
#define RED_LED_ON()
/**
 * \if de
 * Makro zum Ausschalten der roten Status-LED.
 * \endif
 *
 * \if en
 * A macro to turn off the red status LED.
 * \endif
 */
#define RED_LED_OFF()
/**
 * \if de
 * Makro zum Einschalten der Front-LED.
 * \endif
 *
 * \if en
 * A macro to turn on the front LED.
 * \endif
 */
#define FRONT_LED_ON()
/**
 * \if de
 * Makro zum Ausschalten der Front-LED.
 * \endif
 *
 * \if en
 * A macro to turn off the front LED.
 * \endif
 */
#define FRONT_LED_OFF()
/**
 * \if de
 * Makro zum Einschalten der Odometrie-LEDs.
 * \endif
 *
 * \if en
 * A macro to turn on the odometry LEDs.
 * \endif
 */
#define ODOMETRY_LED_ON()
/**
 * \if de
 * Makro zum Ausschalten der Odometrie-LEDs.
 * \endif
 *
 * \if en
 * A macro to turn off the odometry LEDs.
 * \endif
 */
#define ODOMETRY_LED_OFF()
/**
 * \if de
 * Alias für ODOMETRY_LED_ON().
 *
 * @deprecated Wird im Zuge der Schnittstellenvereinheitlichung durch
 *             ODOMETRY_LED_ON() ersetzt
 * \endif
 *
 * \if en
 * Alias for ODOMETRY_LED_ON().
 *
 * @deprecated Replaced by ODOMETRY_LED_ON() for interface consolidation purposes.
 * \endif
 *
 * @see ODOMETRY_LED_ON()
 */
#define ODOMETRIE_LED_ON()
/**
 * \if de
 * Alias für ODOMETRY_LED_OFF().
 *
 * @deprecated Wird im Zuge der Schnittstellenvereinheitlichung durch
 *             ODOMETRY_LED_OFF() ersetzt
 * \endif
 *
 * \if en
 * Alias for ODOMETRY_LED_OFF().
 *
 * @deprecated Replaced by ODOMETRY_LED_OFF() for interface consolidation purposes.
 * \endif
 *
 * @see ODOMETRY_LED_OFF()
 */
#define ODOMETRIE_LED_OFF()
#else
#define OFF 0
#define ON  1

#define GREEN   1
#define RED     2
#define YELLOW  3

#define GREEN_LED           (1 << PB0)
#define RED_LED             (1 << PD2)
#define FRONT_LED           (1 << PD6)
#define ODOMETRY_LED        (1 << PD7)
#define ODOMETRIE_LED       ODOMETRY_LED
#define BACK_LED_RIGHT      (1 << PC0)
#define BACK_LED_LEFT       (1 << PC1)
#define BACK_LED_BOTH       (BACK_LED_LEFT | BACK_LED_RIGHT)

#define GREEN_LED_ON()      PORTB |=  GREEN_LED
#define GREEN_LED_OFF()     PORTB &= ~GREEN_LED
#define RED_LED_ON()        PORTD |=  RED_LED
#define RED_LED_OFF()       PORTD &= ~RED_LED
#define FRONT_LED_ON()      PORTD |= FRONT_LED
#define FRONT_LED_OFF()     PORTD &= ~FRONT_LED
#define ODOMETRY_LED_ON()   PORTD |= ODOMETRIE_LED
#define ODOMETRY_LED_OFF()  PORTD &= ~ODOMETRIE_LED
#define ODOMETRIE_LED_ON()  ODOMETRY_LED_ON()
#define ODOMETRIE_LED_OFF() ODOMETRY_LED_OFF()
#endif

/**
 * \if de
 * Konfiguriert die Back- und Odometrie-LEDs so, dass die Back-LEDs angesteuert
 * werden können. Daraus folgt, dass keine Odometrie-Messung durchgeführt
 * werden kann, die Odometrie-LEDs werden abgeschaltet.
 *
 * @par Wichtiger Hinweis:
 * Dieses Makro ist nur eingeschränkt interruptfest. Wird es durch eine ISR
 * unterbrochen, die LED_CONFIGURE_ODOMETRY() ausführt, ist der resultierende
 * Zustand der LEDs undefiniert.
 * \endif
 *
 * \if en
 * Configures the back and odometry LEDs into back LED mode. The odometry
 * LEDs will be turned off, no more odometry measurments can be done.
 *
 * @par Important:
 * This macro is not completely interrupt-safe. The LEDs final state is
 * undefined if it is interrupted by an ISR which executes the macro
 * LED_CONFIGURE_ODOMETRY().
 * \endif
 */
#ifdef __DOXYGEN__
#define LED_CONFIGURE_BACK()
#else
#define LED_CONFIGURE_BACK() \
    do { \
        ODOMETRY_LED_OFF(); \
        DDRC |= BACK_LED_LEFT; \
        DDRC |= BACK_LED_RIGHT; \
    } while (false)
#endif

/**
 * \if de
 * Konfiguriert die Back- und Odometrie-LEDs so, dass Odometrie-Messungen
 * durchgeführt werden können, die Odometrie-LEDs werden dabei aktiviert.
 * Die Schaltung der Back-LED Ausgänge wird dabei nicht beeinflusst.
 *
 * @par Wichtiger Hinweis:
 * Dieses Makro ist nur eingeschränkt interruptfest. Wird es durch eine ISR
 * unterbrochen, die LED_CONFIGURE_BACK() ausführt, ist der resultierende
 * Zustand der LEDs undefiniert.
 * \endif
 *
 * \if en
 * Configures the back and odometry LEDs into odometry mode. The odometry
 * LEDs will be turned on, the back LEDs will extinguish. The state of the
 * back LED control lines will not change, the back LEDs will light up again
 * if you call LED_CONFIGURE_BACK().
 *
 * @par Important:
 * This macro is not completely interrupt-safe. The LEDs final state is
 * undefined if it is interrupted by an ISR which executes the macro
 * LED_CONFIGURE_BACK().
 * \endif
 */
#ifdef __DOXYGEN__
#define LED_CONFIGURE_ODOMETRY()
#else
#define LED_CONFIGURE_ODOMETRY() \
    do { \
        ODOMETRY_LED_ON(); \
        DDRC &= ~BACK_LED_LEFT; \
        DDRC &= ~BACK_LED_RIGHT; \
    } while (false)
#endif

/**
 * \if de
 * Alias für LED_CONFIGURE_ODOMETRY().
 *
 * @deprecated Wird im Zuge der Schnittstellenvereinheitlichung durch
 *             LED_CONFIGURE_ODOMETRY() ersetzt
 * \endif
 *
 * \if en
 * Aliases to LED_CONFIGURE_ODOMETRY().
 *
 * @deprecated Replaced by LED_CONFIGURE_ODOMETRY() for interface consolidation
 *             purposes.
 * \endif
 *
 * @see LED_CONFIGURE_ODOMETRY()
 *
 */
#ifdef __DOXYGEN__
#define LED_CONFIGURE_ODOMETRIE()
#else
#define LED_CONFIGURE_ODOMETRIE() LED_CONFIGURE_ODOMETRY()
#endif

/**
 * \if de
 * Initialisiert die LED-Ansteuerung. Die Status-LED wird auf grün geschaltet,
 * alle anderen LEDs bleiben dunkel. Die Umschaltung zwischen Back- und
 * Odometrie-LEDs wird auf Back-LED konfiguriert.
 * \endif
 *
 * \if en
 * Initializes the LED control lines. The status LED is configured to glow
 * green, all other LEDs will be turned off. The back/odometry LEDs are
 * configured to back LED mode.
 * \endif
 */
extern void LEDInit(void);
/**
 * \if de
 * Setzt die Status-LED auf die übergebene Farbe oder schaltet sie aus.
 *
 * @par Wichtiger Hinweis:
 * Diese Funktion ist nicht interruptfest. Wird sie gleichzeitig aus dem
 * Hauptprogramm und einer ISR aufgerufen, ist der resultierende Zustand der LED
 * undefiniert.
 *
 * @param color die zu setzende Farbe, GREEN, RED, YELLOW oder OFF
 * \endif
 *
 * \if en
 * Configures the status LED according to the passed color/state.
 *
 * @par Important:
 * This function is not interrupt-safe. The LED status is undefined if the
 * function is called from the main program and an ISR at the same time.
 *
 * @param color the color/state to configure, either GREEN, RED, YELLOW or OFF
 * \endif
 */
extern void StatusLED(const uint8_t color);
/**
 * \if de
 * Schaltet den Status der Front/Linienfolge-LED um.
 *
 * @param status der zu setzende Status, ON oder OFF
 * \endif
 *
 * \if en
 * Configures the front/line following LED according to the passed state.
 *
 * @param status the state to configure, either ON or OFF
 * \endif
 */
extern void FrontLED(const uint8_t status);
/**
 * \if de
 * Schaltet den Status der Back-LEDs um, sollen eine oder beide Back-LEDs
 * aktiviert werden, wird automatisch von Odometrie auf die Back-LEDs umgestellt.
 *
 * Diese Funktion existiert nur aus Kompatibilitätsgründen, intern wird
 * BackLEDFast() aufgerufen.
 *
 * Zur Auswahl stehen für jede LED die Zustände ON und OFF
 *
 * @deprecated Es wird empfohlen, die die optimierte Version BackLEDFast() zu
 *             verwenden.
 *
 * @param left der zu setzende Status für die linke Back-LED
 * @param right der zu setzende Status für die rechte Back-LED
 * \endif
 *
 * \if en
 * Configures the back LEDs according to the passed states. The back/odometry
 * LEDs are automatically configured to back LED mode if at least one back LED
 * shall be enabled.
 *
 * This function does only exist for compatibility reasons, BackLEDFast() is
 * used internally.
 *
 * Each LED might be configured as ON or OFF.
 *
 * @deprecated Using the optimized function BackLEDFast() is recommended
 *
 * @param left the state to apply to the left back LED
 * @param right the state to apply to the right back LED
 * \endif
 */
extern void BackLED(const uint8_t left, const uint8_t right);
/**
 * \if de
 * Schaltet den Status der Back-LEDs um, sollen eine oder beide Back-LEDs
 * aktiviert werden, wird automatisch von Odometrie auf die Back-LEDs umgestellt.
 *
 * @param leds der zu setzende Status für beide LEDs, mögliche Werte:
 *             OFF, BACK_LED_LEFT, BACK_LED_RIGHT oder
 *             (BACK_LED_LEFT | BACK_LED_RIGHT).
 *             Letzteres entspricht BACK_LED_BOTH
 * \endif
 *
 * \if en
 * Configures the back LEDs according to the passed state. The back/odometry
 * LEDs are automatically configured to back LED mode if at least one back LED
 * shall be enabled.
 *
 * @param leds the state to apply to both back LEDs, which may be: OFF,
 *             BACK_LED_LEFT, BACK_LED_RIGHT or (BACK_LED_LEFT | BACK_LED_RIGHT),
 *             which is the same as BACK_LED_BOTH
 * \endif
 */
extern void BackLEDFast(const uint8_t leds);

#endif /* LED_H_ */
