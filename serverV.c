/**
 * Il centro vaccinale comunica al ServerV il codice ricevuto
 * dal client ed il periodo di validità del green pass.
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "service/addresses.h"
#include "service/functions.h"
#include "service/green_pass.h"
#include <signal.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/select.h>

#define MAX_NUM_OF_GREEN_PASS 120

// e operazioni su questa variabile sono eseguite in modo atomico e
// non vengono ottimizzate dal compilatore
volatile sig_atomic_t run = 1;
// Impostata ad 1 perchè vengono utilizzati due clienti
int count = 1;

void handler( int );
int main() {

    // Inizialmente viene utilizzato sigaction
    // Per cattura il segnale e gestirlo 
    // Ed eventuali mascheramenti di segnali
    struct sigaction newAction;
    newAction.sa_handler = handler;
    sigemptyset( &newAction.sa_mask );
    newAction.sa_flags = 0;
    sigaction( SIGINT, &newAction, NULL );

    // Descrittore del file socket
    int sockfd;
    int connfd;
    // Struttura degli indirizzi
    struct sockaddr_in servaddr;
    struct sockaddr_in clientaddr;
    socklen_t clientaddrLength;
    // Nuovo GreenPass
    GreenPass newGreenPass;
    // Risposta dal centro vaccinale
    int response;

    // Green Pass presenti
    GreenPass allGreenPass[ MAX_NUM_OF_GREEN_PASS ];
    initAllGreenPass( allGreenPass );
    printAllGreenPass( allGreenPass, 2 );

    // Creazione socket utilizzata per comunicare con il centro vaccinale
    // Per la comunicazione viene utilizzato un dominio AF_INET e protocollo TCP
    sockfd = Socket( AF_INET, SOCK_STREAM, 0 );

    // Impostazione degli indirizzi del server
    servaddr.sin_family = AF_INET; // Dominio
    inet_pton( AF_INET, LOCAL_HOST, &servaddr.sin_addr ); // Text to binary
    servaddr.sin_port = htons( SERVERV_PORT ); // Host to network

    // Da valutare
    int optval = 1;
    if ( setsockopt( sockfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof( optval ) ) < 0) {
        perror( "setsockopt" );
        exit( EXIT_FAILURE );
    }

    // Assegnazione indirizzo locale alla socket
    Bind( sockfd, ( struct sockaddr* ) &servaddr, sizeof( servaddr ) );

    // Mette in ascolto un socket TCP per le connessioni in entrata
    Listen( sockfd, SOMAXCONN );

    printf( "\n---- SERVER V IN ASCOLTO ----\n" );

    while ( run ) {
        
        // Accettazione della connessione da un client
        connfd = Accept( sockfd, NULL, NULL );

        // Ricevo il newGreenPass dal centro vaccinale
        ssize_t nread = FullRead( connfd, &newGreenPass, sizeof( newGreenPass ) );

        // Green pass
        printf( "\nGreen Pass ricevuto con successo green pass:" );
        printf( "\nCodice Fiscale: %s", newGreenPass.code );
        printf( "\nData inizio: %s", ctime( &newGreenPass.valid_from ) );
        printf( "Data fine: %s", ctime( &newGreenPass.valid_until ) );
        response = 1; // Simulazione
        addGreenPass( allGreenPass, newGreenPass, count );
        printf( "Green Pass Totali: %d\n", ++count + 1 );

        // Invio la risposta
        ssize_t nleftW = FullWrite( connfd, &response, sizeof( response ) );
        
    }

    // Chiude il socket del server prima di uscire
    close( sockfd );
    return 0;
}

/**
 * Gestore del segnale SIGINT
 * in linea generale il gestore dovrebbe avere
 * poche istruzioni per evitare problemi di race condiction
 * run è di tipo volatile sig_atomic_t.
 * 
 * @param signo segnale
*/
void handler( int signo ) {
    run = 0;
}