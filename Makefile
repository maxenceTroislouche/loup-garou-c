CC=gcc
ARGS=-Wall -Wextra -pthread
INCLUDE=-I /home/maxence/dev/programmation_systeme/loup/includes
all: objects/role.o objects/rolesDisponibles.o objects/joueur.o objects/listeJoueurs.o client.exe

objects/role.o: sources/role.c
	$(CC) sources/role.c -c $(ARGS) $(INCLUDE) -o objects/role.o

objects/rolesDisponibles.o: sources/rolesDisponibles.c
	$(CC) sources/rolesDisponibles.c -c $(ARGS) $(INCLUDE) -o objects/rolesDisponibles.o

objects/joueur.o: sources/joueur.c
	$(CC) sources/joueur.c -c $(ARGS) $(INCLUDE) -o objects/joueur.o


objects/listeJoueurs.o: sources/listeJoueurs.c
	$(CC) sources/listeJoueurs.c -c $(ARGS) $(INCLUDE) -o objects/listeJoueurs.o

client.exe: client.c
	$(CC) client.c objects/role.o objects/joueur.o objects/listeJoueurs.o objects/rolesDisponibles.o -o executables/client.exe $(INCLUDE) $(ARGS)

clean: 
	rm objects/* executables/*