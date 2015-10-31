/**
 * @file
 *
 * \if de
 * Definiert und implementiert verschiedene Typen, Funktionen und Makros zum
 * atomaren und/oder geschützten Durchführen von Operationen und Zuweisungen.
 *
 * Der Umfang dieses Headers gliedert sich im wesentlichen in zwei Teile:
 * @li Hilfsmakros zum Erzeugen von Speicherbarrieren, verankern von Variablen
 *     und durchführen von atomaren Zuweisungen
 * @li Das Makro LOCKED_BLOCK und einige Hilfsmakros/Inline-Funktionen zum
 *     spezifizieren eines duch eine Lock-Variable (vom Typ lock_t) geschützten
 *     Codeblocks
 *
 * @warning Die hier definierten/implementierten Funktionen haben
 *          experimentellen Status. Speziell TIE_VARIABLE() und SAFE_ASSIGN()
 *          sind bezüglich ihrer tatsächlichen Auswirkungen weitestgehend
 *          ungeprüft. Es ist möglich dass deren Verwendung nicht den
 *          gewünschten Effekt oder sogar negative Auswirkungen mit sich bringt.
 * \endif
 *
 * \if en
 * Defines and implements several types, functions and macros for executing
 * atomic/protected operations and assigments.
 *
 * This headerfile might be divided into two parts:
 * @li Some helper-macros for memory barriers, anchoring variables and execute
 *     atomic assigments
 * @li The macro LOCKED_BLOCK as well as several helper-macros and
 *     inline-functions for creating/accessing critical sections protected by
 *     a lock-variable (of the type lock_t)
 *
 * @warning The functions defined/implemented here are highly experimental.
 *          Especially TIE_VARIABLE() and SAFE_ASSIGN() are unevaluated
 *          regarding their real behaviour. It is possible that their usage
 *          does not have any effects at all, but might also lead to unexpected
 *          negative side-effects.
 * \endif
 *
 * @author Markus Jung
 *
 * @version 25.05.2010 \n
 *  Lock-Implementierung
 * @version 26.05.2010 \n
 *  __defaultValues wegrationalisiert
 * @version 18.06.2010 \n
 *  Rewrite der LOCK_ Funktionen, Vereinfachungen \n
 *  Bugfix TIE_VARIABLE \n
 *  Doku
 * @version 05.11.2010 \n
 *  English documentation (at least a small part)
 * @version 11.04.2011 \n
 *  Kleiner Tippfehler in der Doku behoben
 * @version 13.04.2013 \n
 *  Complete English documentation \n
 *  Translate comments into English
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

#ifndef LOCK_H_
#define LOCK_H_

#include <stdbool.h>
#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "misc.h"

/**
 * \if de
 * Erzeugt eine Speicherbarriere die GCC dazu zwingt, alle SRAM-Variablen
 * zurückzuschreiben beziehungsweise neu einzulesen.
 * \endif
 *
 * \if en
 * Creates a memory barrier which forces GCC to reread/-write all variables
 * located at the SRAM.
 * \endif
 */
#ifdef __DOXYGEN__
#define MEM_BARRIER()
#else
#define MEM_BARRIER() asm volatile ("; Barrier " ::: "memory")
#endif

/**
 * \if de
 * Erzwingt, dass ein eine Variable zum Zeitpunkt der Verwendung dieses Makros
 * vollständig berechnet ist. Es sollte dem Compiler nicht möglich sein,
 * Teile der Berechnung auf einen Zeitpunkt nach dem Aufruf dieses Makros zu
 * verschieben.
 *
 * Dieses Makro verhindert nicht, dass die übergebene Variable vom Compiler
 * wegoptimiert wird, wenn sie nicht benötigt wird.
 *
 * @warning Die Verwendung dieses Makros ist auf numerische Datentypen
 *          beschränkt. Die Übergabe von structs als Parameter kann, muss aber
 *          nicht funktionieren.
 *
 * @param var die zu "verankernde" Variable
 * \endif
 *
 * \if en
 * Forces the compiler to finish the computation of a variable before the usage
 * of this macro. It should prevent that parts of the computation can be moved/
 * delayed.
 *
 * This macro does not prevent the removal of the passed variable by the
 * compiler, if unused.
 *
 * @warning The usage of this macro is restricted to numeric types. Passing
 *          structs or other data might work, but does not have to.
 *
 * @param var the variable to be "anchored"
 * \endif
 */
#ifdef __DOXYGEN__
#define TIE_VARIABLE(var)
#else
#define TIE_VARIABLE(var) asm ("" : [io] "=r"(var) : "[io]" (var))
#endif

