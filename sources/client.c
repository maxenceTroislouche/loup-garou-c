/*
Liste des types :
    - 2 : Demande de connexion (fait)
    - PID Client : Réponse de connexion + réception des infos serveur (liste_joueurs, état_partie)
    - 3 : Ecriture dans le tchat villageois
    - 4 : Ecriture dans le tchat LG
    - 5 : Ecriture vote villageois
    - 6 : Ecriture vote LG
    - 7 : Ecriture vote voyante
    - 11 à 30 : Lecture dans le tchat villageois
    - 31 à 40 : Lecture dans le tchat LG
    - 41 à 60 : Lecture réponse vote villageois
    - 61 à 70 : Lecture réponse vote LG
    - 100 : Lecture réponse vote voyante
 */

#include <bal.h>
#include <connexion.h>
#include <joueur.h>
#include <partie.h>

pthread_mutex_t mutex_partie_commence = PTHREAD_MUTEX_INITIALIZER;

void jouer_voyante(partie_t *partie);
void jouer_lg(partie_t *partie);
void jouer_villageois(partie_t *partie, liste_joueurs_t *liste_joueurs_morts);

int main()
{
    // Affichage du pid du client
    printf("Mon PID : %d\n", getpid());

    // On demande la clef à l'utilisateur
    key_t clef;
    printf("Quelle est la clef de la partie ?\n");
    scanf("%d", &clef);

    // Création de la bal
    int id_bal = creer_bal(clef);
    assert(id_bal != -1);

    // Afficher les infos de la bal
    //afficher_infos_bal(id_bal);

    char nom[MAX_NOM_CLIENT];

    printf("Quel est votre nom ?\n");
    getchar();
    fgets(nom, MAX_NOM_CLIENT, stdin);

    printf("Votre nom est %s\n", nom);

    // Envoi de la demande de connexion
    int resEnv = envoyer_demande_connexion(id_bal, nom);
    if (resEnv == -1)
    {
        printf("La partie est pleine ou nom déjà pris !\n");
        return 1;
    }
    
    pthread_mutex_lock(&mutex_partie_commence);

    printf("Connexion réussie !\n");

    partie_t partie = lire_infos_partie_joueurs(id_bal);
    liste_joueurs_t liste_joueurs_morts = init_liste_joueurs();

    role_t mon_role = role_joueur_pid(&partie.liste_joueurs, getpid());

    printf("Votre role est %s\n", mon_role.nom);
 
    while(partie.etape != 4)
    {
        afficher_roles_restants_partie(&partie);

        if (partie.etape == 1)
        {
            jouer_voyante(&partie);
            partie = lire_infos_partie_joueurs(id_bal);
        }
        else if(partie.etape == 2)
        {
            jouer_lg(&partie);
            partie = lire_infos_partie_joueurs(id_bal);
        }
        else if(partie.etape == 3)
        {
            jouer_villageois(&partie, &liste_joueurs_morts);
            partie = lire_infos_partie_joueurs(id_bal);
            printf("Après le vote du village : ");
            afficher_dernier_joueur_mort(&partie, &liste_joueurs_morts);
        }
        else
        {
            printf("Erreur etape de la partie non pris en compte !\n");
            return 1;
        }
    }

    afficher_res_partie(&partie);

    return 0;
}

void jouer_voyante(partie_t *partie)
{
    role_t role = role_joueur_pid(&partie->liste_joueurs, getpid());
    if (role.num == ROLE_VOYANTE && partie->liste_joueurs.joueurs[index_joueur_pid(&partie->liste_joueurs, getpid())].est_vivant == 0)
    {
        printf("C'est à votre tour de jouer\n");
        envoyer_vote_voyante(partie);
    }
    else if (partie->liste_joueurs.joueurs[index_joueur_pid(&partie->liste_joueurs, getpid())].est_vivant == 1)
    {
        printf("Vous etes mort, c'est à la voyante de jouer !\n");
    }
    else
    {
        printf("C'est à la voyante de jouer !\n");
    }
}

void jouer_lg(partie_t *partie)
{
    role_t role = role_joueur_pid(&partie->liste_joueurs, getpid());
    if (role.num == ROLE_LG && partie->liste_joueurs.joueurs[index_joueur_pid(&partie->liste_joueurs, getpid())].est_vivant == 0)
    {
        printf("C'est à votre tour de jouer\n");
        envoyer_vote_lg(partie);
    }
    else if (role.num == ROLE_LG && partie->liste_joueurs.joueurs[index_joueur_pid(&partie->liste_joueurs, getpid())].est_vivant == 1)
    {
        printf("Vous etes mort, c'est aux loups garous de jouer !\n");
    }
    else
    {
        printf("C'est aux loups garous de jouer !\n");
    }
}
void jouer_villageois(partie_t *partie, liste_joueurs_t *liste_joueurs_morts)
{
    printf("C'est à votre tour de jouer !\n");
    printf("Pendant la nuit : ");
    afficher_dernier_joueur_mort(partie, liste_joueurs_morts);
    if (partie->liste_joueurs.joueurs[index_joueur_pid(&partie->liste_joueurs, getpid())].est_vivant == 0)
        envoyer_vote_villageois(partie->id_bal, partie);
    else
        printf("Vous etes morts, c'est aux villageois de jouer\n");
}
