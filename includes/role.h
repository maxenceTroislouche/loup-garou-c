#ifndef ROLE
#define ROLE

#include <main.h>

#define MAX_NOM_ROLE 50
#define NB_ROLES 3
#define MAX_ROLES 10

typedef unsigned char role_t;

void initNomRoles();
char *getNomRole(role_t role);

#endif