/**
 * \if de
 * Ermöglicht es, eine atomare Zuweisung @bcode{dst = src;} durchzuführen.
 * Es wird erzwungen, dass der originale Wert von "dst" bei Deaktivierung der
 * Interruptfreigabe noch vorhanden und nach Reaktivierung der Interruptfreigabe
 * die Zuweisung erfolgt ist.
 *
 * Die Implementierung dieses Makros erfordert, dass alle zuzuweisenden Daten
 * in Registern gehalten werden, eine Verwendung bei großen Dateneinheiten
 * macht daher wenig Sinn.
 *
 * @warning Die Verwendung dieses Makros ist auf numerische Datentypen
 *          beschränkt. Die Übergabe von structs als Parameter kann, muss aber
 *          nicht funktionieren.
 *
 * @param dst das Zuweisungsziel
 * @param src die zuzuweisende Wert/Variable
 * \endif
 *
 * \if en
 * Executes an atomic assigment @bcode{dst = src;} The compiler is forced to
 * keep the old value of "dst" until interrupts are temporary disabled. The
 * assigment has to be finished before interrupts are re-enabled.
 *
 * The way this macro is implemented requires that all data to be assigned can
 * be passed in registers. Thus, using it with larger quantities does not make
 * sense.
 *
 * @warning The usage of this macro is restricted to numeric types. Passing
 *          structs or other data might work, but does not have to.
 *
 * @param dst the target of the assigment
 * @param src the value to be assigned
 * \endif
 */
#ifdef __DOXYGEN__
#define SAFE_ASSIGN(dst, src)
#else
#define SAFE_ASSIGN(dst, src) do { \
        uint8_t __sreg = SREG; \
        cli(); \
        TIE_VARIABLE(dst); \
        dst = (src); \
        TIE_VARIABLE(dst); \
        SREG = __sreg; \
    } while(false)
#endif

/**
 * \if de
 * Definiert den blanken lock-Typ ohne volatile (für interne Zwecke).
 * \endif
 *
 * \if en
 * Defines the naked type for locking without volatile modifier
 * (for internal use only).
 * \endif
 */
typedef uint8_t __lock_base_t;

/**
 * \if de
 * Definiert den Typ lock zur Verwendung mit dem Makro LOCKED_BLOCK.
 * \endif
 *
 * \if en
 * Defines the lock type for use with the macro LOCKED_BLOCK.
 * \endif
 */
typedef volatile __lock_base_t lock_t;

#ifndef __DOXYGEN__
/**
 * \if de
 * Der innerhalb von LOCKED_BLOCK verwendete Datentyp.
 * \endif
 *
 * \if en
 * The datatype internally used by LOCKED_BLOCK.
 * \endif
 */
typedef struct {
    lock_t * const lock_ptr;
    bool loop;
} __lock_struct_t;
#endif

/**
 * \if de
 * Diese Funktion wartet, bis der übergebene Lock frei ist und sperrt ihn dann
 * selbst.
 *
 * Es wird davon ausgegangen, dass beim Aufruf dieser Funktion Interrupts
 * freigeschaltet sind, die globale Interruptfreigabe wird während des
 * Wartevorganges wiederholt ein- und ausgeschaltet. Nach Ausführung dieser
 * Funktion bleibt die globale Interruptfreigabe aktiv.
 *
 * @param lock der zu sperrende Lock
 *
 * @return immer true
 * \endif
 *
 * \if en
 * This function waits until the passed lock gets released and acquires the lock
 * afterwards.
 *
 * It is assumed that interrups are globally enabled while calling this function.
 * Internally, the global interrupt flag will be toggled repeatedly. Interrupts
 * will be left globally enabled after the execution of this function-
 *
 * @param lock the lock which should be acquired
 *
 * @return always true
 * \endif
 */
static ALWAYS_INLINE bool __lockWait(lock_t * const lock) {
    // Lock can only be released inside of an interrupt, so we have to toggle
    // the global interrupt flag and test the lock while interrups are disabled

    do { // Interrupts must be enabled since otherwise this loop ends never
        sei();
        // enable interrupts for a very short term to allow ISR execution
        cli();
    } while (*lock != 0); // read lock value protected from ISRs

    *lock = 1; // acquire lock

    MEM_BARRIER();
    sei();

    return true;
}

/**
 * \if de
 * Diese Funktion inkrementiert den übergebenen Lock um eins, was effektiv
 * bedeutet dass ein freier Lock gesperrt wird, bei mehren Sperrungen wird die
 * Anzahl um eins erhöht.
 *
 * @param lock der zu inkrementierende Lock
 *
 * @return true wenn das Lock vorher frei war, sonst false
 * \endif
 *
 * \if en
 * This function increments the passed lock by one, effectively acquiring the
 * lock if it has not been before, else the lock reflects how often the lock has
 * been acquired.
 *
 * @param lock the lock which should be incremented
 *
 * @return true if the lock got acquired, false if it had been already acquired
 *         somewere elese
 * \endif
 */
