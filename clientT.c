/**
 * Un ClientT, inoltre, può invalidare o ripristinare la validità
 * di un green pass comunicando al ServerG il contagio o la guarigione
 * di una persona attraverso il codice della tessera sanitaria.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <ctype.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "service/addresses.h"
#include "service/functions.h"
#include "service/green_pass.h"
#include "service/todo.h"

int main( int argc, char* argv[] ) {

    // Controllo effettuato per la verifica dei dati in input
    // in questo caso il numero della tessera sanitaria
    if ( argc != 2 ) {
        printf( "\nusage: %s <Codice Tessera Sanitaria>\n", argv[0] );
        exit( EXIT_FAILURE );
    }

    printf( "\n--------- BENVENUTO CLIENT T ----------\n" );

    // Descrittore del file socket
    int sockfd;
    // Struttura degli indirizzi
    struct sockaddr_in address;
    // Dati
    todo todoHandle;
    // Risposta
    int response = FLASE;
    int choice;

    // Copia la tessera sanitaria passata come argomento nel nostro array
    strncpy( todoHandle.code, argv[1], CODE_LENGTH );
    todoHandle.code[ CODE_LENGTH ] = '\0'; // Assicura che la stringa sia terminata correttamente

    // Conversione caratteri del codice della tessera sanitaria in maiuscolo
    for ( int i = 0; todoHandle.code[i]; i++ ) {
        todoHandle.code[i] = toupper( todoHandle.code[i] );
    }
    printf( "\nTessera Sanitaria CF: %s\n", todoHandle.code );

    // Creazione socket utilizzata per comunicare con il server
    // Per la comunicazione viene utilizzato un dominio AF_INET e protocollo TCP
    sockfd = Socket( AF_INET, SOCK_STREAM, 0 );

    // Impostazione degli indirizzi del server
    address.sin_family = AF_INET; // Dominio
    inet_pton( AF_INET, LOCAL_HOST, &address.sin_addr ); // Text to binary
    address.sin_port = htons( SERVERG_PORT ); // Host to network

    printf( "\n---- MENU ----" );
    printf( "\n1. Invalida il tuo Green Pass" );
    printf( "\n2. Ripristina il tuo Green Pass" );
    printf( "\nInserisci la tua scelta: " );
    scanf( "%d", &choice );


    switch ( choice ) {
        case 1: strncpy( todoHandle.todo, INVALIDATE, TODO_SIZE );
            break;
        case 2: strncpy( todoHandle.todo, RESTORE, TODO_SIZE );
            break;
        default: strncpy( todoHandle.todo, RESTORE, TODO_SIZE );
            break;
    }
    todoHandle.todo[ TODO_SIZE ] = '\0';

    // Stabilisco una connessione con il server
    Connect( sockfd, ( struct sockaddr* ) &address, sizeof( address ) );

    // Invio il codice attraverso la FullWrite
    FullWrite( sockfd, &todoHandle, sizeof( todoHandle ) );

    // Risposta da parte del SERVER G utilizzando FullRead
    FullRead( sockfd, &response, sizeof( response ) );

    // Risposta
    if ( response ) {
        printf( "\nOperazione avvenuta con successo" );
        printf( "\n-------- TUTTE LE OPERAZIONI --------\n" );
        printf( "---- SONO AVVENUTE CON SUCCESSO! ----\n" );
    } else {
        printf( "\n------ OPERAZIONI TERMINATE -------\n" );
    }

    // Chiusura descrittore
    close( sockfd );
    printf( "\n" );
    return 0;
}