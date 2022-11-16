#ifndef JOUEUR
#define JOUEUR

#include <rolesDisponibles.h>

#define MAX_JOUEURS 10
#define MAX_NOM_JOUEUR 50

typedef struct Joueur
{
    char nom[MAX_NOM_JOUEUR];
    role_t role;
    bool vivant;
} Joueur_t;

Joueur_t *creerJoueur(char *nom, ListeRolesDisponibles_t *l);
void printJoueur(Joueur_t *j);



#endif