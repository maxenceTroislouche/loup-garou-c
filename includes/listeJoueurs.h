#ifndef LISTE_JOUEURS
#define LISTE_JOUEURS

#include <joueur.h>

typedef struct ListeJoueur
{
    Joueur_t joueurs[MAX_JOUEURS];
    unsigned int nbJoueurs;
} ListeJoueur_t;

ListeJoueur_t *creerListeJoueurs();
bool ajouterJoueurDansListeJoueurs(ListeJoueur_t *l, Joueur_t *j);
void printListeJoueurs(ListeJoueur_t *l);

#endif