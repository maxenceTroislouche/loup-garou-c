#ifndef CONNEXION
#define CONNEXION

/* ------------------------------ */
/*            Includes            */
/* ------------------------------ */
#include <bal.h>

/* ------------------------------ */
/*           Constantes           */
/* ------------------------------ */
#define MAX_CLIENTS 5
#define MAX_NOM_CLIENT 50

/* ------------------------------ */
/*           Structures           */
/* ------------------------------ */
typedef struct client
{
    pid_t pid;
    char nom[MAX_NOM_CLIENT];
} client_t;

typedef struct liste_clients
{
    client_t clients[MAX_CLIENTS];
    unsigned int nb_clients;
    pthread_mutex_t mutex_acces;
} liste_clients_t;

typedef struct msg_demande_connexion
{
    pid_t pid;
    char nom[MAX_NOM_CLIENT];
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
liste_clients_t init_liste_clients();
client_t * creer_client(pid_t pid, char *nom);
client_t init_client(pid_t pid, char *nom);
int index_client(liste_clients_t *l, pid_t client);
int ajouter_client(liste_clients_t *l, client_t client);
int retirer_client(liste_clients_t *l, client_t client);
int afficher_liste_clients(liste_clients_t *l);

int lire_demande_connexion(int id_bal, liste_clients_t *liste_clients);
int envoyer_demande_connexion(int id_bal, char *nom);

#endif