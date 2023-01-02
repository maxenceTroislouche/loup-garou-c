#include <joueur.h>

role_t creer_role(int num)
{
    role_t role;
    if (num < 0 || num > 2)
    {
        role.num = -1;
        strcpy(role.nom, "NULL");
        return role;
    }

    role.num = num;
    switch(num)
    {
        case 0:
            strcpy(role.nom, "Villageois"); 
            break;

        case 1:
            strcpy(role.nom, "Loup-Garou");
            break;

        case 2:
            strcpy(role.nom, "Voyante");
            break;

        default:
            break;
    }
    return role;
}

int afficher_role(role_t *role)
{
    if (role == NULL)
    {
        printf("Erreur : Impossible d'afficher un role null !\n");
        return -1;
    }

    printf("Numéro du role : %d / Nom du role : %s\n", role->num, role->nom);
    return 0;
}

int est_role_null(role_t *role)
{
    if (role == NULL)
        return 0;

    if (role->num == -1)
        return 0;

    return 1;
}

roles_disponibles_t * creer_liste_roles_disponibles()
{
    roles_disponibles_t *roles_disponibles = (roles_disponibles_t *)malloc(sizeof(roles_disponibles_t));
    if (roles_disponibles == NULL)
    {
        printf("Erreur : allocation de mémoire de la liste de roles disponibles !\n");
        return NULL;
    }

    roles_disponibles->nb_roles_disponibles = MAX_CLIENTS;
    
    // Une voyante dans la partie
    // TODO : Faire une allocation des roles dynamiques qui prend en compte le nombre max de clients
    roles_disponibles->roles[0] = creer_role(ROLE_VOYANTE);
    roles_disponibles->roles[1] = creer_role(ROLE_LG);
    roles_disponibles->roles[2] = creer_role(ROLE_LG);
    roles_disponibles->roles[3] = creer_role(ROLE_VILLAGEOIS);
    roles_disponibles->roles[4] = creer_role(ROLE_VILLAGEOIS);
    
    return roles_disponibles;
}

roles_disponibles_t init_liste_roles_disponibles()
{
    roles_disponibles_t roles_disponibles;
    roles_disponibles.nb_roles_disponibles = MAX_CLIENTS;

    // TODO : Distribution dynamique des roles
    roles_disponibles.roles[0] = creer_role(ROLE_VOYANTE);
    roles_disponibles.roles[1] = creer_role(ROLE_LG);
    roles_disponibles.roles[2] = creer_role(ROLE_LG);
    roles_disponibles.roles[3] = creer_role(ROLE_VILLAGEOIS);
    roles_disponibles.roles[4] = creer_role(ROLE_VILLAGEOIS);

    return roles_disponibles;
}

int afficher_liste_roles_disponibles(roles_disponibles_t *roles_disponibles)
{
    if (roles_disponibles == NULL)
    {
        printf("Erreur : Impossible d'afficher une liste de roles disponibles nulle !\n");
        return -1;
    }

    printf("Liste des roles disponibles : \n");

    for (unsigned int i = 0; i < roles_disponibles->nb_roles_disponibles; i++)
    {
        afficher_role(&roles_disponibles->roles[i]);
    }
    return 0;
}

role_t recuperer_role(roles_disponibles_t * roles_disponibles)
{
    if (roles_disponibles == NULL)
    {
        printf("Erreur : Impossible de retirer un role d'une liste NULL !\n");
        return creer_role(-1);
    }

    int index = rand() % roles_disponibles->nb_roles_disponibles;
    role_t role = roles_disponibles->roles[index];
    roles_disponibles->roles[index] = roles_disponibles->roles[--roles_disponibles->nb_roles_disponibles];
    return role;
}

joueur_t * creer_joueur(client_t *client, roles_disponibles_t *roles_disponibles)
{
    if (client == NULL)
    {
        printf("Erreur : Impossible de créer un joueur ayant un client NULL !\n");
        return NULL;
    }

    if (roles_disponibles == NULL)
    {
        printf("Erreur : Impossible de créer un joueur avec une liste de roles disponibles nulle !\n");
        return NULL;
    }

    role_t role = recuperer_role(roles_disponibles);
    if (est_role_null(&role) == 0)
    {
        printf("Erreur : Impossible de créer un joueur avec un role null !\n");
        return NULL;
    }

    joueur_t *joueur = (joueur_t *)malloc(sizeof(joueur_t));
    joueur->client = *client;
    joueur->role = role;
    joueur->est_vivant = 0;
    return joueur;
}

joueur_t init_joueur(client *client, roles_disponibles_t *roles_disponibles)
{
    if (client == NULL)
    {
        printf("Erreur : Impossible de créer un joueur ayant un client NULL !\n");
        return NULL;
    }

    if (roles_disponibles == NULL)
    {
        printf("Erreur : Impossible de créer un joueur avec une liste de roles disponibles nulle !\n");
        return NULL;
    }

    role_t role = recuperer_role(roles_disponibles);
    if (est_role_null(&role) == 0)
    {
        printf("Erreur : Impossible de créer un joueur avec un role null !\n");
        return NULL;
    }

    joueur_t joueur;
    joueur.client = *client;
    joueur.role = role;
    joueur.est_vivant = 0;
    return joueur;
}

int afficher_joueur(joueur_t *joueur)
{
    if (joueur == NULL)
    {
        printf("Erreur : Impossible d'afficher un joueur null !\n");
        return -1;
    }

    printf("PID : %d / Nom : %s / Role : %s / Est vivant : %d\n", joueur->client.pid, joueur->client.nom, joueur->role.nom, joueur->est_vivant);
    return 0;
}

