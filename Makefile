# compilatore utilizzato
CC = gcc
# abilita tutti i warning del compilatore
CFLAGS = -g -Wall

# lista delle cartelle contenenti i file header
INCLUDES = -Iservice

# lista dei file oggetto necessari per l'eseguibile centro_vaccinale
CENTRO_OBJ = centro_vaccinale.o
CENTRO_DEPS = service/addresses.h service/functions.h service/green_pass.h

# lista dei file oggetto necessari per l'eseguibile client
CLIENT_OBJ = client.o
CLIENT_DEPS = service/addresses.h service/functions.h service/green_pass.h

# lista dei file oggetto necessari per l'eseguibile clientS
CLIENTS_OBJ = clientS.o
CLIENTS_DEPS = service/addresses.h service/functions.h service/green_pass.h service/todo.h

# lista dei file oggetto necessari per l'eseguibile clientT
CLIENTT_OBJ = clientT.o
CLIENTT_DEPS = service/addresses.h service/functions.h service/green_pass.h service/todo.h

# lista dei file oggetto necessari per l'eseguibile serverG
SERVERG_OBJ = serverG.o
SERVERG_DEPS = service/addresses.h service/functions.h service/green_pass.h service/todo.h

# lista dei file oggetto necessari per l'eseguibile serverV
SERVERV_OBJ = serverV.o
SERVERV_DEPS = service/addresses.h service/functions.h service/green_pass.h

all : clean client clientT clientS centro_vaccinale serverV serverG

client : $(CLIENT_OBJ)
	$(CC) $(CFLAGS) $(INCLUDES) -o client $(CLIENT_OBJ)

clientT : $(CLIENTT_OBJ)
	$(CC) $(CFLAGS) $(INCLUDES) -o clientT $(CLIENTT_OBJ)

clientS : $(CLIENTS_OBJ)
	$(CC) $(CFLAGS) $(INCLUDES) -o clientS $(CLIENTS_OBJ)

centro_vaccinale : $(CENTRO_OBJ)
	$(CC) $(CFLAGS) $(INCLUDES) -o centro_vaccinale $(CENTRO_OBJ)

serverV : $(SERVERV_OBJ)
	$(CC) $(CFLAGS) $(INCLUDES) -o serverV $(SERVERV_OBJ)

serverG : $(SERVERG_OBJ)
	$(CC) $(CFLAGS) $(INCLUDES) -o serverG $(SERVERG_OBJ)

%.o: %.c $(CENTRO_DEPS) $(CLIENT_DEPS) $(CLIENTS_DEPS) $(CLIENTT_DEPS) $(SERVERG_DEPS) $(SERVERV_DEPS)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean :
	rm -f *.o all
	rm -f clientT
	rm -f clientS
	rm -f client
	rm -f centro_vaccinale
	rm -f serverG
	rm -f serverV