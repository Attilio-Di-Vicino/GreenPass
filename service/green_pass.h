#ifndef GREEN_PASS_H
#define GREEN_PASS_H

#include <time.h>

#define CODE_LENGTH 17
#define VALIDITY_PERIOD ( 6 * 30 * 24 * 60 * 60 )

typedef struct green_pass {
    char code[ CODE_LENGTH ];
    time_t valid_from;
    time_t valid_until;
    int validity;
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
    strncpy( newGreenPass.code, code, CODE_LENGTH );
    newGreenPass.code[ CODE_LENGTH ] = '\0'; // Assicurati di terminare correttamente la stringa

    // Imposta la data di validità da adesso fino a 6 mesi dopo
    newGreenPass.valid_from = time( NULL );
    newGreenPass.valid_until = newGreenPass.valid_from + VALIDITY_PERIOD;

    // validity TRUE
    newGreenPass.validity = 1;

    return newGreenPass;
}

/**
 * Funzione per inizializzazione Green Pass
 * 
 * @param allGreenPass: Array Green Pass
 */
void initAllGreenPass( GreenPass allGreenPass[] ) {
    // Vengono utilizzati 2 Green Pass di esempio

    // Alessio Esposito
    strncpy( allGreenPass[0].code, "SPSLSS00A01F839A", CODE_LENGTH );
    allGreenPass[0].code[ CODE_LENGTH ] = '\0'; // Assicurati di terminare correttamente la stringa

    // Imposta la data di validità da adesso fino a 6 mesi dopo
    allGreenPass[0].valid_from = time( NULL );
    allGreenPass[0].valid_until = allGreenPass[0].valid_from + VALIDITY_PERIOD;

    // validity TRUE
    allGreenPass[0].validity = 1;

    // Angela Esposito
    strncpy( allGreenPass[1].code, "SPSNGL01A41F839D", CODE_LENGTH );
    allGreenPass[1].code[ CODE_LENGTH ] = '\0'; // Assicurati di terminare correttamente la stringa

    // Imposta la data di validità da adesso fino a 6 mesi dopo
    allGreenPass[1].valid_from = time( NULL );
    allGreenPass[1].valid_until = allGreenPass[1].valid_from + VALIDITY_PERIOD;

    // validity TRUE
    allGreenPass[1].validity = 1;
}

/**
 * Funzione per aggiungere Green Pass
 * 
 * @param allGreenPass: Array Green Pass
 * @param newGreenPass: Green Pass da aggiungere
 * @param N: Size di allGreenPass
 */
void addGreenPass( GreenPass allGreenPass[], GreenPass newGreenPass, const int N ) {
    strncpy( allGreenPass[0].code, newGreenPass.code, CODE_LENGTH );
    allGreenPass[0].code[ CODE_LENGTH ] = '\0'; // Assicurati di terminare correttamente la stringa

    // Imposta la data di validità da adesso fino a 6 mesi dopo
    allGreenPass[0].valid_from = newGreenPass.valid_from;
    allGreenPass[0].valid_until = newGreenPass.valid_until;

    // validity TRUE
    allGreenPass[0].validity = newGreenPass.validity;
}

/**
 * Funzione per stampare Green Pass
 * 
 * @param allGreenPass: Array Green Pass
 * @param N: Size di allGreenPass
 */
void printAllGreenPass( GreenPass allGreenPass[], const int N ) {
    for ( int i = 0; i < N; i++ ) {
        printf( "\nCodice Fiscale: %s", allGreenPass[i].code );
        printf( "\nData inizio: %s", ctime( &allGreenPass[i].valid_from ) );
        printf( "Data fine: %s", ctime( &allGreenPass[i].valid_until ) );
        printf( "validity: %d", allGreenPass[i].validity );
        printf( "\n" );
    }
}

#endif // GREEN_PASS_H