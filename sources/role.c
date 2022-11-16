#include <role.h>

char nomRoles[NB_ROLES][MAX_NOM_ROLE];

void initNomRoles()
{
    strcpy(nomRoles[0], "Villageois");
    strcpy(nomRoles[1], "Loup-Garou");
    strcpy(nomRoles[2], "Voyante");
}

char *getNomRole(role_t role)
{
    if (role >= NB_ROLES)
        return NULL;

    return nomRoles[role];
}
