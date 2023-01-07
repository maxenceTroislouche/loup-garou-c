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

int envoyer_vote_villageois(int id_bal, partie_t *partie)
{
    if (partie == NULL)
    {
        printf("Erreur : Impossible de voter pour un joueur dans une partie nulle !\n");
        return NULL;
    }

    if (id_bal == -1)
    {
        printf("Erreur : Impossible de voter avec un id_bal de -1 !\n");
        return -1;
    }

    // Affiche à l'utilisateur la liste des personnes pour qui elle peut voter
    liste_joueurs_t liste_joueurs_possibles = init_liste_joueurs();
    pid_t mon_pid = getpid();

    int i;
    for (i = 0; i < partie->liste_joueurs.nb_joueurs; i++)
    {
        // Si le joueur est nous meme
        if (partie->liste_joueurs.joueurs[i].client.pid == mon_pid)
            continue;

        // Si le joueur est mort
        if (partie->liste_joueurs.joueurs[i].est_vivant != 0)
            continue;

        ajouter_joueur(&liste_joueurs_possibles, &partie->liste_joueurs.joueurs[i]);
    }
    
    printf("Pour qui voulez vous voter ?\n");

    for (i = 0; i < liste_joueurs_possibles.nb_joueurs; i++)
    {
        printf("- %d : %s\n", i, liste_joueurs_possibles.joueurs[i].client.nom);
    }


    // Demande à l'utilisateur son vote
    int vote;

    scanf("%d\n", &vote);

    if (vote < 0 || vote >= liste_joueurs_possibles.nb_joueurs)
    {
        printf("Votre vote n'est pas possible\n");
        return envoyer_vote_villageois(id_bal, partie);
    }

    // Envoie le vote au serveur (type : 5)
    vote_villageois_t vote_villageois;
    vote_villageois.mtype = 5;
    vote_villageois.mtext.votant = mon_pid;
    vote_villageois.mtext.vote = liste_joueurs_possibles.joueurs[vote].client.pid;

    int resEcr = ecrire_bal(id_bal, &vote_villageois, sizeof(msg_vote_villageois_t));
    if (resEcr == -1)
    {
        printf("Erreur : Echec de l'envoi du vote\n");
        return -1;
    }

    // Il faut récupérer le type sur lequel on doit écouter
    int index = index_joueur_pid(&partie->liste_joueurs, mon_pid);

    if (index == -1)
    {
        printf("Erreur : Impossible de récupérer le type d'écoute pour le vote villageois !\n");
        return -1;
    }

    int type_reponse = partie->liste_joueurs.joueurs[i].type_vote_villageois;

    // Attend la réponse du serveur (type se trouvant dans la structure joueur)
    reponse_serveur_t reponse;
    int resLec = lire_bal(id_bal, &reponse, sizeof(char) * 3, type_reponse);
    if (resLec == -1)
    {
        printf("Erreur : Echec de la lecture de la reponse serveur\n");
        return -1;
    }
    
    if (strcmp(reponse.mtext, "OK") == 0)
    {
        // Si ok on quitte la fonction
        return 0;
    }
    else 
    {
        // Sinon on redemande à l'utilisateur son vote
        return envoyer_vote_villageois(id_bal, partie);
    }
}

int lire_vote_villageois(int id_bal, partie_t *partie, gestion_vote_villageois_t *gestion_vote_villageois)
{
    if (partie == NULL)
    {
        printf("Erreur : Impossible de lire un vote de villageois avec une partie nulle !\n");
        return -1;
    }

    if (gestion_vote_villageois == NULL)
    {
        printf("Erreur : Impossible de lire un vote villageois avec un gestionnaire de vote null !\n");
        return -1;
    }

    if (id_bal == -1)
    {
        printf("Erreur : Impossible de lire un vote villageois avec un id_bal de -1 !\n");
        return -1;
    }

    vote_villageois_t vote_lu;
    int resLec = lire_bal(id_bal, &vote_lu, sizeof(msg_vote_villageois_t), 5);
    if (resLec == -1)
    {
        printf("Erreur : erreur lors de la lecture du vote villageois !\n");
        return -1;
    }

    // On regarde de quel joueur il s'agit
    
    pid_t votant = vote_lu.mtext.votant;
    pid_t vote = vote_lu.mtext.vote;
    int i;
    int ok = 0; // 0 : ok / 1 : ko
    
    // Si le joueur est le votant alors on annule
    if (votant == vote)
        ok = 1;

    if (ok != 1)
    {
        // Si le votant a déjà voté, alors on annule
        for (i = 0; i < gestion_vote_villageois->nb_votes i++)
        {
            if (votant == gestion_vote_villageois->votant[i])
            {
                ok = 1;
                break;
            }
        }
    }
    
    int index_votant = -1;
    int index_joueur = -1;

    if (ok != 1)
    {
        // Si le joueur ne se trouve pas dans la partie alors le vote n'est pas valide
        index_joueur = index_joueur_pid(&partie->liste_joueurs, vote);

        if (index_joueur == -1)
            ok = 1;
    }

    if (ok != 1)
    {
        // Si le votant ne se trouve pas dans la partie alors le vote n'est pas valide
        index_votant= index_joueur_pid(&partie->liste_joueurs, votant);

        // On ne peut pas renvoyer de réponse au client :) 
        return -1;
    }

    // Si le votant est mort, alors on annule
    if (ok != 1)
    {
        if (partie->liste_joueurs.joueurs[index_votant].est_vivant != 0)
            ok = 1;
    }

    // Si le joueur est mort alors on annule
    if (ok != 1)
    {
        if (partie->liste_joueurs.joueurs[index_joueur].est_vivant != 0)
            ok = 1;
    }

    // Sinon OK
    reponse_serveur_t reponse_serveur;
    reponse_serveur.mtype = partie->liste_joueurs.joueurs[index_votant].type_vote_villageois;
    if (ok == 0)
        strcpy(reponse_serveur.mtext, "OK");
    else
        strcpy(reponse_serveur.mtext, "KO");

    int resEcr(id_bal, &reponse_serveur, sizeof(char) * 3);
    if (resEcr == -1)
    {
        printf("Erreur : Echec lors de l'envoi de la réponse au client !\n");
        return -1;
    }

    gestion_vote_villageois->votant[gestion_vote_villageois->nb_votes] = votant;
    gestion_vote_villageois->vote[gestion_vote_villageois->nb_votes] = vote;
    gestion_vote_villageois->nb_votes++;
    return 0;
}

pid_t resultat_vote_villageois(gestion_vote_villageois_t *gestion_vote_villageois)
{
    if (gestion_vote_villageois == NULL)
    {
        printf("Erreur : Impossible de déterminer le résultat du vote si le gestionnaire de vote est null!\n");
        return -1;
    }

    pid_t vote[MAX_CLIENTS];
    int total[MAX_CLIENTS];
    int vote_differents = 0;

    int i = 0;
    int j = 0;
    int trouve = 0;
    for (i = 0; i < gestion_vote_villageois->nb_votes; i++)
    {
        for (j = 0; j < vote_differents; j++)
        {
            if (vote[j] == gestion_vote_villageois->vote[i])
            {
                trouve = 1;
                total[j]++;
            }
        }
        if (trouve == 0)
        {
            vote_differents++;
            vote[vote_differents] = gestion_vote_villageois->vote[i];
            total[vote_differents] = 1;
        }
    }

    pid_t res;
    int max = 0;
    for (i = 0; i < vote_differents; i++)
    {
        if (total[i] > max)
        {
            res = vote[i];
            max = total[i];
        }
    }

    return res;
}