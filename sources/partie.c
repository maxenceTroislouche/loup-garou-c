#include <partie.h>


partie_t creer_partie()
{
    partie_t partie;
    partie.etape = 0;
    partie.liste_joueurs = init_liste_joueurs();
    return partie;
}

int afficher_partie(partie_t *partie)
{
    if (partie == NULL)
    {
        printf("Erreur : Impossible d'afficher une partie nulle !\n");
        return -1;
    }

    printf("Affichage de la partie : \n");
    printf("\tEtape %d\n", partie->etape);
    afficher_liste_joueurs(&(partie->liste_joueurs));
    return 0;
}

int commencer_partie(partie_t *partie)
{
    if (partie == NULL)
    {
        printf("Erreur : Impossible de commencer une partie nulle !\n");
        return -1;
    }

    if (partie->etape != 0)
    {
        printf("Erreur : partie déjà commencée !\n");
        return -1;
    }

    partie->etape = 1;
    return 0;
}

int envoyer_infos_partie_joueurs(int id_bal, partie_t *partie)
{
    if (partie == NULL)
    {
        printf("Erreur : Impossible d'envoyer les informations d'une partie nulle");
        return -1;
    }

    infos_partie_t message_infos_partie;
    message_infos_partie.mtext.partie = *partie;

    // Il faut envoyer ce message à tous les joueurs
    // On envoie le message au pid du joueur 
    int i;
    for (i = 0; i < partie->liste_joueurs.nb_joueurs; i++) {
        pid_t pid = partie->liste_joueurs.joueurs[i].client.pid;

        message_infos_partie.mtype = pid;

        int resEcr = ecrire_bal(id_bal, &message_infos_partie, sizeof(msg_infos_partie_t));
        if (resEcr == -1)
        {
            printf("Erreur : Echec de l'envoi des infos de la partie !\n");
            return -1;
        }
    }
}

partie_t lire_infos_partie_joueurs(int id_bal)
{
    if (id_bal == -1)
    {
        printf("Erreur : Impossible de lire les infos de la partie avec un id_bal = -1\n");
        return creer_partie();
    }

    infos_partie_t infos_partie;

    // On lit les infos au type = au pid du joueur

    int resLec = lire_bal(id_bal, &infos_partie, sizeof(msg_infos_partie_t), getpid());
    if (resLec == -1)
    {
        printf("Erreur dans la lecture des infos de la partie\n");
        return creer_partie();
    }

    return infos_partie.mtext.partie;
}