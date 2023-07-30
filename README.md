# GreenPass

Questo progetto è stato sviluppato come parte del corso di Reti di Calcolatori presso l'Università di Napoli Parthenope. Il progetto consiste in un'applicazione client/server sviluppata in linguaggio C utilizzando il protocollo di comunicazione TCP e un modello di I/O multiplex per il server.

## Descrizione

Il progetto GreenPass è un'applicazione di gestione di Green Pass, dove un utente può generare nuovi Green Pass, verificare la validità di un Green Pass e invalidare o ripristinare la validità di un Green Pass in seguito a un contagio o una guarigione.

Il sistema è composto da diverse componenti:

- `client`: è l'applicazione client che consente agli utenti di interagire con il centro vaccinale per richiedere servizi relativi ai Green Pass.

- `clientS`: è un'ulteriore applicazione client che simula la verifica della validità di un Green Pass inviando il codice di una tessera sanitaria al server.

- `clientT`: è un'ulteriore applicazione client che ha il ruolo di segnalare i casi di contagio o guarigione di una persona.

- `centro_vaccinale`: è il centro vaccinale, un'applicazione peer che invia al server i dati di un nuovo Green Pass da registrare, ricevuti dal client.

- `serverV`: è il server di validazione dei Green Pass. Gestisce le richieste di verifica dei client e tiene traccia dei Green Pass validi.

- `serverG`: è il server di gestione dei Green Pass. Si occupa di ricevere le segnalazioni di contagio o guarigione dai client e di invalidare o ripristinare la validità dei Green Pass di conseguenza.

## Come compilare

Assicurarsi di avere il compilatore GCC installato sul proprio sistema. Per compilare il progetto, eseguire i seguenti comandi:

1. Pulire i file oggetto e gli eseguibili generati:
```
make clean
```

2. Compilare tutto il progetto e generare gli eseguibili:
```
make all
```

## Utilizzo

Prima di eseguire qualsiasi applicazione client o server, assicurarsi che i server siano in esecuzione.

Per avviare il centro vaccinale, eseguire il seguente comando:
```
./centro_vaccinale
```

Per avviare il serverV, eseguire il seguente comando:
```
./serverV
```

Per avviare il serverG, eseguire il seguente comando:
```
./serverG
```

Per avviare il client, eseguire il seguente comando:
```
./client D**************P
```

Per avviare il clientS, eseguire il seguente comando:
```
./clientS D**************P
```

Per avviare il clientT, eseguire il seguente comando:
```
./clientT D**************P
```

## Licenza

Questo progetto è rilasciato con licenza MIT. Consultare il file LICENSE per ulteriori dettagli.