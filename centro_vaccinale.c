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
#include "addresses.h"
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
    sockfd = socket( AF_INET, SOCK_STREAM, 0 );

    // Controllo se la creazione della socket è andata a buon fine
    if ( sockfd == -1 ) {
        printf( "\nErrore durante la creazione della socket centro vaccinale\n" );
        exit( EXIT_FAILURE );
    }

    // Impostazione degli indirizzi del server
    servaddr.sin_family = AF_INET; // Dominio
    inet_pton( AF_INET, LOCAL_HOST, &servaddr.sin_addr ); // Text to binary
    servaddr.sin_port = htons( CENTER_PORT ); // Host to network

    // Assegnazione indirizzo locale alla socket
    if ( bind( sockfd, ( struct sockaddr* ) &servaddr, sizeof( servaddr ) ) == -1 ) {
        perror( "Errore binding del socket del centro vaccinale" );
        close( sockfd );
        exit( EXIT_FAILURE );
    }

    // Mette in ascolto un socket TCP per le connessioni in entrata
    if ( listen( sockfd, SOMAXCONN ) == -1 ) {
        perror( "Errore listening del centro vaccinale" );
        close( sockfd );
        exit( EXIT_FAILURE );
    }

    while ( run ) {

        // Accettazione della connessione da un client
        clientaddrLength = sizeof( clientaddr );
        connfd = accept( sockfd, ( struct sockaddr* ) &clientaddr, &clientaddrLength );

        // Controllo run
        if ( !run ) {
            perror( "\nTerminazione centro vaccinale con successo" );
            exit( EXIT_SUCCESS );
        }

        if ( connfd < 0 ) {
            perror( "Errore nell'accettare la connessione" );
            close( sockfd );
            exit( EXIT_FAILURE );
        }

        // Gestione della connessione con il client
        // Ricevo il codice dal client
        if ( recv( connfd, &code, sizeof( code ), 0 ) < 0 ) {
            perror( "\nErrore durante la ricezione della risposta dal centro vaccinale" );
            close( sockfd );
            close( connfd );
            exit( EXIT_FAILURE );
        }

        response = 1;
        printf( "\nRichiesta client gestita risultato: %d\n", response );

        // Invio la risposta
        if ( send( connfd, &response, sizeof( response ), 0) < 0 ) {
            perror( "\nErrore durante l'invio del codice al centro vaccinale" );
            close( sockfd );
            close( connfd );
            exit( EXIT_FAILURE );
        }

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