#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

#include <role.h>
#include <rolesDisponibles.h>
#include <joueur.h>
#include <listeJoueurs.h>

int main()
{
    // Seed pour l'aléatoire
    srand(time(NULL));
    initNomRoles();
    ListeRolesDisponibles_t *l = creerListeRolesDisponibles();
    printListeRolesDisponibles(l);
    Joueur_t *j1 = creerJoueur("J1", l);
    Joueur_t *j2 = creerJoueur("J2", l);
    Joueur_t *j3 = creerJoueur("J3", l);
    Joueur_t *j4 = creerJoueur("J4", l);
    Joueur_t *j5 = creerJoueur("J5", l);
    Joueur_t *j6 = creerJoueur("J6", l);
    Joueur_t *j7 = creerJoueur("J7", l);
    Joueur_t *j8 = creerJoueur("J8", l);
    Joueur_t *j9 = creerJoueur("J9", l);
    Joueur_t *j10 = creerJoueur("J10", l);
    ListeJoueur_t *liste_joueurs = creerListeJoueurs();
    ajouterJoueurDansListeJoueurs(liste_joueurs, j1);
    ajouterJoueurDansListeJoueurs(liste_joueurs, j2);
    ajouterJoueurDansListeJoueurs(liste_joueurs, j3);
    ajouterJoueurDansListeJoueurs(liste_joueurs, j4);
    ajouterJoueurDansListeJoueurs(liste_joueurs, j5);
    ajouterJoueurDansListeJoueurs(liste_joueurs, j6);
    ajouterJoueurDansListeJoueurs(liste_joueurs, j7);
    ajouterJoueurDansListeJoueurs(liste_joueurs, j8);
    ajouterJoueurDansListeJoueurs(liste_joueurs, j9);
    ajouterJoueurDansListeJoueurs(liste_joueurs, j10);
    printListeJoueurs(liste_joueurs);
    printListeRolesDisponibles(l);

    return 0;
}