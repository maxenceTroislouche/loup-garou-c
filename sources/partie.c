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