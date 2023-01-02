#ifndef JOUEUR
#define JOUEUR

/* ------------------------------ */
/*            Includes            */
/* ------------------------------ */
#include <connexion.h>

/* ------------------------------ */
/*           Constantes           */
/* ------------------------------ */
#define MAX_NOM_ROLE 50

#define ROLE_VILLAGEOIS 0
#define ROLE_LG 1
#define ROLE_VOYANTE 2

/* ------------------------------ */
/*           Structures           */
/* ------------------------------ */
typedef struct role
{
    int num;
    char nom[MAX_NOM_ROLE];
} role_t;

typedef struct roles_disponibles
{
    role_t roles[MAX_CLIENTS];
    unsigned int nb_roles_disponibles;
} roles_disponibles_t;

typedef struct joueur
{
    client_t client;
    role_t role;
    char est_vivant;
} joueur_t;

typedef struct liste_joueurs
{
    joueur_t joueurs[MAX_CLIENTS];
    unsigned int nb_joueurs;
    pthread_mutex_t mutex_acces;
} liste_joueurs_t;

/* ------------------------------ */
/*    Déclarations de fonctions   */
/* ------------------------------ */
role_t creer_role(int num);
int afficher_role(role_t *role);
int est_role_null(role_t *role);
roles_disponibles_t * creer_liste_roles_disponibles();
roles_disponibles_t init_liste_roles_disponibles();
int afficher_liste_roles_disponibles(roles_disponibles_t *roles_disponibles);
role_t recuperer_role(roles_disponibles_t * roles_disponibles);
joueur_t * creer_joueur(client_t *client, roles_disponibles_t *roles_disponibles);
joueur_t init_joueur(client *client, roles_disponibles_t *roles_disponibles);
int afficher_joueur(joueur_t *joueur); 
liste_joueurs_t * creer_liste_joueurs();
liste_joueurs_t init_liste_joueurs();
int ajouter_joueur(liste_joueurs_t *liste_joueurs, joueur_t *joueur);
int index_joueur(liste_joueurs_t *liste_joueurs, joueur_t *joueur);
int retirer_joueur(liste_joueurs_t *liste_joueurs, joueur_t *joueur);
int ajouter_joueurs(liste_joueurs_t *liste_joueurs, liste_clients_t *liste_clients, roles_disponibles_t *roles_disponibles);
int afficher_liste_joueurs(liste_joueurs_t *liste_joueurs);
int nb_joueurs_vivants(liste_joueurs_t *liste_joueurs);

#endif