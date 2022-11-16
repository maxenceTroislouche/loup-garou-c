#ifndef ROLES_DISPONIBLES
#define ROLES_DISPONIBLES

#include <role.h>

/**
 * Liste des roles :
 *  - 0 : Villageois
 *  - 1 : Loup
 *  - 2 : Voyante
 */
typedef struct ListeRolesDisponibles
{
    role_t roles[MAX_ROLES];
    unsigned int nbRolesDisponibles;
} ListeRolesDisponibles_t;

ListeRolesDisponibles_t *creerListeRolesDisponibles();
role_t getRoleAleatoire(ListeRolesDisponibles_t *l);
void printListeRolesDisponibles(ListeRolesDisponibles_t *l);


#endif