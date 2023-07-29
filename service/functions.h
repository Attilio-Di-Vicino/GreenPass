/**
 * In questo file header ci sono varie funzioni utili al progetto
 * Tra cui anche le funzioni wrapper
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <time.h>
#include <errno.h>
#include <sys/select.h>
#include "green_pass.h"

/**
 * Funzione wrapper per la creazione di una socket
 * 
 * @param domain: Dominio della socket (AF_INET per IPv4, AF_INET6 per IPv6, ...)
 * @param type: Tipo della socket (SOCK_STREAM per TCP, SOCK_DGRAM per UDP, ...)
 * @param protocol: Protocollo utilizzato (0 per il protocollo di default associato al tipo)
 * @return sockfd: Descrittore della socket creata
 */
int Socket( int domain, int type, int protocol ) {
    int sockfd;
    if ( ( sockfd = socket( domain, type, protocol ) ) < 0 ) {
        perror( "\nErrore nella creazione della socket" );
        exit( EXIT_FAILURE );
    }
    return sockfd;
}

/**
 * Funzione wrapper per la connessione a un indirizzo di rete da parte del client
 * 
 * @param sockfd: Descrittore della socket
 * @param addr: Puntatore a una struttura sockaddr contenente l'indirizzo al quale connettersi
 * @param addrlen: Dimensione della struttura sockaddr
 */
void Connect( int sockfd, const struct sockaddr *addr, socklen_t addrlen ) {
    if ( ( connect( sockfd, addr, addrlen ) ) < 0 ) {
        fprintf( stderr, "connect error\n" );
        exit( EXIT_FAILURE );
    }
}

/**
 * Funzione wrapper per l'associazione di una socket a un indirizzo di rete
 * 
 * @param sockfd: Descrittore della socket
 * @param addr: Puntatore a una struttura sockaddr contenente l'indirizzo al quale associare la socket
 * @param addrlen: Dimensione della struttura sockaddr
 */
void Bind( int sockfd, const struct sockaddr *addr, socklen_t addrlen ) {
    if ( ( bind( sockfd, addr, addrlen ) ) < 0 ) {
        perror( "\nErrore nella bind della socket" );
        exit( EXIT_FAILURE );
    }
}

/**
 * Funzione wrapper per mettere in ascolto una socket per nuove connessioni
 * 
 * @param sockfd: Descrittore della socket in ascolto
 * @param backlog: Numero massimo di connessioni pendenti nella coda delle connessioni in arrivo
 */
void Listen( int sockfd, int backlog ) {
    if ( ( listen( sockfd, backlog ) ) < 0 ) {
        perror( "\nErrore nella listen della socket" );
        exit( EXIT_FAILURE );
    }
}

/**
 * Funzione wrapper per accettare una nuova connessione da una socket in ascolto
 * 
 * @param sockfd: Descrittore della socket in ascolto
 * @param addr: Puntatore a una struttura sockaddr nella quale sarà memorizzato l'indirizzo del client
 * @param addrlen: Puntatore a una variabile nella quale sarà memorizzata la dimensione della struttura sockaddr
 * @return client_sockfd: Descrittore della nuova socket creata per la connessione accettata
 */
int Accept( int sockfd, struct sockaddr *addr, socklen_t *addrlen ) {
    int client_sockfd;
    if ( ( client_sockfd = accept( sockfd, addr, addrlen ) ) < 0 ) {
        perror( "\nErrore nella accept della socket" );
        exit( EXIT_FAILURE );
    }
    return client_sockfd;
}

/**
 * Funzione wrapper per la chiamata di sistema select().
 * 
 * @param nfds: Numero massimo dei file descriptor da controllare + 1.
 * @param readfds: Puntatore a un insieme di socket in lettura da monitorare.
 * @param writefds: Puntatore a un insieme di socket in scrittura da monitorare.
 * @param exceptfds: Puntatore a un insieme di socket in errore da monitorare.
 * @param timeout: Puntatore a una struttura timeval che rappresenta il tempo massimo di attesa per l'evento.
 * @return nready: Numero di file descriptor pronti per la lettura, scrittura o in errore. -1 in caso di errore.
 */
int Select( int nfds, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, struct timeval *timeout ) {
    int nready;
    if ( ( nready = select( nfds, readfds, writefds, exceptfds, timeout ) ) < 0 ) {
        perror( "\nErrore nella select" );
        exit( EXIT_FAILURE );
    }
    return nready;
}

/**
 * Funzione per scrivere l'intero contenuto di un buffer in un file descriptor
 * 
 * @param fd: Descrittore del file descriptor nel quale scrivere i dati
 * @param buf: Puntatore al buffer contenente i dati da scrivere
 * @param count: Numero di byte da scrivere nel file descriptor
 * @return nleft: Numero di byte rimanenti da scrivere nel file descriptor
 */
ssize_t FullWrite( int fd, const void *buf, size_t count ) {
	size_t nleft;
	ssize_t nwritten;
	nleft = count;
	while ( nleft > 0 ) {

        if ( ( nwritten = write( fd, buf, nleft ) ) < 0 ) {
            if ( errno == EINTR )
                continue;
            else
                exit( nwritten );
        }
        nleft -= nwritten;
        buf += nwritten;
	}
	return nleft;
}

/**
 * Funzione per leggere l'intero contenuto di un file descriptor in un buffer
 * 
 * @param fd: Descrittore del file descriptor da cui leggere i dati
 * @param buf: Puntatore al buffer in cui memorizzare i dati letti
 * @param count: Numero di byte da leggere dal file descriptor
 * @return nleft: Numero di byte rimanenti da leggere dal file descriptor
 */
ssize_t FullRead( int fd, void *buf, size_t count ) {
    size_t nleft;
    ssize_t nread;
    nleft = count;
    while( nleft > 0 ) {
        
        nread = read( fd, buf, nleft );
        
        if ( errno == EINTR )
            continue;
        else
            return nread;
        
        if ( nread == 0 )
            break;
        
        nleft -= nread;
        buf += nread;
    }
    buf = 0;
    return nleft;
}