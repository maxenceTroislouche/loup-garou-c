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
partie_t partie;

int main()
{
    // Affichage du pid du client
    printf("Mon PID : %d\n", getpid());

    // On demande la clef à l'utilisateur
    key_t clef;
    printf("Quelle est la clef de la bal ?\n");
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
        printf("La partie est pleine !\n");
        return 1;
    }
    
    pthread_mutex_lock(&mutex_partie_commence);

    printf("Connexion réussie !\n");

    partie_t partie = lire_infos_partie_joueurs(id_bal);

    afficher_partie(&partie);

   // envoyer_vote_villageois(id_bal, &partie);

    int index_joueur = index_joueur_pid(&partie.liste_joueurs, getpid());

    if (partie.liste_joueurs.joueurs[index_joueur].role.num == ROLE_LG)
    {
        envoyer_vote_lg(&partie);
    }

    partie = lire_infos_partie_joueurs(id_bal);

    afficher_partie(&partie);

    return 0;
}