liste_joueurs_t * creer_liste_joueurs()
{
    liste_joueurs_t *liste = (liste_joueurs_t *)malloc(sizeof(liste_joueurs_t));
    if (liste == NULL)
    {
        printf("Erreur : Allocation de mémoire création de la liste de joueur a échoué !\n");
        return NULL;
    }

    pthread_mutex_init(&(liste->mutex_acces), NULL);
    liste->nb_joueurs = 0;
    return liste;
}

liste_joueurs_t init_liste_joueurs()
{
    liste_joueurs_t liste_joueurs;
    pthread_mutex_init(&(liste_joueurs.mutex_acces), NULL);
    liste_joueurs.nb_joueurs = 0;
    return liste_joueurs;
}

int ajouter_joueur(liste_joueurs_t *liste_joueurs, joueur_t *joueur)
{
    if (liste_joueurs == NULL)
    {
        printf("Erreur : Impossible d'ajouter un joueur dans une liste nulle !\n");
        return -1;
    }

    if (joueur == NULL)
    {
        printf("Erreur : Impossible d'ajouter un joueur null dans une liste !\n");
        return -1;
    }

    if (liste_joueurs->nb_joueurs >= MAX_CLIENTS)
    {
        printf("Erreur : Impossible d'ajouter un joueur dans une liste pleine !\n");
        return -1;
    }

    pthread_mutex_lock(&liste_joueurs->mutex_acces);
    liste_joueurs->joueurs[liste_joueurs->nb_joueurs++] = *joueur;
    pthread_mutex_unlock(&liste_joueurs->mutex_acces);
    return 0;
}

int index_joueur(liste_joueurs_t *liste_joueurs, joueur_t *joueur)
{
    if (liste_joueurs == NULL)
    {
        printf("Erreur : Impossible de trouver un joueur dans une liste nulle !\n");
        return -2;
    }

    if (joueur == NULL)
    {
        printf("Erreur : Impossible de trouver un joueur null dans une liste !\n");
        return -2;
    }

    pthread_mutex_lock(&liste_joueurs->mutex_acces);
    for (unsigned int i = 0; i < liste_joueurs->nb_joueurs; i++)
    {
        if (joueur->client.pid == liste_joueurs->joueurs[i].client.pid)
        {
            pthread_mutex_unlock(&liste_joueurs->mutex_acces);
            return i;
        }
    }
    pthread_mutex_unlock(&liste_joueurs->mutex_acces);
    return -1;
}

int retirer_joueur(liste_joueurs_t *liste_joueurs, joueur_t *joueur)
{
    if (liste_joueurs == NULL)
    {
        printf("Erreur : Impossible de retirer un joueur d'une liste nulle !\n");
        return -1;
    }

    if (joueur == NULL)
    {
        printf("Erreur : Impossible de retirer une joueur null d'une liste !\n");
        return -1;
    }

    int index = index_joueur(liste_joueurs, joueur);

    if (index < 0)
    {
        printf("Erreur : Impossible de retirer un joueur ne se trouvant pas dans la liste !\n");
        return -1;
    }

    pthread_mutex_lock(&liste_joueurs->mutex_acces);
    liste_joueurs->joueurs[index] = liste_joueurs->joueurs[liste_joueurs->nb_joueurs--];
    pthread_mutex_unlock(&liste_joueurs->mutex_acces);
    return 0;
}

int ajouter_joueurs(liste_joueurs_t *liste_joueurs, liste_clients_t *liste_clients, roles_disponibles_t *roles_disponibles)
{
    if (liste_joueurs == NULL)
    {
        printf("Erreur : Impossible d'ajouter des joueurs dans une liste de joueurs nulle !\n");
        return -1;
    }

    if (liste_clients == NULL)
    {
        printf("Erreur : Impossible d'ajouter des joueurs avec une liste de clients nulle !\n");
        return -1;
    }

    if (roles_disponibles == NULL)
    {
        printf("Erreur : Impossible d'ajouter des joueurs avec une liste de roles disponibles nulle !\n");
        return -1;
    }

    joueur_t *j;

    for (unsigned int i = 0; i < liste_clients->nb_clients; i++)
    {
        j = creer_joueur(&liste_clients->clients[i], roles_disponibles);
        ajouter_joueur(liste_joueurs, j);
    }
    return 0;
}

int afficher_liste_joueurs(liste_joueurs_t *liste_joueurs)
{
    if (liste_joueurs == NULL)
    {
        printf("Erreur : Impossible d'afficher une liste de joueurs nulle !\n");
        return -1;
    }

    pthread_mutex_lock(&liste_joueurs->mutex_acces);
    for (unsigned int i = 0; i < liste_joueurs->nb_joueurs; i++)
    {
        afficher_joueur(&liste_joueurs->joueurs[i]);
    }
    pthread_mutex_unlock(&liste_joueurs->mutex_acces);
    return 0;
}

int nb_joueurs_vivants(liste_joueurs_t *liste_joueurs)
{
    if (liste_joueurs == NULL)
    {
        printf("Erreur : Impossible de compter le nombre de joueurs vivants dans une liste nulle !\n");
        return -1;
    }
    int nb_joueurs_vivants = 0;
    int i;
    for (i = 0; i < liste_joueurs->nb_joueurs; i++)
    {
        if (liste_joueurs->joueurs[i].est_vivant == 0)
        {
            nb_joueurs_vivants++;
        }
    }
    return nb_joueurs_vivants;
}