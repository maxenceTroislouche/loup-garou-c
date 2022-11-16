#include <joueur.h>

Joueur_t *creerJoueur(char *nom, ListeRolesDisponibles_t *l)
{
    if (nom == NULL || strlen(nom) > 50)
        return NULL;

    Joueur_t *j = (Joueur_t *)malloc(sizeof(Joueur_t));
    if (j == NULL)
        return NULL;

    strcpy(j->nom, nom);
    j->role = getRoleAleatoire(l);
    j->vivant = true;
    return j;
}

void printJoueur(Joueur_t *j)
{
    if (j == NULL)
        return;

    printf("Joueur : %s / role : %s / vivant : %s\n", j->nom, getNomRole(j->role), j->vivant ? "true" : "false");
}
