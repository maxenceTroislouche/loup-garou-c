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
#include <joueur.h>
#include <partie.h>

void *gerer_connexions(void *liste_clients);
void *gerer_votes_villageois(void *partie);
void *gerer_votes_lg(void *partie);
void *gerer_vote_voyante(void *partie);


pthread_mutex_t mutex_partie_pleine = PTHREAD_MUTEX_INITIALIZER;

int main()
{
    srand(time(NULL));

    // Création de la liste de clients
    liste_clients_t liste_clients = init_liste_clients();

    // Génération de la clef
    key_t clef = generer_clef_bal();
    assert(clef != -1);

    // Création de la bal
    int id_bal = creer_bal(clef);
    assert(id_bal != -1);

    // On affiche la clé de la bal
    printf("Clef de la boite aux lettres : %d\n", clef);

    // On lock une première fois le mutex
    pthread_mutex_lock(&mutex_partie_pleine);

    // Thread permettant de gérer la connexion des utilisateurs
    pthread_t thread_connexion;
    pthread_create(&thread_connexion, NULL, gerer_connexions, &liste_clients);

    // On attend que la partie soit pleine
    pthread_mutex_lock(&mutex_partie_pleine);

    printf("Partie pleine, on peut commencer !\n");

    // On crée la partie
    roles_disponibles_t roles_disponibles = init_liste_roles_disponibles();
    types_disponibles_t types_disponibles = init_types_disponibles();
    partie_t partie = creer_partie(id_bal);
   
    ajouter_joueurs(&partie.liste_joueurs, &liste_clients, &roles_disponibles, &types_disponibles);
    // Met la partie à l'étape 1
    commencer_partie(&partie);

    afficher_partie(&partie);

    // On envoie les infos aux joueurs
    envoyer_infos_partie_joueurs(id_bal, &partie);

    printf("Infos partie envoyée !\n");

    // Premier tour voyante
    // Deuxieme tour lg
    // Troisieme tour villageois
    // Retour tour un si voyante en vie

    /*
        Etape 0 : Attente des joueurs
        Etape 1 : Le village s'endort : La voyante se réveille
        Etape 2 : Les LG se réveillent : Vote LG
        Etape 3 : Le village se réveille : Vote des villageois
        Etape 4 : Fin de partie
    */

    int nb_lg = nb_joueurs_role(&partie.liste_joueurs, ROLE_LG);
    int nb_joueurs_vivant = nb_joueurs_vivants(&partie.liste_joueurs);
    int nb_voyante = nb_joueurs_role(&partie.liste_joueurs, ROLE_VOYANTE);
    // Jeu en cours tant que des loups garous sont encore en vie ou qu'ils n'y a plus que des loups en jeu
    while(nb_lg > 0 && nb_joueurs_vivant > nb_lg)
    {
        // Tour de la voyante
        if (nb_voyante > 0)
        {
            pthread_t thread_vote_voyante;
            pthread_create(&thread_vote_voyante, NULL, gerer_vote_voyante, &partie);
            pthread_join(thread_vote_voyante, NULL);
            
            // Maintenant au tour des LG
            partie.etape = 2;
            envoyer_infos_partie_joueurs(partie.id_bal, &partie);
        }

        // Tour des loups
        pthread_t thread_votes_lg;
        pthread_create(&thread_votes_lg, NULL, gerer_votes_lg, &partie);
        pthread_join(thread_votes_lg, NULL);
        
        // Tour des villageois
        nb_joueurs_vivant = nb_joueurs_vivants(&partie.liste_joueurs);
        if (nb_lg == 0 || nb_joueurs_vivant == nb_lg)
        {
            partie.etape = 4;
            envoyer_infos_partie_joueurs(partie.id_bal, &partie);
            break;
        }
        else
        {
            partie.etape = 3;
            envoyer_infos_partie_joueurs(partie.id_bal, &partie);
        }

        pthread_t thread_votes_villageois;
        pthread_create(&thread_votes_villageois, NULL, gerer_votes_villageois, &partie);
        pthread_join(thread_votes_villageois, NULL);

        nb_lg = nb_joueurs_role(&partie.liste_joueurs, ROLE_LG);
        nb_voyante = nb_joueurs_role(&partie.liste_joueurs, ROLE_VOYANTE);
        nb_joueurs_vivant = nb_joueurs_vivants(&partie.liste_joueurs);
        
        if (nb_lg == 0 || nb_joueurs_vivant == nb_lg)
            partie.etape = 4;
        else if (nb_voyante > 0)
            partie.etape = 1;
        else partie.etape = 2;
        
        envoyer_infos_partie_joueurs(partie.id_bal, &partie);   
    }

    printf("Fin de la partie !\n");

    // Suppression de la bal
    int resSupp = supprimer_bal(id_bal);
    assert(resSupp != -1);
    
    return 0;
}

void *gerer_connexions(void *liste_clients)
{
    liste_clients_t *l = (liste_clients_t *)liste_clients;
    int resLec;
    int nbJoueurs = 0;
    while (1)
    {
        resLec = lire_demande_connexion(creer_bal(generer_clef_bal()), l);
        assert(resLec != -1);

        printf("Connexion réussie !\n");

        nbJoueurs++;

        if (nbJoueurs == MAX_CLIENTS)
            pthread_mutex_unlock(&mutex_partie_pleine);

        afficher_liste_clients(liste_clients);
    }
    pthread_exit(0);
}

void *gerer_votes_villageois(void *partie)
{
    partie_t *p = (partie_t *)partie;
    int nb_votes = nb_joueurs_eligible_vote_villageois(p);
    gestion_vote_t gestion_vote_villageois;
    gestion_vote_villageois.nb_votes = 0;

    printf("Lecture des votes !\n");
    printf("Nombre de votes à lire : %d\n", nb_votes);

    while(gestion_vote_villageois.nb_votes < nb_votes)
    {
        lire_vote_villageois(p->id_bal, p, &gestion_vote_villageois);
        afficher_gestion_vote(&gestion_vote_villageois);
    }

    pid_t res = resultat_vote(&gestion_vote_villageois);

    int index_joueur = index_joueur_pid(&p->liste_joueurs, res);
    p->liste_joueurs.joueurs[index_joueur].est_vivant = 1;

    pthread_exit(0);
}

void *gerer_votes_lg(void *partie)
{
    partie_t *p = (partie_t *)partie;
    int nb_votes = nb_joueurs_eligible_vote_lg(p);
    gestion_vote_t gestion_vote_lg;
    gestion_vote_lg.nb_votes = 0;

    printf("Lecture des votes LG !\n");
    printf("Nombre de votes à lire : %d\n", nb_votes);

    while(gestion_vote_lg.nb_votes < nb_votes)
    {
        lire_vote_lg(p, &gestion_vote_lg);
        afficher_gestion_vote(&gestion_vote_lg);
    }

    pid_t res = resultat_vote(&gestion_vote_lg);

    int index_joueur = index_joueur_pid(&p->liste_joueurs, res);
    p->liste_joueurs.joueurs[index_joueur].est_vivant = 1;

    pthread_exit(0);
}

void *gerer_vote_voyante(void *partie)
{
    partie_t *p = (partie_t *)partie;
    int nb_votes = 1;

    printf("Lecture du vote voyante !\n");

    while(nb_votes > 0)
    {
        int res = lire_vote_voyante(p);
        if (res == 0)
            nb_votes--;
    }

    pthread_exit(0);
}