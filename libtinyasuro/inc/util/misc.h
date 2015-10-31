/**
 * @file
 *
 * \if de
 * Definiert diverse Hilfs-Makros.
 *
 * Bezüglich der in den Makros verwendeten Attribute sei an dieser Stelle auf die
 * GCC-Dokumentation verwiesen.
 * \endif
 *
 * \if en
 * Defines various helper-macros.
 *
 * Please consult the GCC documentation for details about the used attributes.
 * \endif
 *
 * @author Markus Jung
 *
 * @version 14.06.2010 \n
 *  Erste Versionierung der Datei \n
 *  CONST und PURE hinzugefügt
 * @version 04.09.2013 \n
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

#ifndef MISC_H_
#define MISC_H_

/**
 * \if de
 * Setzt das GCC-Attribut "used", was dem Compiler mitteilt dass eine (scheinbar)
 * ungenutzte Funktion/Variable dennoch verwendet wird und nicht wegoptimiert
 * werden darf.
 *
 * Kann im Kontext von Funktionen und Variablen verwendet werden.
 * \endif
 *
 * \if en
 * Applies the GCC attribute "used" to a function/variable, telling the
 * compiler to not optimize away it, even if it appears to be unused.
 *
 * Can be applied to functions and variables.
 * \endif
 */
#define USED __attribute__((used))
/**
 * \if de
 * Setzt das GCC-Attribut "unused", was dem Compiler mitteilt, dass eine Funktion/
 * Variable oder ein Typ möglicherweise ungenutzt ist und dieser keine Warnung
 * diesbezüglich ausgeben soll.
 *
 * Kann im Kontext von Funktionen, Variablen und Typdefinitionen verwendet werden.
 * \endif
 *
 * \if en
 * Applies the GCC attribute "unused" to a function/variable, telling the
 * compiler to not emit warnings in case it is really not used.
 *
 * Can be applied to functions and variables.
 * \endif
 */
#define UNUSED __attribute__((unused))
/**
 * \if de
 * Setzt die GCC-Attribute "OS_main" und "noreturn", was bei main() einige
 * Instruktionen sparen kann.
 *
 * Kann im Kontext von Funktionen (genauer: main()) verwendet werden.
 * \endif
 *
 * \if en
 * Applies the GCC attributes "OS_main" and "noreturn" which might allow the
 * compiler to omit some instructions before/after main().
 *
 * Can be applied to functions (to be more exact: main()).
 * \endif
 */
#define MAIN __attribute__((OS_main,noreturn))
/**
 * \if de
 * Setzt das GCC-Attribut "naked", dadurch generiert der Compiler keinerlei
 * Pro-/Epilog.
 *
 * Kann im Kontext von Funktionen (genauer: Inline-Assembler) verwendet werden.
 * \endif
 *
 * \if en
 * Applies the GCC attribute "naked", telling the compiler to omit the pro-/
 * epilogue.
 *
 * Can be applied to functions (especially ISRs implemented in inline assembly).
 * \endif
 */
#define NAKED __attribute__((naked))
/**
 * \if de
 * Setzt inline und das GCC-Attribut "always_inline", eine so markierte Funktion
 * muss vom Compiler immer geinlined werden.
 *
 * Kann im Kontext von Funktionen genutzt werden.
 * \endif
 *
 * \if en
 * Applies the GCC attribute "always_inline". The compiler must inline every
 * occurance of a function attributed with "always_inline".
 *
 * Can be applied to functions.
 * \endif
 */
#define ALWAYS_INLINE inline __attribute__((always_inline))
/**
 * \if de
 * Setzt das GCC-Attribut "noinline", was dem Compiler das inlinen einer so
 * markierten Funktion verbietet.
 *
 * Kann im Kontext von Funktionen genutzt werden.
 * \endif
 *
 * \if en
 * Applies the GCC attribute "noinline" which prevents the compiler from
 * inlining attributed functions.
 *
 * Can be applied to functions.
 * \endif
 */
#define NOINLINE __attribute__((noinline))
/**
 * \if de
 * Ermöglicht es, eine Cleanup-Funktion für eine Variable zu spezifizieren.
 * Verlässt der Programmfluss den Geltungsbereich einer so markierten Variable,
 * wird die Funktion action mit einem Zeiger auf diese Variable aufgerufen.
 * Hat diese Funktion einen Rückgabewert, wird dieser ignoriert.
 *
 * Kann im Kontext von Variablen genutzt werden.
 *
 * @param action eine Funktion die einen Zeiger auf die markierte Variable
 *               übergeben bekommt.
 * \endif
 *
 * \if en
 * ON_CLEANUP (and the GCC attribute "cleanup(action)" offer the ability to
 * specify a so called cleanup function. If the scope of a variable attributed
 * this way is left, the function "action" will be called with a pointer to the
 * particular variable as argument. The return value of "action", if there is
 * any, will be ignored.
 *
 * Can be applied to variables.
 *
 *  @param action a function which takes a pointer to the attributed variable as
 *         argument.
 * \endif
 */
