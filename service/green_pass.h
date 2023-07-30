#ifndef GREEN_PASS_H
#define GREEN_PASS_H

#include <time.h>
#include "todo.h"

#define CODE_LENGTH 17
#define VALIDITY_PERIOD ( 6 * 30 * 24 * 60 * 60 )
#define MAX_NUM_OF_GREEN_PASS 120
#define FLASE 0
#define TRUE 1
#define TODO_SIZE 4

typedef struct green_pass {
    char code[ CODE_LENGTH ];
    time_t valid_from;
    time_t valid_until;
    int validity;
    char toCheck[ TODO_SIZE ];
} GreenPass;

// Green Pass presenti
GreenPass allGreenPass[ MAX_NUM_OF_GREEN_PASS ];
static int count = 0;

int getSizeAllGreenPass() {
    return count;
}

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
    newGreenPass.validity = TRUE;
    strncpy( newGreenPass.toCheck, OTHER, TODO_SIZE );
    newGreenPass.toCheck[ TODO_SIZE ] = '\0';

    return newGreenPass;
}

/**
 * Procedura per inizializzazione Green Pass
*/
void initAllGreenPass() {
    // Vengono utilizzati 3 Green Pass di esempio

    // Alessio Esposito
    strncpy( allGreenPass[0].code, "SPSLSS00A01F839A", CODE_LENGTH );
    allGreenPass[0].code[ CODE_LENGTH ] = '\0'; // Assicurati di terminare correttamente la stringa

    // Imposta la data di validità da adesso fino a 6 mesi dopo
    allGreenPass[0].valid_from = time( NULL );
    allGreenPass[0].valid_until = allGreenPass[0].valid_from + VALIDITY_PERIOD;

    // validity TRUE
    allGreenPass[0].validity = TRUE;
    strncpy( allGreenPass[0].toCheck, OTHER, TODO_SIZE );
    allGreenPass[0].toCheck[ TODO_SIZE ] = '\0';

    // Angela Esposito
    strncpy( allGreenPass[1].code, "SPSNGL01A41F839D", CODE_LENGTH );
    allGreenPass[1].code[ CODE_LENGTH ] = '\0'; // Assicurati di terminare correttamente la stringa

    // Imposta la data di validità da adesso fino a 6 mesi dopo
    allGreenPass[1].valid_from = time( NULL );
    allGreenPass[1].valid_until = allGreenPass[1].valid_from + VALIDITY_PERIOD;

    // validity TRUE
    allGreenPass[1].validity = TRUE;
    strncpy( allGreenPass[1].toCheck, OTHER, TODO_SIZE );
    allGreenPass[1].toCheck[ TODO_SIZE ] = '\0';

    // Attilio Di Vicino
    strncpy( allGreenPass[2].code, "DVCTTL97T30F839P", CODE_LENGTH );
    allGreenPass[2].code[ CODE_LENGTH ] = '\0'; // Assicurati di terminare correttamente la stringa

    // Imposta la data di validità da adesso fino a 6 mesi dopo
    allGreenPass[2].valid_from = time( NULL );
    allGreenPass[2].valid_until = allGreenPass[1].valid_from + VALIDITY_PERIOD;

    // validity TRUE
    allGreenPass[2].validity = TRUE;
    strncpy( allGreenPass[2].toCheck, OTHER, TODO_SIZE );
    allGreenPass[2].toCheck[ TODO_SIZE ] = '\0';
    count = 3;
}

/**
 * Procedura per aggiungere Green Pass
 * 
 * @param newGreenPass: Green Pass da aggiungere
*/
void addGreenPass( GreenPass newGreenPass ) {
    strncpy( allGreenPass[ count ].code, newGreenPass.code, CODE_LENGTH );
    allGreenPass[ count ].code[ CODE_LENGTH ] = '\0'; // Assicurati di terminare correttamente la stringa

    // Imposta la data di validità da adesso fino a 6 mesi dopo
    allGreenPass[ count ].valid_from = newGreenPass.valid_from;
    allGreenPass[ count ].valid_until = newGreenPass.valid_until;

    // validity TRUE
    allGreenPass[ count ].validity = newGreenPass.validity;
    strncpy( allGreenPass[ count ].toCheck, OTHER, TODO_SIZE );
    allGreenPass[ count++ ].toCheck[ TODO_SIZE ] = '\0';
}

/**
 * Funzione controllo validità codice fiscale
 * 
 * @param code: code da verificare
 * @return validità del green pass
*/
int checkValidity( char code[] ) {
    for ( int i = 0; i < count; i++ )
        if ( strcmp( code, allGreenPass[i].code ) == 0 )
            return allGreenPass[i].validity;
    return FLASE;
}

/**
 * Procedura cambio validità
 * 
 * @param code: code
 * @param chval: validità da inserire
*/
void changeValidity( char code[], int chval ) {
    for ( int i = 0; i < count; i++ )
        if ( strcmp( code, allGreenPass[i].code ) == 0 ) {
            allGreenPass[i].validity = chval;
            break;
        }
} 

/**
 * Procedura per stampare Green Pass
*/
void printAllGreenPass() {
    for ( int i = 0; i < count; i++ ) {
        printf( "\nCodice Fiscale: %s", allGreenPass[i].code );
        printf( "\nData inizio: %s", ctime( &allGreenPass[i].valid_from ) );
        printf( "Data fine: %s", ctime( &allGreenPass[i].valid_until ) );
        printf( "validity: %d", allGreenPass[i].validity );
        printf( "\n" );
    }
}

#endif // GREEN_PASS_H