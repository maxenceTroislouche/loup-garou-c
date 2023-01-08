#include <connexion.h>

liste_clients_t *creer_liste_clients()
{
    liste_clients_t *l = (liste_clients_t *)malloc(sizeof(liste_clients_t));
    if (l == NULL)
    {
        printf("Erreur : L'allocation de mémoire de la liste de clients a échoué !\n");
        return NULL;
    }

    l->nb_clients = 0;
    pthread_mutex_init(&(l->mutex_acces), NULL);
    return l;
}

liste_clients_t init_liste_clients()
{
    liste_clients_t liste;
    liste.nb_clients = 0;
    pthread_mutex_init(&(liste.mutex_acces), NULL);
    return liste;
}

client_t * creer_client(pid_t pid, char *nom)
{
    if (nom == NULL)
    {
        printf("Erreur : impossible de créer un client avec un nom NULL !\n");
        return NULL;
    }

    if (strlen(nom) >= MAX_NOM_CLIENT)
    {
        printf("Erreur : Nom du client trop long !\n");
        return NULL;
    }

    client_t * c = (client_t *)malloc(sizeof(client_t));
    if (c == NULL)
    {
        printf("Erreur : L'allocation de mémoire pour la création d'un client a échoué !\n");
        return NULL;
    }

    c->pid = pid;
    strcpy(c->nom, nom);
    return c;
}

client_t init_client(pid_t pid, char *nom)
{
    client_t client;

    if (nom == NULL)
    {
        printf("Erreur : impossible de créer un client avec un nom NULL !\n");
        return client;
    }

    if (strlen(nom) >= MAX_NOM_CLIENT)
    {
        printf("Erreur : Nom du client trop long !\n");
        return client;
    }

    client.pid = pid;
    strcpy(client.nom, nom);
    return client;
}

int index_client(liste_clients_t *l, pid_t pid_client)
{
    if (l == NULL)
    {
        printf("Erreur : Impossible de trouver un client dans une liste nulle !\n");
        return -2;
    }

    pthread_mutex_lock(&(l->mutex_acces));
    for (unsigned int i = 0; i < l->nb_clients; i++)
        if (l->clients[i].pid == pid_client)
        {
            pthread_mutex_unlock(&(l->mutex_acces));
            return i;
        }

    pthread_mutex_unlock(&(l->mutex_acces));
    return -1;
}

int ajouter_client(liste_clients_t *l, client_t client)
{
    if (l == NULL)
    {
        printf("Erreur : Impossible d'ajouter un client dans une liste nulle !\n");
        return -1;
    }

    if (l->nb_clients >= MAX_CLIENTS)
    {
        printf("Erreur : La liste de clients est pleine !\n");
        return -1;
    }

    if (index_client(l, client.pid) != -1)
    {
        printf("Erreur : Ajout d'un client se trouvant déjà dans la liste\n");
        return -1;
    }
    pthread_mutex_lock(&(l->mutex_acces));
    l->clients[l->nb_clients++] = client;
    pthread_mutex_unlock(&(l->mutex_acces));
    return 0;
}

int retirer_client(liste_clients_t *l, client_t client)
{
    if (l == NULL)
    {
        printf("Erreur : Impossible de retirer un client dans une liste nulle !\n");
        return -1;
    }

    int index = index_client(l, client.pid);
    if (index < 0)
        return -1;

    pthread_mutex_lock(&(l->mutex_acces));
    l->clients[index] = l->clients[--l->nb_clients];
    pthread_mutex_unlock(&(l->mutex_acces));
    return 0;
}

int afficher_liste_clients(liste_clients_t *l)
{
    if (l == NULL)
    {
        printf("Erreur : Impossible d'afficher une liste de clients nulle\n");
        return -1;
    }

    printf("Liste de clients (%d):\n", l->nb_clients);

    pthread_mutex_lock(&(l->mutex_acces));
    for (unsigned int i = 0; i < l->nb_clients; i++)
    {
        printf("%d\n", l->clients[i].pid);
        printf("%s\n", l->clients[i].nom);
    }   
    pthread_mutex_unlock(&(l->mutex_acces));
    printf("\n");
    return 0;
}

int lire_demande_connexion(int id_bal, liste_clients_t *liste_clients)
{
    // Est ce que la bal est valide ?
    if (id_bal == -1)
    {
        printf("Erreur : Impossible de lire une demande de connexion d'une bal avec un id = -1\n");
        return -1;
    }

    demande_connexion_t demande;

    // Lecture de la demande de connexion
    int resLec = lire_bal(id_bal, &demande, sizeof(msg_demande_connexion_t), 2);
    if (resLec == -1)
    {
        printf("Erreur dans la lecture de la demande de connexion\n");
        return -1;
    }

    unsigned int i;
    int ok = 0;
    for (i = 0; i < liste_clients->nb_clients; i++)
    {
        if (strcmp(liste_clients->clients[i].nom, demande.mtext.nom) == 0)
        {
            ok = 1;
        }
    }

    if (ok != 1)
    {
        // Créer le client à partir des infos reçus
        client_t *c = creer_client(demande.mtext.pid, demande.mtext.nom);
        if (c == NULL)
        {
            printf("Erreur dans la création du client !\n");
            return -1;
        }

        // Ajout du client dans la liste de clients
        int resAjout = ajouter_client(liste_clients, *c);

        // On envoie une réponse au client sur le type = pid client
        demande_connexion_t reponse;
        reponse.mtype = demande.mtext.pid;
        reponse.mtext.pid = getpid();
        if (resAjout == -1)
            strcpy(reponse.mtext.message, "KO");
        else
            strcpy(reponse.mtext.message, "OK");

        int resEcr = ecrire_bal(id_bal, &reponse, sizeof(msg_demande_connexion_t));
        return resEcr;
    }
    else
    {
        demande_connexion_t reponse;
        reponse.mtype = demande.mtext.pid;
        reponse.mtext.pid = getpid();
        strcpy(reponse.mtext.message, "KO");
        int resEcr = ecrire_bal(id_bal, &reponse, sizeof(msg_demande_connexion_t));
        return resEcr;
    }
}

int envoyer_demande_connexion(int id_bal, char *nom)
{
    if (id_bal == -1)
    {
        printf("Erreur : Impossible d'envoyer une demande de connexion dans une bal avec un id = -1\n");
        return -1;
    }

    if (nom == NULL)
    {
        printf("Erreur : Impossible d'envoyer une demande de connexion sans un nom de joueur !\n");
        return -1;
    }

    if (strlen(nom) >= MAX_NOM_CLIENT)
    {
        printf("Erreur : Impossible d'envoyer une demande de connexion avec un nom aussi long !\n");
        return -1;
    }

    nom[strlen(nom) - 1] = '\0';

    demande_connexion_t demande;
    demande.mtype = 2;
    demande.mtext.pid = getpid();
    strcpy(demande.mtext.nom, nom);

    int resEcr = ecrire_bal(id_bal, &demande, sizeof(msg_demande_connexion_t));
    if (resEcr == -1)
    {
        printf("Erreur : Echec de l'envoi de la demande de connexion\n");
        return -1;
    }

    demande_connexion_t reponse;

    int resLec = lire_bal(id_bal, &reponse, sizeof(msg_demande_connexion_t), getpid());
    if (resLec == -1)
    {
        printf("Erreur : Echec de la lecture de la reponse serveur\n");
        return -1;
    }

    if (strcmp(reponse.mtext.message, "OK") == 0)
        return 0;

    return -1;
}