#define ON_CLEANUP(action) __attribute__((cleanup(action)))
/**
 * \if de
 * Setzt das GCC-Attribut "const", dies signalisiert dem Compiler dass der
 * Rückgabewert einer so markierte Funktion nur von den übergebenen Parametern
 * abhängt und davon abgesehen keine weiteren Seiteneffekte verursacht.
 *
 * Eine CONST-Funktion darf weder nicht-CONST-Funktionen aufrufen, noch
 * übergebene Pointer dereferenzieren. \n
 * Eine CONST-Funktion mit Rückgabetyp void ist sinnlos.
 *
 * Kann im Kontext von Funktionen genutzt werden.
 * \endif
 *
 * \if en
 * Applies the GCC attribute "const" to a function, telling the compiler its
 * return value does only depend on the passed araguments. Additionally this
 * function must not cause any side effects.
 *
 * A CONST function must not call any non-CONST function neither dereference
 * a passed pointer. \n
 * A CONST function returning "void" does not make any sense at all.
 *
 * Can be applied to functions.
 * \endif
 */
#define CONST __attribute__((const))
/**
 * \if de
 * Setzt das GCC-Attribut "pure", dies signalisiert dem Compiler dass der
 * Rückgabewert einer so markierte Funktion nur von den übergebenen Parametern
 * sowie globalen Variablen abhängt und davon abgesehen keine weiteren
 * Seiteneffekte verursacht.
 *
 * Eine PURE-Funktion darf ledliglich PURE- oder CONST-Funktionen aufrufen und
 * lesend globale Variablen zugreifen. Ebenso sind lediglich lesende Zugrife auf
 * den von übergebenen Pointern referenzierten Speicher erlaubt. \n
 * Eine PURE-Funktion mit Rückgabetyp void ist sinnlos.
 *
 * Kann im Kontext von Funktionen genutzt werden.
 * \endif
 *
 * \if en
 * Applies the GCC attribute "pure" to a function, telling the compiler its
 * return value does only depend on the passed arguments and global variables.
 * Additionally this function must not cause any side effects.
 *
 * A PURE function is only allowed to call PURE or CONST functions. It may only
 * read global variables or dereference passed pointers for read access. \n
 * A PURE function returning "void" does not make any sense at all.
 * \endif
 */
#define PURE __attribute__((pure))
/**
 * \if de
 * Setzt das GCC-Attribut "alias", was dem Compiler signalisiert, dass die damit
 * markierte Funktion nur ein Alias einer anderen Funktion (im gleichen
 * Quelltextdokument) ist.
 *
 * Kann im Kontext von Funktionsdeklarationen genutzt werden.
 * \endif
 *
 * \if en
 * Applies the GCC attribute "alias(of)" to a function, telling the compiler
 * the particular function declares just an alias to another function. The
 * aliased function must be defined in the same compilation unit.
 *
 * Can be applied to function declarations.
 * \endif
 */
#define ALIAS(of) __attribute__((alias(of)))
/**
 * \if de
 * Setzt das GCC-Attribut "deprecated", wird eine Funktion/Variable oder ein Typ
 * mit dieser Markierung im Quelltext verwendet, gibt der Compiler eine
 * entsprechende Warnung aus.
 *
 * @param msg ein zusätzlicher eigener Warntext, kann erst ab GCC >= 4.5
 *            verwendet werden
 * \endif
 *
 * \if en
 * Applies the GCC attribute "deprecated" to a function/variable or type, which
 * causes the compiler to emit a deprecation warning if an annotated
 * function/variable/type is used.
 *
 * @param msg an additional message to be printed, supported from GCC >= 4.5
 * \endif
 */
#define DEPRECATED(msg) __attribute__((deprecated(msg)))
/**
 * \if de
 * Setzt das GCC-Attribut "warning", wird eine Funktion mit dieser Markierung im
 * Quelltext verwendet, gibt der Compiler eine Warnung aus.
 *
 * @param msg ein zusätzlicher eigener Warntext
 * \endif
 *
 *  \if en
 * Applies the GCC attribute "warning" to a function, which causes the compiler
 * to emit a warning if an annotated function is used.
 *
 * @param msg an additional message to be printed
 * \endif
 */
#define WARNING(msg) __attribute__((warning(msg)))
/**
 * \if de
 * Setzt das GCC-Attribut "error", wird eine Funktion mit dieser Markierung im
 * Quelltext verwendet, gibt der Compiler eine Fehlermeldung aus.
 *
 * @param msg ein zusätzlicher eigener Fehlertext
 * \endif
 *
 * \if en
 * Applies the GCC attribute "error" to a function, which causes the compiler to
 * emit an error if an annotated function is used.
 * \endif
 */
#define ERROR(msg) __attribute__((error(msg)))

/**
 * \if de
 * Führt eine Typumwandlung der übergebenen Variable zum Zieltyp durch, ohne
 * den Parameter dabei zu modifizieren (entspricht einem reinterpreted cast).
 *
 * @param x die umzuwandelnde Variable
 * @param destType der Zieltyp
 *
 * @return x interpretiert als destType
 * \endif
 *
 * \if en
 * Converts the type of the passed variable to the destination type, without
 * any further processing (corresponds to a reinterpreted cast).
 *
 * @param x the variable to convert
 * @param destType the destination type
 *
 * @return x interpreted as destType
 * \endif
 */
#define UNION_CAST(x, destType) \
    (( (union {__typeof__(x) in; destType out;}) x).out)

#endif /* MISC_H_ */
