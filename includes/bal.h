#ifndef BAL
#define BAL

/* ------------------------------ */
/*            Includes            */
/* ------------------------------ */
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>
#include <pthread.h>

/* ------------------------------ */
/*    Déclarations de fonctions   */
/* ------------------------------ */

/**
 * @brief Génère la clé permettant la création de la bal
 * @return key_t clé
 */
key_t generer_clef_bal();

/**
 * @brief Crée une bal
 *
 * @param clef clé de création de la bal
 * @return int -1 en cas d'échec, id de la bal sinon
 */
int creer_bal(key_t clef);

/**
 * @brief Supprime une bal
 *
 * @param id_bal id de la boite à supprimer
 * @return int -1 en cas d'échec, 0 si réussi
 */
int supprimer_bal(int id_bal);

/**
 * @brief Affiche les infos concernant une bal
 *
 * @param id_bal id de la boite à afficher
 */
void afficher_infos_bal(int id_bal);

/**
 * @brief Ecrit un message dans une bal
 *
 * @param id_bal id de la bal
 * @param message pointeur vers le message à envoyer
 * @param message_size taille du message à envoyer
 * @return int -1 si echec, 0 si réussi
 */
int ecrire_bal(int id_bal, void *message, size_t message_size);

/**
 * @brief Lit un message dans une bal
 *
 * @param id_bal id de la bal
 * @param message_lu pointeur vers une structure vide dans laquelle sera copié le message
 * @param message_size taille du message à lire
 * @param type type sur lequel on lit le message
 * @return int -1 si échec, 0 si réussi
 */
int lire_bal(int id_bal, void *message_lu, size_t message_size, long type);

#endif