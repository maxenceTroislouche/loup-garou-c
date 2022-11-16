#include <listeJoueurs.h>

ListeJoueur_t *creerListeJoueurs()
{
    ListeJoueur_t *l = (ListeJoueur_t *)malloc(sizeof(ListeJoueur_t));
    if (l == NULL)
        return NULL;

    l->nbJoueurs = 0;
    return l;
}

bool ajouterJoueurDansListeJoueurs(ListeJoueur_t *l, Joueur_t *j)
{
    if (l == NULL || j == NULL)
        return false;

    if (l->nbJoueurs >= MAX_JOUEURS)
        return false;

    l->joueurs[l->nbJoueurs++] = *j;
    return true;
}

void printListeJoueurs(ListeJoueur_t *l)
{
    if (l == NULL)
        return;

    printf("Liste des joueurs : \n");

    unsigned int i;
    for (i = 0; i < l->nbJoueurs; i++)
    {
        printJoueur(&(l->joueurs[i]));
    }
}