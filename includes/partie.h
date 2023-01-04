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
    Etape 1 : Le village s'endort : La voyante se réveille
    Etape 2 : Les LG se réveillent : Vote LG
    Etape 3 : Le village se réveille : Vote des villageois
*/
typedef struct partie
{
    liste_joueurs_t liste_joueurs;
    unsigned int etape;
} partie_t;

typedef struct msg_infos_partie
{
    partie_t partie;
} msg_infos_partie_t;

typedef struct infos_partie
{
    long mtype;
    msg_infos_partie_t mtext;
} infos_partie_t;

/* ------------------------------ */
/*    Déclarations de fonctions   */
/* ------------------------------ */
partie_t creer_partie();
int afficher_partie(partie_t *partie);
int commencer_partie(partie_t *partie);
int envoyer_infos_partie_joueurs(int id_bal, partie_t *partie);
partie_t lire_infos_partie_joueurs(int id_bal);

#endif