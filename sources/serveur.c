/*
Liste des types :
    - 2 : Lecture demande de connexion (fait)
    - PID Client : Réponse de connexion + envoi des infos serveur (liste_joueurs, état_partie)
    - 3 : Lecture message client pour tchat villageois
    - 4 : Lecture message client pour tchat LG
    - 5 : Lecture votes villageois
    - 6 : Lecture votes LG
    - 7 : Lecture votes voyante
    - 11 à 30 : Envoi du tchat villageois aux clients
    - 31 à 40 : Envoi du tchat LG aux clients
    - 41 à 60 : Envoi réponse vote villageois
    - 61 à 70 : Envoi réponse vote LG
    - 100 : Envoi réponse vote voyante
 */

#include <bal.h>
#include <connexion.h>

void *gerer_connexions(void *liste_clients);

int main()
{
    srand(time(NULL));

    // Création de la liste de clients
    liste_clients_t *liste_clients = creer_liste_clients();
    assert(liste_clients != NULL);

    // Génération de la clef
    key_t clef = generer_clef_bal();
    assert(clef != -1);

    // Création de la bal
    int id_bal = creer_bal(clef);
    assert(id_bal != -1);

    // On affiche la clé de la bal
    printf("Clef de la boite aux lettres : %d\n", clef);

    // Thread permettant de gérer la connexion des utilisateurs
    pthread_t thread_connexion;
    pthread_create(&thread_connexion, NULL, gerer_connexions, liste_clients);
    pthread_join(thread_connexion, NULL);

    // Suppression de la bal
    int resSupp = supprimer_bal(id_bal);
    assert(resSupp != -1);

    return 0;
}

void *gerer_connexions(void *liste_clients)
{
    liste_clients_t *l = (liste_clients_t *)liste_clients;
    int resLec;
    while (1)
    {
        resLec = lire_demande_connexion(creer_bal(generer_clef_bal()), l);
        assert(resLec != -1);

        printf("Connexion réussie !\n");

        afficher_liste_clients(liste_clients);
    }
    pthread_exit(0);
}