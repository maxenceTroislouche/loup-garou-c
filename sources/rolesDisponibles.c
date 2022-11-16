#include <rolesDisponibles.h>

ListeRolesDisponibles_t *creerListeRolesDisponibles()
{
    ListeRolesDisponibles_t *l = (ListeRolesDisponibles_t *)malloc(sizeof(ListeRolesDisponibles_t));
    if (l == NULL)
        return NULL;

    /**
     * TODO : Vrai répartition comme dans les règles !
     */

    // Une voyante par partie
    l->roles[0] = 2;

    int i;
    // Un tier de loups
    for (i = 1; i < MAX_ROLES / 3; i++)
    {
        l->roles[i] = 1;
    }

    // Reste = villageois
    for (i = i; i < MAX_ROLES; i++)
    {
        l->roles[i] = 0;
    }
    l->nbRolesDisponibles = MAX_ROLES;
    return l;
}

role_t getRoleAleatoire(ListeRolesDisponibles_t *l)
{
    unsigned int index = rand() % l->nbRolesDisponibles;
    role_t role = l->roles[index];
    l->roles[index] = l->roles[--l->nbRolesDisponibles];
    return role;
}

void printListeRolesDisponibles(ListeRolesDisponibles_t *l)
{
    if (l == NULL)
        return;

    printf("Liste des roles disponibles : \n");
    printf("Il reste : %d roles \n", l->nbRolesDisponibles);

    int i;
    for (i = 0; i < MAX_ROLES; i++)
    {
        printf("Role : %d\n", l->roles[i]);
    }
}
