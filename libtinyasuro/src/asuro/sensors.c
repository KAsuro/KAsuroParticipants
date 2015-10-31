/**
 * @file
 *
 * \if de
 * Implementiert die in sensors.h spezifizierte Funktionalität.
 * \endif
 *
 * \if en
 * Implements the functionality specified in sensors.h
 * \endif
 *
 * @author Markus Jung
 *
 * @version 18.05.2010 \n
 *  Funktionalität aus asuro.c reimplementiert
 * @version 28.05.2010 \n
 *  Interruptbetrieb implementiert
 * @version 31.05.2010 \n
 *  Auf neuen ADC-Betriebsmodus umgestellt
 * @version 25.06.2010 \n
 *  Fehler in SensorConfigAutomode bei am_odo | am_line behoben
 * @version 16.08.2010 \n
 *  + Hook für ADCISR
 * @version 17.12.2011 \n
 *  Odometrie -> Odometry
 * @version 15.09.2013 \n
 *  Add BatteryVoltage() function for direct battery voltage measurment
 * @version 16.09.2013 \n
 *  English translation
 * @version 10.02.2014 \n
 *  Add deprecation warning to OdometrieData
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

#include <avr/interrupt.h>

#include <asuro/adc.h>
#include <asuro/config.h>
#include <asuro/sensors.h>
#include <util/misc.h>
#include <util/math.h>

#define AUTOMODE_CH_ODO_RIGHT   ADC_CH_0 // 0
#define AUTOMODE_CH_ODO_LEFT    ADC_CH_1 // 1
#define AUTOMODE_CH_IR_RIGHT    ADC_CH_2 // 2
#define AUTOMODE_CH_IR_LEFT     ADC_CH_3 // 3
#define AUTOMODE_ADC_CH(ch) ADC_LEFT_ADJUST((ch) | ADC_REF_VCC_DECOUPLED)

volatile AutoMeasurmentMode_t amMode;

volatile SensorValues_t odoData;
volatile bool odoDataUpdated;

volatile SensorValues_t lineData;
volatile bool lineDataUpdated;

#if ADC_ISR_HOOK_ENABLE
    ISRHookFunc_t *ADCISRHook;
#endif

void SensorInit(void) {
    amMode = am_off;
    odoDataUpdated = false;
    lineDataUpdated = false;

    // Configure signal inputs as input, low
    DDRC &= ~((1 << DDC0) | (1 << DDC1) | (1 << DDC2) | (1 << DDC3) | (1 << DDC5));
    PORTC &= ~((1 << PC0) | (1 << PC1) | (1 << PC2) | (1 << PC3) | (1 << PC5));
}

void SensorConfigAutomode(const AutoMeasurmentMode_t mode) {
    if (mode == am_off) {
        ADCInterruptDisable();
    } else {
        ADCInterruptEnable(AUTOMODE_ADC_CH(mode / 2));
    }

    amMode = mode;
}

void LineData(uint16_t * const data) {
    ADC_BLOCK {
        ADCSelectChannel(ADC_IR_LEFT);
        ADCMeasure();
        data[LEFT] = ADC;

        ADCSelectChannel(ADC_IR_RIGHT);
        ADCMeasure();
        data[RIGHT] = ADC;
    }
}

void OdometryData(uint16_t * const data) {
    LED_CONFIGURE_ODOMETRY();
    ODOMETRY_LED_ON();

    ADC_BLOCK {
        ADCSelectChannel(ADC_ODO_LEFT);
        ADCMeasure();
        data[LEFT] = ADC;

        ADCSelectChannel(ADC_ODO_RIGHT);
        ADCMeasure();
        data[RIGHT] = ADC;
    }
}

DEPRECATED("Replaced by OdometryData.")
void OdometrieData(uint16_t * const data) ALIAS("OdometryData");

uint8_t BatteryVoltage() {
    uint16_t adcVal;

    ADC_BLOCK {
        ADCSelectChannel(ADC_BAT_INTERNAL);
        ADCMeasure();

        adcVal = ADC;
    }

    /*
     * The ASURO is has a voltage divider (R12 and R13) originally intented to
     * be used for battery voltage measurments:
     * Vcc = ADC * VBG * (R12 + R13) / (R13 + 1024)
     * with R12 = 12kOhm, R13 = 10kOhm
     *
     * The ADC offers a second way of supply voltage measurment using the
     * supply voltage as reference and the internal bandgap voltage reference as
     * measurment input for the ADC, this option is expected to be more precise.
     * Vcc = 1024 * VBG / ADC
     *
     * The first approach requires (in contrary to the bandgap trick) no
     * division (bitshift!) but can not be ported to the ATmega[4|8|16|32]8
     * series because their internal reference voltage is below the voltage
     * divided by R12 and R13.
     */

#if !INTERNAL_BATTERY_MEASURMENT
    return adcVal * (uint8_t)(2.56f * 22 * BATTERY_CALIBRATION_FACTOR) / 1024;
#else
#if __AVR_ATmega8__
    // Internal voltage reference of ATmega8: ~1.3V
    return (uint16_t)(1024 * 1.29f * BATTERY_CALIBRATION_FACTOR) / adcVal;
#else
    // Internal voltage reference of ATmega[4|8|16|32]8: ~1.1V
    return (uint16_t)(1024 * 1.092f * BATTERY_CALIBRATION_FACTOR) / adcVal;
#endif
#endif
}

ISR(ADC_vect)
{
    static uint8_t right;
    uint8_t channel = ADC_GET_CHANNEL();

    sei(); // re-enable interrupts

    switch (channel) {
    case AUTOMODE_ADC_CH(AUTOMODE_CH_ODO_LEFT):
        if (amMode & am_line) {
            channel++; // IR_RIGHT is the successor of ODO_LEFT
        } else {
            channel = AUTOMODE_ADC_CH(AUTOMODE_CH_ODO_RIGHT);
        }

        cli();
        odoData.left = ADCH;
        odoData.right = right;
        odoDataUpdated = true;
        sei();

        break;
    case AUTOMODE_ADC_CH(AUTOMODE_CH_IR_LEFT):
        if (amMode & am_odo) {
            channel = AUTOMODE_ADC_CH(AUTOMODE_CH_ODO_RIGHT);
        } else {
            channel = AUTOMODE_ADC_CH(AUTOMODE_CH_IR_RIGHT);
        }

        cli();
        lineData.left = ADCH;
        lineData.right = right;
        lineDataUpdated = true;
        sei();

        break;
    default:
        right = ADCH;
        channel++;
        break;
    }

    ADCInterruptEnable(channel);

#if ADC_ISR_HOOK_ENABLE
    if (ADCISRHook != NULL) {
        ADCISRHook();
    }
#endif
}
