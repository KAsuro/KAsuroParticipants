/**
 * @file
 *
 * \if de
 * @par Tiny ASURO Bootloader
 *      Eine Bootloader-Implementierung für den ASURO, kompatibel zum
 *      mitgelieferten Bootloader und dem dazugehörigen Programm "ASURO Flash".
 * @par
 *      Während der Original-Bootloader 1024 Byte (512 Words) belegt, benötigt
 *      diese Implementierung nur 512 Byte (256 Words). Zudem kann die
 *      Spannungsuntergrenze bei Bedarf angepasst werden.
 *
 * @par Unterstützte AVRs:
 *      Die Implementierung unterstützt (momentan) nur den ATmega8.
 *
 * @par Verwendung:
 *      Bevor der Bootloader verwendet werden kann, muss dieser einmalig in den
 *      Flash des verwendeten AVR geladen werden (via SPI oder HVPP). Zudem
 *      sind die Fusebits wie folgend zu programmieren: \n
 *      High-Byte = 0xD4 (0b11010100) - SPIEN, EESAVE, BOOTSZ0, BOOTRST \n
 *      Low-Byte  = 0xBF (0b10111111) - BODEN
 *
 * @par Assemblierung:
 *      Der Tiny ASURO Bootloader kann mit avr-gcc/avr-as übersetzt werden,
 *      das beigefügte Makefile erleichtert diese Aufgabe. Im Gegensatz zu
 *      früheren Versionen des Bootloaders wird AVRASM2 (Teil des AVR Studios)
 *      nicht mehr benötigt.
 *
 * @par Anmerkung:
 *      "ASURO Flash" prüft die Größe der zu übertragenden Programme bevor diese
 *      an den "ASURO" gesendet werden. Da es vom Original-Bootloader ausgeht,
 *      sind die unbelegten 512 Byte im Bootloader-Bereich nicht nutzbar. \n
 *      Allerdings ist der Quelltext der Kommandozeilenversion für Linux frei
 *      verfügbar - Was die Anpassung des programmierbaren Bereichs ermöglicht.
 *
 * @par Protokoll:
@verbatim
       ASURO              Computer
                    <-     "Flash"
      "ASURO"       ->
                    <-    Pagedaten
  "OK"/"CK"/"ER"    ->
                     :
                     :
                    <-    Pagedaten (alles 0xFF)
  "OK"/"CK"/"ER"    ->
                   Ende @endverbatim
 *      Pagedaten: 1 Byte Pagenummer, 64 Byte Page (Inhalt), 2 Byte CRC16
 *                 (über Byte 0 bis 64) \n
 *      "OK" = Page erfolgreich geschrieben \n
 *      "CK" = Page fehlerhaft empfangen \n
 *      "ER" Page fehlerhaft geschrieben
 *
 * @par
 *      Bei leeren Batterien/Akkus sendet der ASURO ununterbrochen "VL". Dabei
 *      blinkt/flackert die Status-LED orange.
 * \endif
 *
 * \if en
 * @par Tiny ASURO Bootloader
 *      The tiny ASURO bootloader is a bootloader implementation for the ASURO.
 *      It is compatible to the original bootloader and the upload program
 *      "ASURO Flash".
 * @par
 *      While original bootloader consumes 1024 bytes (512 words), the tiny
 *      ASURO bootloader requires only 512 bytes (256 words). It also allows to
 *      configure the low battery threshold value.
 *
 * @par Supported AVRs:
 *      The (at least for now) only supported MCU is the ATmega8.
 *
 * @par Usage:
 *      Before the bootloader can be used, it has to be programmed once into the
 *      flash memory of the AVR (e.g. via SPI or HVPP). Additionally, the fuse
 *      bits are required to be programmed as below: \n
 *      High byte = 0xD4 (0b11010100) - SPIEN, EESAVE, BOOTSZ0, BOOTRST \n
 *      Low byte  = 0xBF (0b10111111) - BODEN
 *
 * @par Assembling:
 *      The tiny ASURO bootloader can be compiled using avr-gcc/avr-as. An
 *      enclosed Makefile can be used to simplify this process. AVRASM2 (which
 *      is part of the AVR studio) is not required any more.
 *
 * @par Remark:
 *      "ASURO Flash" compares the program size to the size of the available
 *      flash memory, which is 7KB (7168 bytes). Because the flash tool expects
 *      the original bootloader to be present, it refuses to write to the unused
 *      512 bytes which are not required by this bootloader implementation.
 *      But: The command line flash tool for linux is open-source. Adjustments
 *      of the upload size limit are therefore possible.
 *
 * @par Protocol:
@verbatim
       ASURO              Computer
                    <-     "Flash"
      "ASURO"       ->
                    <-    Page data
  "OK"/"CK"/"ER"    ->
                     :
                     :
                    <-    Page data (only 0xFF)
  "OK"/"CK"/"ER"    ->
                    End @endverbatim
 *      Page data: 1 byte page number, 64 bytes page content, 2 byte CRC16
 *                 (computed from byte 0 to 64) \n
 *      "OK" = Page written successfully \n
 *      "CK" = Page transmission/receive error \n
 *      "ER" Page write failed
 *
 * @par
 *      The ASURO sends "VL" continously if the batteries are empty. The status
 *      led blinks/flickers orange in this case.
 * \endif
 *
 * @author Markus Jung
 *
 * @version 05.09.2010 \n
 *  Implementierungsbeginn
 * @version 13.09.2010 \n
 *  Erste vollständig funktionierende Implementierung
 * @version 14.09.2010 \n
 *  Fusebit-Kommentar entfernt, da die Info bereits in der Doku steht
 * @version 02.09.2013 \n
 *  Übersetzung des Bootloaders vom Atmel AVRASM2 zu avr-gcc/avr-as Assembler \n
 *  Änderung des ADC-Takts von F_CPU / 32 auf F_CPU / 64
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

#ifndef __DOXYGEN__

#define __SFR_OFFSET 0 // Allows to eliminate the requirement for _SFR_IO_ADDR()

#ifndef F_CPU
    #define F_CPU 8000000
#endif

#ifndef BAUD
    #define BAUD 2400
#endif

#ifndef LOW_BAT_THRESHOLD
    #define LOW_BAT_THRESHOLD 165 // ca. 3.8V for ATmega8
#endif

.nolist
#include <avr/io.h>
#include <util/setbaud.h>
.list

// include compatibility/portability layer and use ldio/stio instead of in/out
.include "compat.inc"

#define WREG(high,low) low // Helper macro for word register access (16-bit operands)

// Register usage:
// r0:  zero register
// r1:  temporary register used during CRC calculation
// r2:  temporary register used during CRC calculation
// r3:  temporary register used during CRC calculation
// r8:  temporary register for compat.inc
// r16: general purpose temporary register
// r17: general purpose temporary register
// r18: general purpose temporary register
// r24: temporary register used for 16/24-bit computations
// r25: temporary register used for 16/24-bit computations
// r26: temporary register used for 24-bit computations
// X:   temporary register containing the SRAM buffer pointer during lpm/spm
// Y:   SRAM pointer to the start of the SRAM during initialization, and to RX_Page[0] after the flash mode has been entered
// Z:   temporary register containing some pointer to SRAM/flash
#define rZero   r0
#define rTmp    r8

// Define some dummy section to prevent ld from relocating reset to the start
// of the boot section (main)
.section .reset
; Reset vector required to start into the main application code
reset:


// Program code goes to .text (flash)
.text

    .global main
main:
; Initializations

    ; initialize stack pointer
    cli
    ldi     r16,            hi8(RAMEND)
    stio    SPH,            r16
    ldi     r16,            lo8(RAMEND)
    stio    SPL,            r16

    ; read constant data/strings
    ldi     ZH,             hi8(P_Data_Start)
    ldi     ZL,             lo8(P_Data_Start)
    ldi     XH,             hi8(R_Data_Start)
    ldi     XL,             lo8(R_Data_Start)
    movw    WREG(YH,YL),    WREG(XH,XL)             ; store base address for later re-use
    ldi     r16,            P_Data_Size
load_constant_data:
    lpm     rZero,          Z+
    st      X+,             rZero                   ; last string byte = 0 -> rZero is really zero from now
    subi    r16,            1
    brne    load_constant_data

    ; initialize IR communications timer (36kHz carrier clock)
    stio    ASSR,           rZero                   ; 0 ~ no asynchronous operation
    ldi     r16,            ((1 << WGM21) | (1 << COM20) | (1 << CS20))
    stio    TCCR2,          r16                     ; CTC, toggle on CompMatch, CLK = F_CPU
    ldi     r16,            (F_CPU / 72000)         ; 72kHz toggle frequency
    stio    OCR2,           r16
    setb    DDRB,           PB3,    rTmp

    ; initialize USART
    stio    UCSRA,          rZero
    ldi     r16,            (1 << RXEN)
    stio    UCSRB,          r16
    ldi     r16,            ((1 << URSEL) | (1 << UCSZ1) | (1 << UCSZ0)) ; 8N1
    stio    UCSRC,          r16
    ldi     r16,            UBRRH_VALUE
    stio    UBRRH,          r16
    ldi     r16,            UBRRL_VALUE
    stio    UBRRL,          r16

    ; initialize status LED (yellow)
    setb    DDRB,           DDB0,   rTmp            ; configure green pin as output
    setb    DDRD,           DDD2,   rTmp            ; configure red pin as output
    setb    PORTB,          PB0,    rTmp            ; green on
    setb    PORTD,          PD2,    rTmp            ; red on


; Initializion is finished now

    ; delay loop before battery voltage check - requires about 10ms
    ldi     r25,            hi8(F_CPU / 400)        ; / (100Hz * 4 clock cycles/iteration)
    ldi     r24,            lo8(F_CPU / 400)
battery_check_delay_loop:
    sbiw    WREG(r25,r24),  1                       ; 2 cycles
    brne    battery_check_delay_loop                ; 2 cycles
    ; check battery voltage
    ldi     r16,            ((1 << MUX2) | (1 << MUX0) | (1 << ADLAR) \
                                | (1 << REFS1) | (1 << REFS0))
    stio    ADMUX,          r16                     ; ADC channel 5, left adjusted result, ARef=VBG(decoupled)
    ldi     r16,            ((1 << ADEN) | (1 << ADSC) | (1 << ADPS2) | (1 << ADPS1))
    stio    ADCSRA,         r16                     ; turn on ADC and start conversion, CLK = F_CPU/64
battery_check_adc_wait:
    ldio    r17,            ADCSRA
    cp      r17,            r16
    breq    battery_check_adc_wait                  ; ADSC-flag is still not set yet
    ; AD-conversion is finished
    ldio    r16,            ADCH
    stio    ADCSRA,         rZero                   ; turn ADC off
    cpi     r16,            LOW_BAT_THRESHOLD
    brsh    init_wait                               ; >= LOW_BAT_THRESHOLD -> proceed

    ; too low battery voltage, flash status LED red/yellow and transmit "LV"
low_voltage_loop:
    ldi     r16,            (1 << PB0)              ; Mask used to toggle the green status LED
    ldi     ZH,             hi8(Str_VL)             ; Write adress of string "VL" to Z
    ldi     ZL,             lo8(Str_VL)
    ; prepare 24-bit counter (10 Hz)
    ldi     r26,            hlo8(F_CPU / 50)        ; / (10Hz * 5 clock cycles/iteration)
    ldi     r25,            hi8(F_CPU / 50)
    ldi     r24,            lo8(F_CPU / 50)
low_voltage_delay_loop:
    ; abuse rZero during loop to toggle the status LED
    ; 24-bit subtraction
    sbiw    WREG(r25,r24),  1                       ; 2 cycles
    sbci    r26,            0                       ; 1 cycle
    brne    low_voltage_delay_loop                  ; 2 cycles
    ; called at approx. 10Hz
    eor     rZero,          r16                     ; toggle green status LED
    stio    PORTB,          rZero                   ; and write back
    rcall   ser_print                               ; send "VL"
    rjmp    low_voltage_loop


; Wait for the "magical" character sequence "Flash"

    ; initialization
init_wait:
    ; 9 clock cycles are required for one delay loop iteration, the time required for receiving is neglected
    ldi     r26,            hlo8(F_CPU * 3 / 9)     ; * 3s / 9 clock cycles/iteration
    ldi     r25,            hi8(F_CPU * 3 / 9)
    ldi     r24,            lo8(F_CPU * 3 / 9)

    ; listen 3 seconds for "Flash"
loop_str_flash_init:
    movw    WREG(ZH,ZL),    WREG(YH,YL)             ; load base address for input buffer
loop_str_flash_next_char:
    ld      r17,            Z+                      ; load next expected character
    cpi     r17,            0
    breq    enter_load_mode                         ; found end of "Flash" sequence, enter download/flash mode
loop_str_flash_char_wait:
    ; test for timeout
    sbiw    WREG(r25,r24),  1                       ; 2 cycles
    sbci    r26,            0                       ; 1 cycle
    brne    loop_str_flash_no_timeout               ; 2 cycles
    ; timeout -> turn status LED off and start main program
    clrb    PORTB,          PB0,    rTmp            ; green LED off
    clrb    PORTD,          PD2,    rTmp            ; red LED off
    rjmp    reset                                   ; jump to flash address 0x0
    ; no timeout, test if a new character has been received
loop_str_flash_no_timeout:
    ldio    r16,            UCSRA                                                   ; 1 cycle
    andi    r16,            (1 << RXC)                                              ; 1 cycle
    breq    loop_str_flash_char_wait                ; no new data, continue waiting ; 2 cycles
    ldio    r16,            UDR                     ; load received character
    cp      r16,            r17                     ; received = expected?
    brne    loop_str_flash_init                     ; no - continue waiting
    rjmp    loop_str_flash_next_char                ; yes - load next expected character


; Wait for page download

    ; status updates and initializations required for the programming mode
    ; Y contains always a pointer to RX_Page[0] from now on
enter_load_mode:
    rcall   ser_print                               ; Z points to the 'A' of "ASURO"
    ldi     YH,             hi8(RX_Page)            ; setup Y pointer to RX_Page[0]
    ldi     YL,             lo8(RX_Page)

    ; initialize delay loop and receive procedure
page_receive_init:
    ; 67 bytes at 2400 baud -> ~200ms transmission time per page
    ; 10 clock cycles required for delay loop, desired frequency 3Hz -> 333ms timeout
    ldi     r23,            hlo8(F_CPU / 30)
    ldi     r25,            hi8(F_CPU / 30)
    ldi     r24,            lo8(F_CPU / 30)
    ldi     r16,            RX_Page_Size            ; RX byte counter
    movw    WREG(ZH,ZL),    WREG(YH,YL)             ; RX page counter
    clrb    PORTB,          PB0,    rTmp            ; turn green LED off, red LED remains on
    ; combined receive/delay loop
page_receive_wait:
    ; 1st test: timeout handling required?
    cpi     r16,            RX_Page_Size            ; compare to initial value                  ; 1 cycle
    breq    page_receive_wait_page_start            ; counter = initial value -> page empty     ; 1 cycle
    sbiw    WREG(r25,r24),  1                                                                   ; 2 cycles
    sbci    r23,            0                                                                   ; 1 cycle
    breq    page_receive_init                       ; timeout, discard received page data       ; 1 cycle
    ; 2nd test: received new page data?
page_receive_wait_page_start:
    ldio    r17,            UCSRA                                                               ; 1 cycle
    sbrs    r17,            RXC                                                                 ; 1 cycle
    rjmp    page_receive_wait                       ; no new data received -> next iteration    ; 2 cycles
    setb    PORTB,          PB0,    rTmp            ; turn green LED on
    ldio    r17,            UDR
    st      Z+,             r17
    subi    r16,            1                       ; decrement RX byte counter
    brne    page_receive_wait                       ; counter > 0 -> more page bytes are pending

    ; Evaluate/validate the received data
    ; FIN page?
    ; Z points currently to the first byte behind the RX_Page (page data)
    ldi     r16,            RX_Page_Size
    ser     r17                                     ; set all bits to compute the binary and with r17 later
page_check_for_fin_loop:
    ld      r18,            -Z                      ; load next received byte
    and     r17,            r18                     ; compute binary and between r17 and the received byte
    subi    r16,            1                       ; decrement byte counter
    brne    page_check_for_fin_loop                 ; have all bytes been processed?
    com     r17                                     ; invert binary, if all bytes had the value 0xFF, r17 will contain now 0x00
    brne    page_check_crc                          ; no FIN page, validate CRC

    ; Received FIN page, send final "OK", configure the status LED to green and enter power-down sleep state
    setb    PORTB,          PB0,    rTmp            ; turn green LED on
    clrb    PORTD,          PD2,    rTmp            ; turn red LED off
    ldi     ZH,             hi8(Str_OK)
    ldi     ZL,             lo8(Str_OK)
    rcall   ser_print                               ; "OK"
    ldi     r16,            ((1 << SE) | (1 << SM1))
    stio    MCUCR,          r16
    sleep                                           ; the end, all interrupts are off, the AVR will not wake up again

    ; Test the CRC checksum of the received page/frame
page_check_crc:
#define     hi              r3
#define     lo              r2
#define     a               r1
#define     counter         r16
#define     par             r17

    clr     hi
    clr     lo
    ldi     counter,        RX_Page_Size - 2        ; exclude the CRC itself
    ; Z points to the first byte, start loop
page_check_crc_loop:
    ; load next byte
    ld      a,              Z+

    ; compute CRC16
    eor     a,              lo                      ; a = a* ^ lo*
    mov     lo,             hi                      ; lo = hi*
    ; compute parity in lo and use hi as temporary storage
    mov     par,            a
    swap    par
    eor     par,            a                       ; 73 62 51 40 73 62 51 40
    mov     hi,             par
    lsr     par
    lsr     par
    eor     par,            hi                      ; 7351 6240 7351 6240 7351 6240 51 40
    mov     hi,             par
    lsr     par
    eor     par,            hi                      ; 76543210 76543210 ...
    andi    par,            0x0F                    ; isolate the parity bits 3210
    ; interleave parity and a
    mov     hi,             a
    lsr     par                                     ; one parity bit -> carry
    ror     hi                                      ; parity to hi.7, (a* ^ lo*).0 -> carry
    ror     par                                     ; [7](a* ^ lo*).0 .. [10]parity, carry = parity
    eor     hi,             a                       ; interleave (a* ^ lo*) with itself
    ror     hi                                      ; [76]parity [6-0](a* ^ lo*).7-1 [5-0](a* ^ lo*).7-2
    ror     par                                     ; [7](a* ^ lo*).0 [76](a* ^ lo*).10 .. [0]parity
    ; and combine with lo
    eor     lo,             par                     ; [7](a* ^ lo*).0 [76](a* ^ lo*).10 [0]parity hi*.7-0

    ; CRC calculation completed?
    subi    counter,        1
    brne    page_check_crc_loop

#undef      a
#undef      par
#undef      counter
    ; load reference CRC value
    ld      r16,            Z+                      ; reference lo
    cp      lo,             r16
    ld      r16,            Z                       ; reference hi
    cpc     hi,             r16
    breq    page_write_start
#undef      lo
#undef      hi

    ; CRC error
    ldi     ZH,             hi8(Str_CK)
    ldi     ZL,             lo8(Str_CK)
    rcall   ser_print                               ; send "CK"
    rjmp    page_receive_init                       ; and start over to receive page again

    ; CRC check passed, write page data into flash memory
page_write_start:
#define     wordH           r1
#define     wordL           r0
#define     pageH           r3
#define     pageL           r2
#define     temp            r16
#define     temp2           r17
#define     count           r18
    ; prepare page address
    movw    WREG(XH,XL),    WREG(YH,YL)
    ld      pageH,          X+
    clr     pageL
    lsr     pageH                                   ; shift address bits to right position, PCPAGE in Z12:Z6
    ror     pageL
    lsr     pageH
    ror     pageL
    movw    WREG(ZH,ZL),    WREG(pageH,pageL)       ; load page address for page erase
    ; wait until EEPROM is idle
page_wait_eeprom:
    skbc    EECR,           EEWE,   rTmp
    rjmp    page_wait_eeprom
    ; wait until flash memory is idle
page_erase_wait:
    ldio    temp,           SPMCR
    sbrc    temp,           SPMEN
    rjmp    page_erase_wait
    ; erase page
    ldi     temp,           ((1 << PGERS) | (1 << SPMEN))
    stio    SPMCR,          temp
    spm
    ; wait until page has been erased
page_write_wait:
    ldio    temp,           SPMCR
    sbrc    temp,           SPMEN
    rjmp    page_write_wait
    ; load page buffer - initialization
    clr     ZH
    clr     ZL
    ldi     count,          (SPM_PAGESIZE / 2)
    ; load page buffer - loading loop
page_write_buffer_loop:
    ld      wordL,          X+
    ld      wordH,          X+
    ldi     temp,           (1 << SPMEN)
    stio    SPMCR,          temp
    spm
    adiw    WREG(ZH,ZL),    2                       ; set pointer to next flash byte address, word aligned
    subi    count,          1
    brne    page_write_buffer_loop                  ; continue if page buffer has not been completely filled yet
    ; page buffer has been loaded - write page buffer to flash memory
    movw    WREG(ZH,ZL),    WREG(pageH,pageL)
    ldi     temp,           ((1 << PGWRT) | (1 << SPMEN))
    stio    SPMCR,          temp
    spm

#undef      wordH
#undef      wordL
    clr     rZero                                   ; restore rZero

    ; Verify written page data
    ; wait until page has been written
page_verify_wait_write:
    ldio    temp,           SPMCR
    sbrc    temp,           SPMEN
    rjmp    page_verify_wait_write
    ; re-enable access to RWW flash memory section
    ldi     temp,           ((1 << RWWSRE) | (1 << SPMEN))
    stio    SPMCR,          temp
    spm
    ; initialization - Z already contains the correct page number
    movw    WREG(XH,XL),    WREG(YH,YL)
    adiw    XL,             1                       ; skip page number in received frame
    ldi     count,          SPM_PAGESIZE
    ; verification loop
page_verify_loop:
    lpm     temp,           Z+                      ; actual value
    ld      temp2,          X+                      ; reference value
    cpse    temp,           temp2
    rjmp    page_verify_failed                      ; mismatch -> write/read error
    ; page verification completed?
    subi    count,          1
    brne    page_verify_loop                        ; no -> test next byte
    ; Fertig
    ldi     ZH,             hi8(Str_OK)             ; set-up pointer to "OK" string ...
    ldi     ZL,             lo8(Str_OK)
    rcall   ser_print                               ; ... transmit string ...
    rjmp    page_receive_init                       ; ... and start over to receive next page

#undef      pageH
#undef      pageL
#undef      temp
#undef      temp2
#undef      count

    ; Verify failed ...
page_verify_failed:
    ldi     ZH,             hi8(Str_ER)             ; ... set-up pointer to "ER" string ...
    ldi     ZL,             lo8(Str_ER)
    rcall   ser_print                               ; ... transmit "ER" ...
    rjmp    page_receive_init                       ; ... and start over to receive page again



    ; Send a null-terminated character string (C string) using the IR communications interface
    ; and return to receive mode afterwards.
ser_print:
    ldi     r16,            (1 << TXC)              ; reset TX complete flag
    stio    UCSRA,          r16
    ldi     r16,            (1 << TXEN)             ; RX off, TX on
    stio    UCSRB,          r16
    ; wait until the next byte can be written into the TX register
ser_print_loop_wait:
    ldio    r16,            UCSRA
    sbrs    r16,            UDRE
    rjmp    ser_print_loop_wait
    ; write the next byte to the TX register
    ld      r16,            Z+
    and     r16,            r16
    breq    ser_print_finish                        ; null byte -> end of string/transmission
    stio    UDR,            r16
    rjmp    ser_print_loop_wait                     ; continue loop, wait until the next byte can be transmitted
ser_print_finish:
    ; wait until the transmission has been completed
    ldio    r16,            UCSRA
    sbrs    r16,            TXC
    rjmp    ser_print_finish
    ldi     r16,            (1 << RXEN)             ; TX off, RX on
    stio    UCSRB,          r16
    ret

; Constant data section starts here, get .text (flash) adress for initialization
P_Data_Start:


; Allocate constant data in .data (SRAM)
.data

R_Data_Start:      ; base adress for initialized data section
    Str_Flash:  .asciz "Flash"
    Str_ASURO:  .asciz "ASURO"
    Str_OK:     .asciz "OK"
    Str_CK:     .asciz "CK"
    Str_ER:     .asciz "ER"
    Str_VL:     .asciz "VL"
R_Data_End:

    .equ    P_Data_Size,    R_Data_End - R_Data_Start


; Allocate non-initialized data in .bss (SRAM)
.section .bss

    .equ    RX_Page_Size,   SPM_PAGESIZE + 2 + 1    ; page size (in byte, not words) + CRC16 + page number
    RX_Page:    .space      RX_Page_Size

#endif
