/*
 * Dieses Programm soll einige Funktionen des Asuro testen.
 * Füllt die Lücken aus um das Programm fertigzustellen.
 * Die Kommentare beschreiben euch was ihr tun müsst.
 */
#include <stdint.h>
#include <asuro/asuro.h>
#include <util/misc.h>

void radTest() {
    //Schalte die Motoren so, dass das rechte Rad rückwärts fährt:

    msleep(500);
    //Schalte die Motoren so, dass das linke Rad rückwärts fährt:

    msleep(500);
    // Stoppe die Motoren (Bremse):

    msleep(500);
    //Schalte die Motoren so, dass der Asuro gerade aus fährt fährt:

    msleep(500);
    // Stoppe wieder die Motoren:

    // fertig!
}

void lichtTest() {
    // Schalte hier die Front-LED an:

    msleep(1000);
    // Schalte hier die rechte Back-LED an:

    msleep(1000);
    // Schalte die rechte Back-LED aus und dafür die linke an:

    msleep(1000);
    // Schalte die Status-LED auf Rot:

    msleep(1000);
    // Schalte beide Back-LEDs aus:

    // Schalte die Status-LED auf grün:

    // fertig!
}

void lichtsensorTest() {
    int data[] = {0,0};
    // Speichere den Lichtwert in data ab:

    // ---
    // Ändere die If-Bedingung so, dass immer die Back-LED leuchtet,
    // auf der mehr Licht empfangen wird:
    if ( ___ > ___) {
        // Schalte die rechte LED an:

    } else {
        // Schalte die linke LED an:

    }
}

odometrieTest() {
    // TODO test schreiben :D
}

kommunikationsTest() {
    // TODO test schreiben :D
}

MAIN void main(void) {
    Init();

    SerWrite("Asuro Selbsttest. Test\n", 12);

    radTest();
    lichtTest();
    lichtsensorTest();
    odometrieTest();
    kommunikationsTest();
}
