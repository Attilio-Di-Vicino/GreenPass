/**
 * Un utente, una volta effettuata la vaccinazione,
 * tramite un client si collega ad un centro vaccinale e
 * comunica il codice della propria tessera sanitaria.
 * Il centro vaccinale comunica al ServerV il codice ricevuto
 * dal client ed il periodo di validità del green pass.
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "service/addresses.h"
#include "service/functions.h"
#include <signal.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>

#define CODE_LENGTH 16

// e operazioni su questa variabile sono eseguite in modo atomico e
// non vengono ottimizzate dal compilatore
volatile sig_atomic_t run = 1;

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
    // Codice tessera sanitaria
    char code[ CODE_LENGTH ];
    // Risposta dal centro vaccinale
    int response;

    // Creazione socket utilizzata per comunicare con il centro vaccinale
    // Per la comunicazione viene utilizzato un dominio AF_INET e protocollo TCP
    sockfd = Socket( AF_INET, SOCK_STREAM, 0 );

    // Impostazione degli indirizzi del server
    servaddr.sin_family = AF_INET; // Dominio
    inet_pton( AF_INET, LOCAL_HOST, &servaddr.sin_addr ); // Text to binary
    servaddr.sin_port = htons( CENTER_PORT ); // Host to network

    // Assegnazione indirizzo locale alla socket
    Bind( sockfd, ( struct sockaddr* ) &servaddr, sizeof( servaddr ) );

    // Mette in ascolto un socket TCP per le connessioni in entrata
    Listen( sockfd, SOMAXCONN );

    while ( run ) {

        // Accettazione della connessione da un client
        clientaddrLength = sizeof( clientaddr );
        connfd = Accept( sockfd, ( struct sockaddr* ) &clientaddr, &clientaddrLength );
        
        // Ricevo il codice dal client
        ssize_t nleftR = FullRead( connfd, &code, sizeof( code ) );

        response = 1;
        printf( "\nRichiesta client gestita risultato: %d\n", response );

        // Invio la risposta
        ssize_t nleftW = FullWrite( connfd, &response, sizeof( response ) );

        // Chiude il socket del client dopo aver finito di comunicare con esso
        close( connfd );
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