/**
 * Un utente, una volta effettuata la vaccinazione, tramite un client
 * si collega ad un centro vaccinale e comunica il codice della propria tessera sanitaria.
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

#define CODE_LENGTH 16

int main( int argc, char* argv[] ) {

    // Controllo effettuato per la verifica dei dati in input
    // in questo caso il numero della tessera sanitaria
    if ( argc != 2 ) {
        printf( "\nusage: %s <Codice Tessera Sanitaria>\n", argv[0] );
        exit( EXIT_FAILURE );
    }

    printf( "\n---- BENVENUTO CLIENT ----\n" );

    // Descrittore del file socket
    int sockfd;
    // Struttura degli indirizzi
    struct sockaddr_in address;
    // Codice tessera sanitaria
    char code[ CODE_LENGTH ];
    // Risposta dal centro vaccinale
    GreenPass myGreenPass;

    // Copia la tessera sanitaria passata come argomento nel nostro array
    strncpy( code, argv[1], CODE_LENGTH - 1 );
    code[ CODE_LENGTH - 1 ] = '\0'; // Assicura che la stringa sia terminata correttamente

    // Conversione caratteri del codice della tessera sanitaria in maiuscolo
    for ( int i = 0; code[i]; i++ ) {
        code[i] = toupper( code[i] );
    }
    printf( "\nTessera Sanitaria CF: %s in fase di accettazione...\n", code );

    // Creazione socket utilizzata per comunicare con il centro vaccinale
    // Per la comunicazione viene utilizzato un dominio AF_INET e protocollo TCP
    sockfd = Socket( AF_INET, SOCK_STREAM, 0 );

    // Impostazione degli indirizzi del server
    address.sin_family = AF_INET; // Dominio
    inet_pton( AF_INET, LOCAL_HOST, &address.sin_addr ); // Text to binary
    address.sin_port = htons( CENTER_PORT ); // Host to network

    // Stabilisco una connessione con il server
    Connect( sockfd, ( struct sockaddr* ) &address, sizeof( address ) );

    // Invio il codice attraverso la FullWrite
    ssize_t nleftW = FullWrite( sockfd, &code, sizeof( code ) );

    // Risposta da parte del centro vaccinale utilizzando FullRead
    ssize_t nleftR = FullRead( sockfd, &myGreenPass, sizeof( myGreenPass ) );

    // Green pass
    if ( myGreenPass.service ) {
        printf( "\nAccettazione avvenuta con successo green pass:" );
        printf( "\nCodice Fiscale: %s", myGreenPass.code );
        printf( "\nData inizio: %s", ctime( &myGreenPass.valid_from ) );
        printf( "Data fine: %s", ctime( &myGreenPass.valid_until ) );
    } else {
        printf( "\nAccettazione fallita." );
    }

    // Chiusura descrittore
    close( sockfd );

    printf( "\n-------- TUTTE LE OPERAZIONI --------\n" );
    printf( "---- SONO AVVENUTE CON SUCCESSO! ----\n" );

    printf( "\n" );
    return 0;
}