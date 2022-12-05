CC=gcc
OPTS=-Wall -Wextra -lpthread -I./includes -lpthread

all: executables/test_serveur.exe executables/test_client.exe

objects/connexion.o: sources/connexion.c
	$(CC) $^ -c -o $@ $(OPTS)

objects/bal.o: sources/bal.c
	$(CC) $^ -c -o $@ $(OPTS)

executables/test_serveur.exe: sources/test_serveur.c objects/bal.o objects/connexion.o
	$(CC) $^ -o $@ $(OPTS)

executables/test_client.exe: sources/test_client.c objects/bal.o objects/connexion.o
	$(CC) $^ -o $@ $(OPTS)

executables/serv.exe: sources/serveur.c
	$(CC) $^ -o $@ $(OPTS)

executables/cli.exe: sources/client.c
	$(CC) $^ -o $@ $(OPTS)

clean:
	rm -rf executables/* objects/*