static ALWAYS_INLINE bool __lockOverride(lock_t * const lock) {
    const uint8_t sreg = SREG;

    cli();

    const __lock_base_t lockValue = *lock; // lock is volatile, copy to register

    // Increment in any case. If the lock has already been acquired, it will
    // be decremented by __lockRelease without execution of the LOCKED_BLOCK
    // (because of !lockValue)
    *lock = lockValue + 1;

    MEM_BARRIER();
    SREG = sreg;

    return !lockValue;
}

/**
 * \if de
 * Dekrementiert das in der übergebenen Datenstruktur referenzierte Lock um eins.
 *
 * @param lstruct die interne Lock-Datenstruktur
 * \endif
 *
 * \if en
 * Decrements the lock of the passed structure by one.
 *
 * @param lstruct the internally used locking structure
 * \endif
 */
static ALWAYS_INLINE void __lockRelease(__lock_struct_t * lstruct) {
    const uint8_t sreg = SREG;

    cli();
    (*(lstruct->lock_ptr))--;

    MEM_BARRIER();
    SREG = sreg;
}

/**
 * \if de
 * Im Modus LOCK_WAIT wird der LOCKED_BLOCK erst ausgeführt, nachdem das Lock
 * freigegeben wurde. Während der Ausführung des LOCKED_BLOCK wird das Lock
 * belegt.
 *
 * In diesem Modus kann nur ein Interrupt das Lock freigeben und die Ausführung
 * des LOCKED_BLOCK auslösen, es wird daher davon ausgegangen, dass Interrupts
 * global aktiviert wurden, bei Beginn der Ausführung des Codes innerhalb des
 * LOCKED_BLOCK findet eine globale Interruptfreigabe statt!
 *
 * Die Verwendung dieses Modus macht nur Sinn, wenn die Lock-Variable innerhalb
 * einer ISR manuell verändert wird, ohne dabei die hier spezifizierten Makros
 * zu verwenden.
 *
 * @warning Innerhalb einer ISR kann LOCK_WAIT nicht verwendet werden, dies
 *          würde einen Deadlock provozieren.
 * \endif
 *
 * \if en
 * A LOCKED_BLOCK with mode LOCK_WAIT will not be executed until the lock has
 * been released. Before execution, the lock will be acquired and remain locked
 * until the execution of the LOCKED_BLOCK has been finished.
 *
 * Only an interrupt can release the lock for which the LOCKED_BLOCK is waiting
 * for. Therefore LOCK_WAIT is assuming that interrupts have been globally
 * enabled. During waiting, the global interrupt enable bit will be toggled
 * periodically, it will remain set after the lock has been acquired.
 *
 * The mode LOCK_WAIT is only useful if the variable used for locking is
 * modified by an ISR, since no two LOCKED_BLOCKs can be active in parallel.
 *
 * @warning LOCK_WAIT must not be used within an ISR, as this would result in a
 *          deadlock.
 * \endif
 */
#ifdef __DOXYGEN__
#define LOCK_WAIT
#else
#define LOCK_WAIT       __lockWait
#endif
/**
 * \if de
 * Im Modus LOCK_OVERRIDE wird der LOCKED_BLOCK immer ausgeführt.
 * Für die Dauer der Ausführung des LOCKED_BLOCK wird das Lock um eins
 * inkrementiert.
 *
 * Eine Verwendung dieses Modus bietet sich vor allem an, wenn ein Codeblock
 * unabhängig vom Lock-Status ausgeführt werden soll, während andere Codeblöcke
 * warten müssen, oder wenn der Lock-Status zu anderen Zwecken erfasst werden
 * soll.
 * \endif
 *
 * \if en
 * A LOCKED_BLOCK with mode LOCK_OVERRIDE will always be executed.
 * The lock value will be incremented before the LOCKED_BLOCK is executed, and
 * decremented afterwards.
 *
 * Using this mode is advised especially if a block of code should be
 * executed in every case and independent of the lock value, while other code
 * blocks should wait.
 * \endif
 */
#ifdef __DOXYGEN__
#define LOCK_OVERRIDE
#else
#define LOCK_OVERRIDE   true | __lockOverride
#endif
/**
 * \if de
 * Im Modus LOCK_SKIP wird der LOCKED_BLOCK nur genau dann ausgeführt, wenn das
 * Lock zuvor nicht belegt wurde. Während der Ausführung des LOCKED_BLOCK wird
 * das Lock belegt.
 *
 * Eine Verwendung dieses Modus liegt vor allem innerhalb von ISRs nahe, da
 * so Codeabschnitte von der Ausführung ausgeschlossen werden können, die den
 * korrekten Ablauf des anderer Ausführungspfade stören würden.
 * \endif
 *
 * \if en
 * A LOCKED_BLOCK with mode LOCK_SKIP will be executed only if the lock has not
 * been acquired by other LOCKED_BLOCKs. The lock will be acquired during
 * execution.
 *
 * This mode is suited best for use in ISRs because this way code paths there
 * can be skipped temporary to prevent side effects with regular code.
 * \endif
 */
