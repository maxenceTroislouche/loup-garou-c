#include <bal.h>
#include <connexion.h>
#include <joueur.h>
#include <partie.h>

int main()
{
    srand(time(NULL));
    partie_t partie = creer_partie();

    roles_disponibles_t roles_disponibles = init_liste_roles_disponibles();
    afficher_liste_roles_disponibles(&roles_disponibles);

    types_disponibles_t types_disponibles = init_types_disponibles();
    afficher_liste_types_disponibles(&types_disponibles);

    client_t client[MAX_CLIENTS];
    joueur_t joueurs[MAX_CLIENTS];
    for (int i = 0; i < MAX_CLIENTS; i++)
    {
        client[i] = init_client(i * 1000 + 1000, "Maxence");
        joueurs[i] = init_joueur(&client[i], &roles_disponibles, &types_disponibles);
        afficher_joueur(&joueurs[i]);
    }

    return 0;
}