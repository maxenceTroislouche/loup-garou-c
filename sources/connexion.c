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
    return l;
}

int index_client(liste_clients_t *l, pid_t client)
{
    if (l == NULL)
    {
        printf("Erreur : Impossible de trouver un client dans une liste nulle !\n");
        return -1;
    }
    
    for (unsigned int i = 0; i < l->nb_clients; i++)
        if (l->clients[i] == client)
            return i;

    return -1;
}

int ajouter_client(liste_clients_t *l, pid_t client)
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

    if (index_client(l, client) != -1)
    {
        printf("Erreur : Ajout d'un client se trouvant déjà dans la liste\n");
        return -1;
    }

    l->clients[l->nb_clients++] = client;
    return 0;
}

int retirer_client(liste_clients_t *l, pid_t client)
{
    if (l == NULL)
    {
        printf("Erreur : Impossible de retirer un client dans une liste nulle !\n");
        return -1;
    }

    int index = index_client(l, client);
    if (index == -1)
        return -1;

    l->clients[index] = l->clients[--l->nb_clients];
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

    for (unsigned int i = 0; i < l->nb_clients; i++)
        printf("%d\n", l->clients[i]);

    printf("\n");
    return 0;
}

int lire_demande_connexion(int id_bal, liste_clients_t *liste_clients)
{
    // Est ce que la bal est valide
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

    // Ajout du client dans la liste de clients
    int resAjout = ajouter_client(liste_clients, demande.mtext.pid);
 
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

int envoyer_demande_connexion(int id_bal)
{
    if (id_bal == -1)
    {
        printf("Erreur : Impossible d'envoyer une demande de connexion dans une bal avec un id = -1\n");
        return -1;
    }

    demande_connexion_t demande;
    demande.mtype = 2;
    demande.mtext.pid = getpid();

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

    if(strcmp(reponse.mtext.message, "OK") == 0)
        return 0;

    return -1;
}
