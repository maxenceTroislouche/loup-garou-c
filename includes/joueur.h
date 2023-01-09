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
    int type_tchat_villageois;
    int type_tchat_lg;
    int type_vote_villageois;
    int type_vote_lg;
    int type_vote_voyante;
} joueur_t;

typedef struct liste_joueurs
{
    joueur_t joueurs[MAX_CLIENTS];
    unsigned int nb_joueurs;
    pthread_mutex_t mutex_acces;
} liste_joueurs_t;

typedef struct types_disponibles
{
    int types_tchat_villageois[MAX_CLIENTS];
    int nb_types_tchat_villageois_restants;
    int types_tchat_lg[MAX_CLIENTS];
    int nb_types_tchat_lg_restants;
    int types_votes_villageois[MAX_CLIENTS];
    int nb_types_votes_villageois_restants;
    int types_votes_lg[MAX_CLIENTS];
    int nb_types_votes_lg_restants;
    int types_vote_voyante[1];
    int nb_types_votes_voyante_restants;
} types_disponibles_t;

/* ------------------------------ */
/*    Déclarations de fonctions   */
/* ------------------------------ */
/**
 * @brief Crée une structure role à partir du numéro du role à créer
 * 
 * @param num numéro du role (utiliser les constantes symboliques ROLE_*)
 * @return role_t 
 */
role_t creer_role(int num);

/**
 * @brief Affiche les informations d'un role
 * 
 * @param role 
 * @return int 
 */
int afficher_role(role_t *role);

/**
 * @brief Détermine si un role est null
 * 
 * @param role 
 * @return int 
 */
int est_role_null(role_t *role);

/**
 * @brief Crée dynamiquement une liste de roles disponibles
 * 
 * @return roles_disponibles_t* 
 */
roles_disponibles_t * creer_liste_roles_disponibles();

/**
 * @brief Crée de manière statique une liste de roles disponibles
 * 
 * @return roles_disponibles_t 
 */
roles_disponibles_t init_liste_roles_disponibles();

/**
 * @brief Affiche une liste de roles disponibles
 * 
 * @param roles_disponibles 
 * @return int 
 */
int afficher_liste_roles_disponibles(roles_disponibles_t *roles_disponibles);

/**
 * @brief Retire un role d'une liste de roles disponibles
 * 
 * @param roles_disponibles 
 * @return role_t 
 */
role_t recuperer_role(roles_disponibles_t * roles_disponibles);

/**
 * @brief Crée un joueur de manière dynamique
 * 
 * @param client 
 * @param roles_disponibles 
 * @return joueur_t* 
 */
joueur_t * creer_joueur(client_t *client, roles_disponibles_t *roles_disponibles);

/**
 * @brief Crée un joueur de manière statique
 * 
 * @param client 
 * @param roles_disponibles 
 * @param types_disponibles 
 * @return joueur_t 
 */
joueur_t init_joueur(client_t *client, roles_disponibles_t *roles_disponibles, types_disponibles_t *types_disponibles);

/**
 * @brief Affiche les informations d'un joueur
 * 
 * @param joueur 
 * @return int 
 */
int afficher_joueur(joueur_t *joueur); 

/**
 * @brief Crée de manière dynamique une liste de joueurs
 * 
 * @return liste_joueurs_t* 
 */
liste_joueurs_t * creer_liste_joueurs();

/**
 * @brief Crée de manière statique une liste de joueurs
 * 
 * @return liste_joueurs_t 
 */
liste_joueurs_t init_liste_joueurs();

/**
 * @brief Ajoute un joueur dans une liste de joueurs
 * 
 * @param liste_joueurs 
 * @param joueur 
 * @return int 
 */
int ajouter_joueur(liste_joueurs_t *liste_joueurs, joueur_t *joueur);

/**
 * @brief Retourne l'index d'un joueur dans la liste de joueurs
 * 
 * @param liste_joueurs 
 * @param joueur 
 * @return int index ou -1 si non trouvé
 */
int index_joueur(liste_joueurs_t *liste_joueurs, joueur_t *joueur);

/**
 * @brief Retourne l'index d'un joueur dans la liste de joueurs à partir de son pid
 * 
 * @param liste_joueurs 
 * @param pid 
 * @return int index ou -1 si non trouvé
 */
int index_joueur_pid(liste_joueurs_t *liste_joueurs, pid_t pid);

/**
 * @brief Retire un joueur d'une liste de joueurs
 * 
 * @param liste_joueurs 
 * @param joueur 
 * @return int 
 */
int retirer_joueur(liste_joueurs_t *liste_joueurs, joueur_t *joueur);

/**
 * @brief Ajoute une liste de clients dans une liste de joueurs
 * 
 * @param liste_joueurs 
 * @param liste_clients 
 * @param roles_disponibles 
 * @param types_disponibles 
 * @return int 
 */
int ajouter_joueurs(liste_joueurs_t *liste_joueurs, liste_clients_t *liste_clients, roles_disponibles_t *roles_disponibles, types_disponibles_t *types_disponibles);

/**
 * @brief Affiche une liste de joueurs
 * 
 * @param liste_joueurs 
 * @return int 
 */
int afficher_liste_joueurs(liste_joueurs_t *liste_joueurs);

/**
 * @brief Retourne le nombre de joueurs vivants dans une liste de joueurs
 * 
 * @param liste_joueurs 
 * @return int 
 */
int nb_joueurs_vivants(liste_joueurs_t *liste_joueurs);

/**
 * @brief Retourne le nombre de joueurs vivants et possèdant un certain role dans une liste de joueurs
 * 
 * @param liste_joueurs 
 * @param num_role 
 * @return int 
 */
int nb_joueurs_role(liste_joueurs_t *liste_joueurs, int num_role);

/**
 * @brief Crée de manière statique la liste de types disponibles
 * 
 * @return types_disponibles_t 
 */
types_disponibles_t init_types_disponibles();

/**
 * @brief Affiche une liste de types disponibles
 * 
 * @param types_disponibles 
 * @return int 
 */
int afficher_liste_types_disponibles(types_disponibles_t *types_disponibles);

/**
 * @brief Retourne le role d'un joueur d'une liste de joueurs trouvé à l'aide de son pid
 * 
 * @param liste_joueurs 
 * @param pid 
 * @return role_t 
 */
role_t role_joueur_pid(liste_joueurs_t *liste_joueurs, pid_t pid);


#endif