/**
 * @file
 *
 * \if de
 * Leere Projektvorlage als Grundlage für eigene Projekte.
 * \endif
 *
 * \if en
 * Empty project template as starting point for own projects.
 * \endif
 *
 * @author Markus Jung
 *
 * @version 29.08.2010 \n
 *  Angelegt
 * @version 10.02.2014 \n
 *  English translation
 */
#include <asuro/asuro.h>
#include <util/misc.h>

MAIN void main(void) {
    Init();
    
    for (;;) {
		sleepUntilInterrupt();
    };
}
