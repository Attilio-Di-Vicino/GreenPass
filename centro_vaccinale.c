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

#define CODE_LENGTH 16

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
    int socketDescriptor;
    int clientDescriptor;
    // Struttura degli indirizzi
    struct sockaddr_in address;
    struct sockaddr_in clientAddress;
    socklen_t clientAddressLength;
    // Codice tessera sanitaria
    char code[ CODE_LENGTH ];
    // Risposta dal centro vaccinale
    int response;

    // Creazione socket utilizzata per comunicare con il centro vaccinale
    // Per la comunicazione viene utilizzato un dominio AF_INET e protocollo TCP
    socketDescriptor = socket( AF_INET, SOCK_STREAM, 0 );

    // Controllo se la creazione della socket è andata a buon fine
    if ( socketDescriptor == -1 ) {
        printf( "\nErrore durante la creazione della socket centro vaccinale\n" );
        exit( EXIT_FAILURE );
    }

    // Impostazione degli indirizzi del server
    address.sin_family = AF_INET; // Dominio
    address.sin_port = htons( CENTER_PORT ); // Host to network
    inet_pton( AF_INET, LOCAL_HOST, &address.sin_addr ); // Text to binary

    // 
    if ( bind( socketDescriptor, ( struct sockaddr* ) &address, sizeof( address ) ) == -1 ) {
        perror( "Errore binding del socket del centro vaccinale" );
        exit( EXIT_FAILURE );
    }

    //
    if ( listen( socketDescriptor, SOMAXCONN ) == -1 ) {
        perror( "Errore listening del centro vaccinale" );
        exit( EXIT_FAILURE );
    }

    while ( run ) {
        // Accettazione della connessione da un client
        clientAddressLength = sizeof( clientAddress );
        clientDescriptor = accept( socketDescriptor, 
                            ( struct sockaddr* ) &clientAddress, &clientAddressLength );
        if ( clientDescriptor < 0 ) {
            perror( "Errore nell'accettare la connessione" );
            close( socketDescriptor );
            exit( EXIT_FAILURE );
        }

        // Gestione della connessione con il client
        // Ricevo il codice dal client
        if ( recv( clientDescriptor, &code, sizeof( code ), 0 ) < 0 ) {
            perror( "\nErrore durante la ricezione della risposta dal centro vaccinale" );
            exit( EXIT_FAILURE );
        }

        response = 1;

        // Invio la risposta
        if ( send( clientDescriptor, &response, sizeof( response ), 0) < 0 ) {
            perror( "\nErrore durante l'invio del codice al centro vaccinale" );
            exit( EXIT_FAILURE );
        }

        // Chiude il socket del client dopo aver finito di comunicare con esso
        close( clientDescriptor );
    }

    // Chiude il socket del server prima di uscire
    close( socketDescriptor );
    return 0;
}

void handler( int signo ) {
    run = 0;
}