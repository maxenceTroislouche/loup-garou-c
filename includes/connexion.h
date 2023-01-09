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
/**
 * @brief Crée de manière dynamique une liste de clients 
 * 
 * @return liste_clients_t* pointeur vers la liste de clients créée
 */
liste_clients_t *creer_liste_clients();

/**
 * @brief Crée de manière statique une liste de clients
 * 
 * @return liste_clients_t 
 */
liste_clients_t init_liste_clients();

/**
 * @brief Crée de manière dynamique un client
 * 
 * @param pid pid du client
 * @param nom nom d'utilisateur du client
 * @return client_t* 
 */
client_t * creer_client(pid_t pid, char *nom);

/**
 * @brief Crée de manière statique un client
 * 
 * @param pid pid du client
 * @param nom nom d'utilisateur du client
 * @return client_t 
 */
client_t init_client(pid_t pid, char *nom);

/**
 * @brief Retourne l'index du client dans la liste
 * 
 * @param l liste de clients
 * @param client client à rechercher
 * @return int index du client dans la liste, -1 si le client n'est pas dans la liste 
 */
int index_client(liste_clients_t *l, pid_t client);

/**
 * @brief Ajoute un client dans la liste
 * 
 * @param l liste de client
 * @param client 
 * @return int 0 ok
 */
int ajouter_client(liste_clients_t *l, client_t client);

/**
 * @brief Retire un client d'une liste de clients
 * 
 * @param l liste de clients
 * @param client 
 * @return int 
 */
int retirer_client(liste_clients_t *l, client_t client);

/**
 * @brief Affiche une liste de clients
 * 
 * @param l liste de clients
 * @return int 
 */
int afficher_liste_clients(liste_clients_t *l);

/**
 * @brief Lit une demande de connexion
 * 
 * @param id_bal id boite aux lettres
 * @param liste_clients liste de clients
 * @return int 0 ok
 */
int lire_demande_connexion(int id_bal, liste_clients_t *liste_clients);

/**
 * @brief Envoit une demande de connexion
 * 
 * @param id_bal id boite aux lettres
 * @param nom nom du client
 * @return int 0 ok
 */
int envoyer_demande_connexion(int id_bal, char *nom);

#endif