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
#include "addresses.h"

#define CODE_LENGTH 16

int main( int argc, char* argv[] ) {

    // Controllo effettuato per la verifica dei dati in input
    // in questo caso il numero della tessera sanitaria
    if ( argc != 2 ) {
        printf( "\nusage: %s <Codice Tessera Sanitaria>\n", argv[0] );
        exit( EXIT_FAILURE );
    }

    // Descrittore del file socket
    int sockfd;
    // Struttura degli indirizzi
    struct sockaddr_in address;
    // Codice tessera sanitaria
    char code[ CODE_LENGTH ];
    // Risposta dal centro vaccinale
    int response;

    // Copia la tessera sanitaria passata come argomento nel nostro array
    strncpy( code, argv[1], sizeof( code ) - 1 );
    code[ sizeof( code ) - 1 ] = '\0'; // Assicura che la stringa sia terminata correttamente

    // Conversione caratteri del codice della tessera sanitaria in maiuscolo
    for ( int i = 0; code[i]; i++ ) {
        code[i] = toupper( code[i] );
    }
    printf( "\nTessera Sanitaria Cliente: %s\n", code );

    // Creazione socket utilizzata per comunicare con il centro vaccinale
    // Per la comunicazione viene utilizzato un dominio AF_INET e protocollo TCP
    sockfd = socket( AF_INET, SOCK_STREAM, 0 );

    // Controllo se la creazione della socket è andata a buon fine
    if ( sockfd == -1 ) {
        printf( "\nErrore durante la creazione della socket client\n" );
        exit( EXIT_FAILURE );
    }

    // Impostazione degli indirizzi del server
    address.sin_family = AF_INET; // Dominio
    inet_pton( AF_INET, LOCAL_HOST, &address.sin_addr ); // Text to binary
    address.sin_port = htons( CENTER_PORT ); // Host to network

    // Stabilisco una connessione con il server
    if ( connect( sockfd, ( struct sockaddr* ) &address, sizeof( address ) ) < 0 ) {
        perror( "\nErrore durante la connessione al server" );
        close( sockfd );
        exit( EXIT_FAILURE );
    }

    // Invio il codice attraverso la send
    if ( send( sockfd, &code, sizeof( code ), 0 ) < 0 ) {
        perror( "\nErrore durante l'invio del codice al centro vaccinale" );
        close( sockfd );
        exit( EXIT_FAILURE );
    }

    // Risposta da parte del centro vaccinale utilizzando recv
    if ( recv( sockfd, &response, sizeof( response ), 0 ) < 0 ) {
        perror( "\nErrore durante la ricezione della risposta dal centro vaccinale" );
        close( sockfd );
        exit( EXIT_FAILURE );
    }

    // Controllo validità green pass
    if ( response )
        printf( "\nIl Green Pass risulta essere VALIDO!\n" );
    else
        printf( "\nIl Green Pass risulta essere NON VALIDO!\n" );

    // Chiusura descrittore
    close( sockfd );

    printf( "\n" );
    return 0;
}