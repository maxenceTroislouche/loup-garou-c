#include <bal.h>
#include <connexion.h>

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
    afficher_infos_bal(id_bal);

    int resEnv = envoyer_demande_connexion(id_bal);
    assert(resEnv != -1);

    printf("Connexion réussie !\n");

    return 0;
}