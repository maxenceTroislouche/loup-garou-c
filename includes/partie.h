#ifndef PARTIE
#define PARTIE

/* ------------------------------ */
/*            Includes            */
/* ------------------------------ */
#include <joueur.h>

/* ------------------------------ */
/*           Constantes           */
/* ------------------------------ */

/* ------------------------------ */
/*           Structures           */
/* ------------------------------ */

/*
    Etape 0 : Attente des joueurs
    Etape 1 : Le village s'endort / La voyante se réveille
    Etape 2 : Les LG se réveillent
    Etape 3 : Vote des villageois
*/
typedef struct partie
{
    liste_joueurs_t liste_joueurs;
    unsigned int etape;
} partie_t;

/* ------------------------------ */
/*    Déclarations de fonctions   */
/* ------------------------------ */
partie_t creer_partie();
int afficher_partie(partie_t *partie);

#endif