#ifndef GREEN_PASS_H
#define GREEN_PASS_H

#include <time.h>

#define CODE_LENGTH 16
// Definizione del periodo di validità (6 mesi in secondi)
#define VALIDITY_PERIOD (6 * 30 * 24 * 60 * 60)
// #define WRITE_GP 0
// #define CHECK_GP 1
// #define VALIDATION_GP 2

typedef struct green_pass {
    char code[ CODE_LENGTH ];
    time_t valid_from;
    time_t valid_until;
    int service;
} GreenPass;

/**
 * Funzione per generare Green Pass nuovo
 * 
 * @param code: Codice fiscale cliente
 * @return newGreenPass: Nuovo Green Pass
 */
GreenPass createGreenPass( const char code[] ) {
    GreenPass newGreenPass;

    // Copia il codice fornito nella struttura GreenPass
    strncpy( newGreenPass.code, code, CODE_LENGTH - 1 );
    newGreenPass.code[ CODE_LENGTH - 1 ] = '\0'; // Assicurati di terminare correttamente la stringa

    // Imposta la data di validità da adesso fino a 6 mesi dopo
    newGreenPass.valid_from = time( NULL );
    newGreenPass.valid_until = newGreenPass.valid_from + VALIDITY_PERIOD;

    // Service TRUE
    newGreenPass.service = 1;

    return newGreenPass;
}

void initAllGreenPass( GreenPass allGreenPass[] ) {
    // Vengono utilizzati 2 Green Pass di esempio

    // Alessio Esposito
    strncpy( allGreenPass[0].code, "SPSLSS00A01F839A", CODE_LENGTH );
    allGreenPass[0].code[ CODE_LENGTH - 1 ] = '\0'; // Assicurati di terminare correttamente la stringa

    // Imposta la data di validità da adesso fino a 6 mesi dopo
    allGreenPass[0].valid_from = time( NULL );
    allGreenPass[0].valid_until = allGreenPass[0].valid_from + VALIDITY_PERIOD;

    // Service TRUE
    allGreenPass[0].service = 1;

    // Angela Esposito
    strncpy( allGreenPass[1].code, "SPSNGL01A41F839D", CODE_LENGTH );
    allGreenPass[1].code[ CODE_LENGTH - 1 ] = '\0'; // Assicurati di terminare correttamente la stringa

    // Imposta la data di validità da adesso fino a 6 mesi dopo
    allGreenPass[1].valid_from = time( NULL );
    allGreenPass[1].valid_until = allGreenPass[1].valid_from + VALIDITY_PERIOD;

    // Service TRUE
    allGreenPass[1].service = 1;
}

void addGreenPass( GreenPass allGreenPass[], GreenPass newGreenPass, const int N ) {
    strncpy( allGreenPass[0].code, newGreenPass.code, CODE_LENGTH );
    allGreenPass[0].code[ CODE_LENGTH - 1 ] = '\0'; // Assicurati di terminare correttamente la stringa

    // Imposta la data di validità da adesso fino a 6 mesi dopo
    allGreenPass[0].valid_from = newGreenPass.valid_from;
    allGreenPass[0].valid_until = newGreenPass.valid_until;

    // Service TRUE
    allGreenPass[0].service = newGreenPass.service;
}

void printAllGreenPass( GreenPass allGreenPass[], const int N ) {
    for ( int i = 0; i < N; i++ ) {
        printf( "\nCodice Fiscale: %s", allGreenPass[i].code );
        printf( "\nData inizio: %s", ctime( &allGreenPass[i].valid_from ) );
        printf( "Data fine: %s", ctime( &allGreenPass[i].valid_until ) );
        printf( "Service: %d", allGreenPass[i].service );
        printf( "\n" );
    }
}

#endif // GREEN_PASS_H