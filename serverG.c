/**
 * Un ClientS, per verificare se un green pass è valido,
 * invia il codice di una tessera sanitaria al ServerG il
 * quale richiede al ServerV il controllo della validità.
 * 
 * Un ClientT, inoltre, può invalidare o ripristinare la validità
 * di un green pass comunicando al ServerG il contagio o la guarigione
 * di una persona attraverso il codice della tessera sanitaria.
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "service/addresses.h"
#include "service/functions.h"
#include "service/green_pass.h"
#include "service/todo.h"
#include <signal.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/select.h>

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
    socklen_t clientaddrLength = sizeof( clientaddr );
    // Messaggio ricevuto
    todo todoHandle;
    // Risposta dal centro vaccinale
    int response;

    // Descrittore del file socket server V
    int sockfdSV;
    // Struttura degli indirizzi server V
    struct sockaddr_in addressSV;

    // Creazione socket utilizzata per comunicare con il centro vaccinale
    // Per la comunicazione viene utilizzato un dominio AF_INET e protocollo TCP
    sockfd = Socket( AF_INET, SOCK_STREAM, 0 );

    // Impostazione degli indirizzi del server
    servaddr.sin_family = AF_INET; // Dominio
    inet_pton( AF_INET, LOCAL_HOST, &servaddr.sin_addr ); // Text to binary
    servaddr.sin_port = htons( SERVERG_PORT ); // Host to network

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

    printf( "\n---- SERVER G IN ASCOLTO ----\n" );

    while ( run ) {

        // Accettazione della connessione da un client
        connfd = Accept( sockfd, ( struct sockaddr* ) &clientaddr, &clientaddrLength );

        // Ricevo il newGreenPass dal centro vaccinale
        ssize_t nread = FullRead( connfd, &todoHandle, sizeof( todoHandle ) );
        GreenPass greenPass = createGreenPass( todoHandle.code );

        // Connesione con il serverV
        printf( "\n----------- CONNESSIONE SERVER V ------------\n" );
        // Creazione socket utilizzata per comunicare con il centro vaccinale
        // Per la comunicazione viene utilizzato un dominio AF_INET e protocollo TCP
        sockfdSV = Socket( AF_INET, SOCK_STREAM, 0 );

        // Impostazione degli indirizzi del server
        addressSV.sin_family = AF_INET; // Dominio
        inet_pton( AF_INET, LOCAL_HOST, &addressSV.sin_addr ); // Text to binary
        addressSV.sin_port = htons( SERVERV_PORT ); // Host to network

        // Stabilisco una connessione con il server
        Connect( sockfdSV, ( struct sockaddr* ) &addressSV, sizeof( addressSV ) );

        if ( strcmp( todoHandle.todo, INVALIDATE ) == 0 ) {
            // Invalidazione
            strncpy( greenPass.toCheck, INVALIDATE, 3 );
            printf( "Cambio di validità FALSE..." );
        } else if ( strcmp( todoHandle.todo, RESTORE ) == 0 ) {
            // Restore
            strncpy( greenPass.toCheck, RESTORE, 3 );
            printf( "Cambio di validità TRUE..." );
        } else {
            // Is valid
            strncpy( greenPass.toCheck, ISVALID, 3 );
            printf( "Verifica in corso..." );
        }

        // Invio il codice attraverso la FullWrite
        ssize_t nleftW = FullWrite( sockfdSV, &greenPass, sizeof( greenPass ) );
        response = 0;
        // Risposta da parte del centro vaccinale utilizzando FullRead
        ssize_t nleftR = FullRead( sockfdSV, &response, sizeof( response ) );
        close( sockfdSV );
        printf( "\nOperazione conclusa." );
        printf( "\n----------- CONNESSIONE CHIUSA V ------------\n" );

        // Invio la risposta
        nleftW = FullWrite( connfd, &response, sizeof( response ) );
        close( connfd );   
    }

    // Chiude il socket del server prima di uscire
    close( sockfd );
    printf( "\n" );
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