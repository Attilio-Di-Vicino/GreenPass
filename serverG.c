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

    // Assegnazione indirizzo locale alla socket
    Bind( sockfd, ( struct sockaddr* ) &servaddr, sizeof( servaddr ) );

    // Mette in ascolto un socket TCP per le connessioni in entrata
    Listen( sockfd, SOMAXCONN );

    // FD_SET
    fd_set readSet;
    FD_ZERO( &readSet );
    int i, fd_connection, fd_open[ FD_SETSIZE ] = {0};
    int fd_ready = 0;
    ssize_t nread = 0;
    int maxfd = 0;

    fd_open[ sockfd ]++;
    maxfd = sockfd;

    printf( "\n---- SERVER G IN ASCOLTO ----\n" );

    while ( run ) {

        for ( i = sockfd; i <= maxfd; i++ )
            if ( fd_open[i] != 0 )
                FD_SET( i, &readSet );

        // Monitoro i file descriptor ed n è il numero di fd pronti
        fd_ready = Select( maxfd + 1, &readSet, NULL, NULL, NULL );

        if ( FD_ISSET( sockfd, &readSet ) ) {
            fd_ready--;
            // Accettazione della connessione da un client
            fd_connection = Accept( sockfd, ( struct sockaddr* ) &clientaddr, &clientaddrLength );
            printf( "\nConnessione stabilita con %d...", fd_connection );
            fd_open[ fd_connection ] = 1;
            if ( maxfd < fd_connection )
                maxfd = fd_connection;
        }

        i = sockfd;

        while ( fd_ready != 0 ) {
            
            i++;
            
            // Se è zero, vai al prossimo
            if ( fd_open[i] == 0 )
                continue;

            if ( FD_ISSET( i, &readSet ) ) {
                fd_ready--;
                // Ricevo il codice dal client
                nread = FullRead( i, &todoHandle, sizeof( todoHandle ) );
                if ( nread < 0 )
                    perror( "\nErrore nella lettura" );
                if ( nread == 0 ) {
                    fd_open[i] = 0;

                    // Se ho raggiunto il maxfd devo trovare il nuovo
                    if ( maxfd == i ) {
                        while ( fd_open[ --i ] == 0 );
                        maxfd = i;
                        break;
                    }
                }
            }

            // Viene creato un green pass temporaneo che è di comunicazione 
            // con il serverV per cui non sarà inserito nell'insieme dei green pass
            GreenPass greenPass = createGreenPass( todoHandle.code );

            // Imposto l'operazione da faress
            if ( strcmp( todoHandle.todo, INVALIDATE ) == 0 ) {
                // Invalidazione
                strncpy( greenPass.toCheck, INVALIDATE, TODO_SIZE );
                printf( "Cambio di validità FALSE..." );
            } else if ( strcmp( todoHandle.todo, RESTORE ) == 0 ) {
                // Restore
                strncpy( greenPass.toCheck, RESTORE, TODO_SIZE );
                printf( "Cambio di validità TRUE..." );
            } else {
                // Is valid
                strncpy( greenPass.toCheck, ISVALID, TODO_SIZE );
                printf( "Verifica in corso..." );
            }
            greenPass.toCheck[ TODO_SIZE ] = '\0';

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

            // Invio il codice attraverso la FullWrite
            FullWrite( sockfdSV, &greenPass, sizeof( greenPass ) );
            response = 0;
            // Risposta da parte del centro vaccinale utilizzando FullRead
            FullRead( sockfdSV, &response, sizeof( response ) );
            close( sockfdSV ); // Chiudo fd di server V
            printf( "\nOperazione conclusa." );
            printf( "\n----------- CONNESSIONE CHIUSA V ------------\n" );

            // Invio la risposta
            FullWrite( i, &response, sizeof( response ) );
        } 
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