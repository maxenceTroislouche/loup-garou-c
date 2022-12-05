#ifndef CONNEXION
#define CONNEXION

/* ------------------------------ */
/*            Includes            */
/* ------------------------------ */
#include <bal.h>

/* ------------------------------ */
/*           Constantes           */
/* ------------------------------ */
#define MAX_CLIENTS 10

/* ------------------------------ */
/*           Structures           */
/* ------------------------------ */
typedef struct liste_clients
{
    pid_t clients[MAX_CLIENTS];
    unsigned int nb_clients; 
} liste_clients_t;

typedef struct msg_demande_connexion
{
    pid_t pid;
    char message[3];
} msg_demande_connexion_t;

typedef struct demande_connexion
{
    long mtype;
    msg_demande_connexion_t mtext;
} demande_connexion_t;

/* ------------------------------ */
/*    Déclarations de fonctions   */
/* ------------------------------ */

liste_clients_t *creer_liste_clients();
int index_client(liste_clients_t *l, pid_t client);
int ajouter_client(liste_clients_t *l, pid_t client);
int retirer_client(liste_clients_t *l, pid_t client);
int afficher_liste_clients(liste_clients_t *l);

int lire_demande_connexion(int id_bal, liste_clients_t *liste_clients);
int envoyer_demande_connexion(int id_bal);

#endif