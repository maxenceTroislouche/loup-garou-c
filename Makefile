CC=gcc
OPTS=-Wall -Wextra -lpthread -I./includes -lpthread

all: executables/serveur.exe executables/client.exe

objects/connexion.o: sources/connexion.c
	$(CC) $^ -c -o $@ $(OPTS)

objects/bal.o: sources/bal.c
	$(CC) $^ -c -o $@ $(OPTS)

objects/joueur.o: sources/joueur.c
	$(CC) $^ -c -o $@ $(OPTS)

objects/partie.o: sources/partie.c
	$(CC) $^ -c -o $@ $(OPTS)

executables/serveur.exe: sources/serveur.c objects/bal.o objects/connexion.o objects/joueur.o objects/partie.o
	$(CC) $^ -o $@ $(OPTS)

executables/client.exe: sources/client.c objects/bal.o objects/connexion.o objects/joueur.o objects/partie.o
	$(CC) $^ -o $@ $(OPTS)

clean:
	rm -rf executables/* objects/*