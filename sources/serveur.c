/**
 * \file serveur.c
 * \author Maxence Troislouche
 * \brief 
 * \date 2022-12-03 
 */
/* ---------------------------------- */
/*             Includes               */
/* ---------------------------------- */
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>


/* ---------------------------------- */
/*            Constantes              */
/* ---------------------------------- */
#define MAX_CHAR 500


/* ---------------------------------- */
/*            Structures              */
/* ---------------------------------- */
typedef struct msg_content
{
    char texte[MAX_CHAR];
    pid_t pid;
}msg_content_t;

typedef struct msg
{
    long mtype;
    msg_content_t mtext;
}msg_t;


/* ---------------------------------- */
/*             Fonctions              */
/* ---------------------------------- */
int creerBoite(key_t clef);
void afficherInfosBoite(int idBoite);
int supprimerBoite(int idBoite);
int ecrireMessageBoite(int idBoite, msg_t * message);
int lireMessageBoite(int idBoite, msg_t * message_lu, long type);
void afficherMessage(msg_t * msg);

int main()
{
    /**
     * TODO :
     * - Crée la boite aux lettres
     * - Affiche la clé de la boite
     * - Client écrit type 10, PID, nom de joueur
     * - Serveur renvoie en type 1 l'état de la partie: avec les types ou les clients doivent écrire
     */
    key_t clef = (key_t)getpid();

    printf("Clef de la boite : %d\n", clef);

    int id = creerBoite(clef);
    assert(id != -1);

    printf("Boite crée avec id : %d\n", id);

    msg_t message_recu;

    int resLecture = lireMessageBoite(id, &message_recu, 10);
    assert(resLecture != -1);

    printf("%d : %s\n", message_recu.mtext.pid, message_recu.mtext.texte);

    msg_t message_a_envoyer;
    message_a_envoyer.mtype = message_recu.mtext.pid;
    message_a_envoyer.mtext.pid = getpid();
    strcpy(message_a_envoyer.mtext.texte, "OK");

    int resEcriture = ecrireMessageBoite(id, &message_a_envoyer);
    assert(resEcriture != -1);

    printf("%d s'est connecté !\n", message_recu.mtext.pid);

    supprimerBoite(id);

    return 0;
}

/**
 * Permet de créer une boite aux lettres à partir d'une clé
 * \param clef clé à utiliser pour créer la boite
 * \return -1 si erreur, msg_id sinon
 */
int creerBoite(key_t clef)
{
    int idBoite = msgget(clef, (IPC_CREAT|0660));
    if (idBoite == -1)
    {
        printf("Erreur : lors de la création de la boite aux lettres !\n");
        perror("Erreur perror");
        return -1;
    }
    printf("Boite aux lettres créée avec l'id : %d\n", idBoite);
    return idBoite;
}

/**
 * Permet d'afficher les informations d'une boite
 * \param idBoite msg_id de la boite
 */
void afficherInfosBoite(int idBoite)
{
    struct msqid_ds infosBoite;
    int returnValue = msgctl(idBoite, IPC_STAT, &infosBoite);
    if (returnValue == -1)
    {
        printf("Erreur : lors de la récupération des informations de la boite !\n");
        perror("Erreur perror");
        return;
    }
    printf("Affichage des informations de la boite : %d\n", idBoite);
    printf("Clé associée : %d\n", infosBoite.msg_perm.__key);
    printf("UID Propriétaire : %d\n", infosBoite.msg_perm.uid);
    printf("GID Propriétaire : %d\n", infosBoite.msg_perm.gid);
    printf("UID Créateur : %d\n", infosBoite.msg_perm.cuid);
    printf("GID Créateur : %d\n", infosBoite.msg_perm.cgid);
    printf("Mode : %d\n", infosBoite.msg_perm.mode);
    printf("Seq : %d\n", infosBoite.msg_perm.__seq);
    printf("ctime : %ld\n", infosBoite.msg_ctime);
    printf("lrpid : %d\n", infosBoite.msg_lrpid);
    printf("lspid : %d\n", infosBoite.msg_lspid);
    printf("qbytes : %ld\n", infosBoite.msg_qbytes);
    printf("qnum : %ld\n", infosBoite.msg_qnum);
    printf("rtime : %ld\n", infosBoite.msg_rtime);
    printf("stime : %ld\n", infosBoite.msg_stime);
}

/**
 * Permet de supprimer une boite aux lettres
 * \param idBoite msg_id de la boite
 * \return -1 si erreur, 0 sinon
 */
int supprimerBoite(int idBoite)
{
    int returnValue = msgctl(idBoite, IPC_RMID, NULL);
    if (returnValue == -1)
    {
        printf("Erreur : lors de la suppression de la boite aux lettre !\n");
        perror("Erreur perror");
        return -1;
    }
    printf("Boite supprimée !\n");
    return 0;
}

/**
 * Permet d'envoyer un message dans une boite aux lettres
 * \param idBoite msg_id de la boite
 * \param message message à envoyer
 * \return -1 si erreur, 0 sinon
 */
int ecrireMessageBoite(int idBoite, msg_t * message)
{
    if (message == NULL)
    {
        printf("Erreur ecrireMessageBoite : Pas de message à envoyer !\n");
        return -1;
    }

    int resMsgsnd = msgsnd(idBoite, message, sizeof(msg_t), 0);
    if (resMsgsnd == -1)
    {
        printf("Erreur lors de l'envoi du message !\n");
        perror("Erreur perror");
        return -1;
    }
    printf("Message envoyé !\n");
    return 0;
}

/**
 * Permet de lire un message dans la boite aux lettres
 * @param idBoite msg_id de la boite
 * @param message_lu structure qui va contenir le message lu
 * @return -1 si erreur, 0 sinon
 */
int lireMessageBoite(int idBoite, msg_t * message_lu, long type)
{
    int resMsgrcv = msgrcv(idBoite, message_lu, sizeof(msg_t), type, 0);
    if (resMsgrcv == -1)
    {
        printf("Erreur lors de la réception du message\n");
        perror("Erreur perror");
        return -1;
    }
    printf("Lecture réussie !\n");
    return 0;
}
