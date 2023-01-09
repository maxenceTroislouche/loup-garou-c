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
    Etape 4 : Fin de partie
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

typedef struct msg_vote_voyante
{
    pid_t pid_demandeur;
    pid_t pid_demande;
} msg_vote_voyante_t;

typedef struct vote_voyante
{
    long mtype;
    msg_vote_voyante_t mtext;
} vote_voyante_t;

typedef struct msg_reponse_serveur_voyante
{
    char status[3];
    role_t role;
} msg_reponse_serveur_voyante_t;

typedef struct reponse_serveur_voyante
{
    long mtype;
    msg_reponse_serveur_voyante_t mtext;
} reponse_serveur_voyante_t;

/* ------------------------------ */
/*    Déclarations de fonctions   */
/* ------------------------------ */
/**
 * @brief Crée une partie
 * 
 * @param id_bal id boite aux lettres
 * @return partie_t 
 */
partie_t creer_partie(int id_bal);

/**
 * @brief Affiche les informations d'une partie
 * 
 * @param partie 
 * @return int 
 */
int afficher_partie(partie_t *partie);

/**
 * @brief Commence une partie / Met l'étape à 1
 * 
 * @param partie 
 * @return int 
 */
int commencer_partie(partie_t *partie);

/**
 * @brief Envoie les informations de la partie à tous les joueurs
 * 
 * @param id_bal id boite aux lettres
 * @param partie partie
 * @return int 
 */
int envoyer_infos_partie_joueurs(int id_bal, partie_t *partie);

/**
 * @brief Lit les informations de la partie envoyés par le serveur
 * 
 * @param id_bal id boite aux lettre
 * @return partie_t 
 */
partie_t lire_infos_partie_joueurs(int id_bal);

/**
 * @brief Envoie le vote villageois
 * 
 * @param id_bal id boite aux lettres
 * @param partie 
 * @return int 
 */
int envoyer_vote_villageois(int id_bal, partie_t *partie);

/**
 * @brief Lit un vote villageois
 * 
 * @param id_bal id boite aux lettres
 * @param partie 
 * @param gestion_vote_villageois 
 * @return int 
 */
int lire_vote_villageois(int id_bal, partie_t *partie, gestion_vote_t *gestion_vote_villageois);

/**
 * @brief Retourne le pid du joueur perdant du vote
 * 
 * @param gestion_vote 
 * @return pid_t 
 */
pid_t resultat_vote(gestion_vote_t *gestion_vote);

/**
 * @brief Affiche les informations du gestionnaire de vote
 * 
 * @param gestion_vote_villageois 
 * @return int 
 */
int afficher_gestion_vote(gestion_vote_t *gestion_vote_villageois);

/**
 * @brief Retourne le nombre de joueurs pouvant voter lors du vote villageois dans une partie
 * 
 * @param p 
 * @return int 
 */
int nb_joueurs_eligible_vote_villageois(partie_t *p);

/**
 * @brief Retourne le nombre de joueurs pouvant voter lors du vote lg dans une partie
 * 
 * @param p 
 * @return int 
 */
int nb_joueurs_eligible_vote_lg(partie_t *p);

/**
 * @brief Envoie le vote Loup-Garou
 * 
 * @param partie 
 * @return int 
 */
int envoyer_vote_lg(partie_t *partie);

/**
 * @brief Lit un vote Loup-Garou
 * 
 * @param partie 
 * @param gestion_vote_lg 
 * @return int 
 */
int lire_vote_lg(partie_t *partie, gestion_vote_t *gestion_vote_lg);

/**
 * @brief Envoie le vote de la voyante
 * 
 * @param partie 
 * @return int 
 */
int envoyer_vote_voyante(partie_t *partie);

/**
 * @brief Lit le vote de la voyante
 * 
 * @param partie 
 * @return int 
 */
int lire_vote_voyante(partie_t *partie);

/**
 * @brief Affiche les roles restants dans la partie
 * 
 * @param partie 
 */
void afficher_roles_restants_partie(partie_t *partie);

/**
 * @brief Affiche le gagnant de la partie
 * 
 * @param partie 
 */
void afficher_res_partie(partie_t *partie);

/**
 * @brief Affiche le dernier joueur mort de la partie
 * 
 * @param partie 
 * @param liste_joueurs_morts 
 */
void afficher_dernier_joueur_mort(partie_t *partie, liste_joueurs_t *liste_joueurs_morts);
#endif