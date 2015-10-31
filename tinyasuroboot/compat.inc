/**
 * @file
 * 
 * \if de
 * @par Kompatibilitätsschicht zum Zugriff auf alle I/O-Register ohne 
 *      berücksichtigung der Grenze zum erweiterten I/O-Adressraum neuerer
 *      AVR-Mikrocontroller. Adaptiert von der Atmel Appnote AVR001.
 * \endif
 *
 * \if en
 * @par A compatibility layer to access all I/O registers without explicit 
 *      handling of address-space limitations (extended I/O space) which are 
 *      present at newer AVR microcontrollers. 
 *      Adapted from the Atmel Appnote AVR001.
 * \endif
 *
 * @author Markus Jung
 *
 * @version 02.09.2013 \n
 *  Adaption der Atmel Appnote AVR001 auf avr-gcc/avr-as
 * @version 10.02.2014 \n
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

;*********************************************************
;*	BIT access anywhere in IO space
;*	SETB - SET Bit
;*	CLRB - CLeaR Bit
;*********************************************************

.macro SETB addr, bit, tmpReg
	.if \bit > 7
		.message "Only values 0-7 allowed for Bit parameter"
	.endif
	
	.if \addr < 0x20
        sbi  \addr, \bit
    .elseif \addr < 0x40
        in   \tmpReg, \addr
		sbr  \tmpReg, (1 << \bit)
		out  \addr, \tmpReg
    .elseif ((\addr >= 0x60) && (\addr < RAMSTART))
        lds  \tmpReg, \addr
		sbr  \tmpReg, (1 << \bit)
		sts  \addr, \tmpReg
    .else
       .error "SETB: Invalid I/O register address"
    .endif
.endm

.MACRO CLRB addr, bit, tmpReg
	.if \bit > 7
        .message "Only values 0-7 allowed for Bit parameter"
    .endif
    
    .if \addr < 0x20
        cbi  \addr, \bit
    .elseif \addr < 0x40
        in   \tmpReg, \addr
        cbr  \tmpReg, (1 << \bit)
        out  \addr, \tmpReg
    .elseif ((\addr >= 0x60) && (\addr < RAMSTART))
        lds  \tmpReg, \addr
        cbr  \tmpReg, (1 << \bit)
        sts  \addr, \tmpReg
    .else
       .error "SETB: Invalid I/O register address"
    .endif
.endm

;*********************************************************
;*	Bit test anywhere in IO space
;*  SKBS : SKip if Bit Set
;*  SKBC : SKip if Bit Cleared
;*********************************************************
.macro SKBS addr, bit, tmpReg
    .if \bit > 7
        .message "Only values 0-7 allowed for Bit parameter"
    .endif
    
    .if \addr < 0x20
        sbis \addr, \bit
    .elseif \addr < 0x40
        in   \tmpReg, \addr
        sbrs \tmpReg, \bit
    .elseif ((\addr >= 0x60) && (\addr < RAMSTART))
        lds  \tmpReg, \addr
        sbrs \tmpReg, \bit
    .else
       .error "SKBS: Invalid I/O register address"
    .endif
.endm

.macro SKBC addr, bit, tmpReg
	.if \bit > 7
		.message "Only values 0-7 allowed for Bit parameter"
	.endif
	
    .if \addr < 0x20
        sbic \addr, \bit
    .elseif \addr < 0x40
        in   \tmpReg, \addr
        sbrc \tmpReg, \bit
    .elseif ((\addr >= 0x60) && (\addr < RAMSTART))
        lds  \tmpReg, \addr
        sbrc \tmpReg, \bit
    .else
       .error "SKBC: Invalid I/O register address"
    .endif
.endm

;*********************************************************
;*	Byte access anywhere in IO space
;*  LDIO: Load register from IO space
;*  STIO: Store register to IO space
;*********************************************************

; usage LDIO reg, addr
.macro LDIO reg, addr
    .if \addr < 0x40
        in \reg, \addr
    .elseif ((\addr >= 0x60) && (\addr < RAMSTART))
        lds \reg, \addr
    .else
       .error "LDIO: Invalid I/O register address"
    .endif
.endm

; usage: STIO addr, reg
.macro STIO addr, reg
    .if \addr < 0x40
        out \addr, \reg
    .elseif ((\addr >= 0x60) && (\addr < RAMSTART))
        sts \addr, \reg
    .else
       .error "STIO: Invalid I/O register address"
    .endif
.endm 
