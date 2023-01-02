#include <bal.h>
#include <connexion.h>
#include <joueur.h>
#include <partie.h>

int main()
{
    partie_t partie = creer_partie();

    afficher_partie(&partie);

    return 0;
}