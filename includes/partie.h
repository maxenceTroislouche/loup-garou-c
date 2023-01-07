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
    int id_bal;
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

typedef struct reponse_serveur
{
    long mtype;
    char mtext[3];
} reponse_serveur_t;

typedef struct msg_vote
{
    pid_t votant;
    pid_t vote;
} msg_vote_t;

typedef struct vote
{
    long mtype;
    msg_vote_t mtext;
} vote_t;

typedef struct gestion_vote
{
    pid_t votant[MAX_CLIENTS];
    pid_t vote[MAX_CLIENTS];
    int nb_votes;
} gestion_vote_t;

/* ------------------------------ */
/*    Déclarations de fonctions   */
/* ------------------------------ */
partie_t creer_partie(int id_bal);
int afficher_partie(partie_t *partie);
int commencer_partie(partie_t *partie);
int envoyer_infos_partie_joueurs(int id_bal, partie_t *partie);
partie_t lire_infos_partie_joueurs(int id_bal);
int envoyer_vote_villageois(int id_bal, partie_t *partie);
int lire_vote_villageois(int id_bal, partie_t *partie, gestion_vote_t *gestion_vote_villageois);
pid_t resultat_vote(gestion_vote_t *gestion_vote);
int afficher_gestion_vote(gestion_vote_t *gestion_vote_villageois);
int nb_joueurs_eligible_vote_villageois(partie_t *p);
int nb_joueurs_eligible_vote_lg(partie_t *p);
int envoyer_vote_lg(partie_t *partie);
int lire_vote_lg(partie_t *partie, gestion_vote_t *gestion_vote_lg);


#endif