#ifdef __DOXYGEN__
#define LOCK_SKIP
#else
#define LOCK_SKIP       __lockOverride
#endif

/**
 * \if de
 * Dieses Makro ermöglicht es, die Ausführung von durch Lock-Variablen
 * geschützten Codeabschnitten auf verschiedene Art und Weise zu steuern.
 *
 * Unter Verwendung Varianten LOCK_WAIT, LOCK_OVERRIDE und LOCK_SKIP ist es
 * möglich, verschiedene Eintrittsbedingungen für den geschützten Codeabschnitt
 * festzulegen.
 *
 * Alle Zugriffe auf die Lock-Variable finden garantiert atomar statt, es gibt
 * bezüglich der Verwendung von LOCKED_BLOCK keine Einschränkungen (man beachte
 * aber die Hinweise zu LOCK_WAIT).
 *
 * Je nach verwendete Eintrittsbedingung können sich auch mehrere
 * Ausführungspfade gleichzeitig innerhalb von durch die gleiche Lock-Variable
 * geschützten Codeabschnitten befinden (etwa bei der Verwendung von
 * unterbrechbaren ISRs). Da die Lock-Variablen als Zähler genutzt werden, geben
 * diese zu jedem Zeitpunkt die Anzahl der Ausführungspfade wieder, die sich
 * innerhalb eines duch dieses Lock geschützten Abschnittes aufhalten, diesen
 * verlassen oder die Eintrittsbedingung prüfen.
 *
 * Anwendungsbeispiel:
 * @code
 * lock_t lockvar; // (globale) Lock-Variable
 *
 * // Anderer Code, Funktionen uvm. ...
 *
 * // Funktion mit LOCKED_BLOCK
 * // ungeschützter Code
 * LOCKED_BLOCK(lockvar, LOCK_SKIP) {
 *      // geschützter Code
 * }
 * // noch mehr ungeschützter Code
 * @endcode
 *
 * @param __lock die Lock-Variable, die bei den Eintrittsbedingungen augewertet
 *               und verändert wird
 * @param __action die Eintrittsbedingung/Aktion, LOCK_WAIT, LOCK_OVERRIDE oder
 *                 LOCK_SKIP
 * \endif
 *
 * \if en
 * The macro LOCKED_BLOCK supports several ways protect/mutally exclude code
 * paths based, on lock variables.
 *
 * Currently, three lock modes are supported, LOCK_WAIT, LOCK_OVERRIDE and
 * LOCK_SKIP. They specify different conditions which have to be met before a
 * LOCKED_BLOCK will be executed.
 *
 * The variable used for locking will always be accessed in an atomar way.
 * LOCKED_BLOCK can be used without any restrictions, but the locking mode has
 * to be chosen carefully to prevent deadlocks (especially if LOCK_WAIT is used).
 *
 * Execution of more than one LOCKED_BLOCK in parallel is possible, depending on
 * the chosen lock mode, especially if ISRs can interrupt themselfes. Since the
 * lock variable is used as counter, the value of this variable always equals
 * to the number of code paths which are executing a LOCKED_BLOCK protected by
 * this lock variable in parallel.
 *
 * Example:
 * @code
 * lock_t lockvar; // (global) lock variable
 *
 * // some other code, functions ...
 *
 * // function containing a LOCKED_BLOCK
 * // unprotected code
 * LOCKED_BLOCK(lockvar, LOCK_SKIP) {
 *      // protected code
 * }
 * // some additional unprotected code
 * @endcode
 *
 * @param __lock the lock variable used to protected the LOCKED_BLOCK
 * @param __action the lock mode, LOCK_WAIT, LOCK_OVERRIDE or LOCK_SKIP
 * \endif
 */
// Doxygen dislikes macros arguments -> @code and @endcode instead of @bcode
#ifdef __DOXYGEN__
#define LOCKED_BLOCK(__lock, __action)
#else
#define LOCKED_BLOCK(__lock, __action) \
    for( \
        __lock_struct_t __lstruct ON_CLEANUP(__lockRelease) = \
            ({ \
                __lock_struct_t lval = { \
                        .lock_ptr = &__lock, \
                        .loop = __action(&__lock), \
                    }; \
                lval; \
            }); \
        __lstruct.loop; \
        __lstruct.loop = false \
    )
#endif

#endif /* LOCK_H_ */
