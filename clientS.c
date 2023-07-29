/**
 * Un ClientS, per verificare se un green pass è valido,
 * invia il codice di una tessera sanitaria al ServerG il
 * quale richiede al ServerV il controllo della validità.
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

    printf( "\n--------- BENVENUTO CLIENT S ----------\n" );

    // Descrittore del file socket
    int sockfd;
    // Struttura degli indirizzi
    struct sockaddr_in address;
    // Dati
    todo todoHandle;
    // Risposta
    int response = FLASE;

    // Copia la tessera sanitaria passata come argomento nel nostro array
    strncpy( todoHandle.code, argv[1], CODE_LENGTH );
    todoHandle.code[ CODE_LENGTH ] = '\0'; // Assicura che la stringa sia terminata correttamente

    // Conversione caratteri del codice della tessera sanitaria in maiuscolo
    for ( int i = 0; todoHandle.code[i]; i++ ) {
        todoHandle.code[i] = toupper( todoHandle.code[i] );
    }
    printf( "\nTessera Sanitaria CF: %s in verifica di validità...\n", todoHandle.code );
    strncpy( todoHandle.todo, ISVALID, 3 );
    printf( "\nTODO: %s", todoHandle.todo );

    // Creazione socket utilizzata per comunicare con il server
    // Per la comunicazione viene utilizzato un dominio AF_INET e protocollo TCP
    sockfd = Socket( AF_INET, SOCK_STREAM, 0 );

    // Impostazione degli indirizzi del server
    address.sin_family = AF_INET; // Dominio
    inet_pton( AF_INET, LOCAL_HOST, &address.sin_addr ); // Text to binary
    address.sin_port = htons( SERVERG_PORT ); // Host to network

    // Stabilisco una connessione con il server
    Connect( sockfd, ( struct sockaddr* ) &address, sizeof( address ) );

    // Invio il codice attraverso la FullWrite
    ssize_t nleftW = FullWrite( sockfd, &todoHandle, sizeof( todoHandle ) );

    // Risposta da parte del SERVER G utilizzando FullRead
    ssize_t nleftR = FullRead( sockfd, &response, sizeof( response ) );

    // Risposta
    if ( response ) {
        printf( "\nVerifica avvenuta con successo" );
        printf( "\nIl Green Pass risulta essere VALIDO" );
        printf( "\n-------- TUTTE LE OPERAZIONI --------\n" );
        printf( "---- SONO AVVENUTE CON SUCCESSO! ----\n" );
    } else {
        printf( "\nIl Green Pass risulta essere NON VALIDO" );
        printf( "\n------ OPERAZIONI TERMINATE -------\n" );
    }

    // Chiusura descrittore
    close( sockfd );
    printf( "\n" );
    return 0;
}