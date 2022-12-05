/* ------------------------------ */
/*            Includes            */
/* ------------------------------ */
#include <bal.h>

/* ------------------------------ */
/*    Déclarations de fonctions   */
/* ------------------------------ */
key_t generer_clef_bal()
{
    return getpid();
}

int creer_bal(key_t clef)
{
    int id_bal = msgget(clef, (IPC_CREAT | 0660));
    if (id_bal == -1)
    {
        perror("Erreur perror");
        return -1;
    }
    return id_bal;
}

int supprimer_bal(int id_bal)
{
    if (id_bal == -1)
    {
        printf("Erreur : Impossible de supprimer une boite ayant un id de -1\n");
        return -1;
    }

    int returnValue = msgctl(id_bal, IPC_RMID, NULL);
    if (returnValue == -1)
    {
        perror("Erreur perror");
        return -1;
    }
    return 0;
}

void afficher_infos_bal(int id_bal)
{
    if (id_bal == -1)
    {
        printf("Erreur : Impossible d'afficher les infos d'une boite ayant un id de -1\n");
        return;
    }
    struct msqid_ds infos_bal;
    int returnValue = msgctl(id_bal, IPC_STAT, &infos_bal);
    if (returnValue == -1)
    {
        perror("Erreur perror");
        return;
    }
    printf("Affichage des informations de la boite : %d\n", id_bal);
    printf("Clé associée : %d\n", infos_bal.msg_perm.__key);
    printf("UID Propriétaire : %d\n", infos_bal.msg_perm.uid);
    printf("GID Propriétaire : %d\n", infos_bal.msg_perm.gid);
    printf("UID Créateur : %d\n", infos_bal.msg_perm.cuid);
    printf("GID Créateur : %d\n", infos_bal.msg_perm.cgid);
    printf("Mode : %d\n", infos_bal.msg_perm.mode);
    printf("Seq : %d\n", infos_bal.msg_perm.__seq);
    printf("ctime : %ld\n", infos_bal.msg_ctime);
    printf("lrpid : %d\n", infos_bal.msg_lrpid);
    printf("lspid : %d\n", infos_bal.msg_lspid);
    printf("qbytes : %ld\n", infos_bal.msg_qbytes);
    printf("qnum : %ld\n", infos_bal.msg_qnum);
    printf("rtime : %ld\n", infos_bal.msg_rtime);
    printf("stime : %ld\n", infos_bal.msg_stime);
}

int ecrire_bal(int id_bal, void *message, size_t message_size)
{
    if (id_bal == -1)
    {
        printf("Erreur : Impossible d'écrire dans une bal ayant comme id -1\n");
        return -1;
    }

    if (message == NULL)
    {
        printf("Erreur : Impossible d'écrire un message NULL\n");
        return -1;
    }

    int res = msgsnd(id_bal, message, message_size, 0);
    if (res == -1)
    {
        perror("Erreur perror");
        return -1;
    }
    return 0;
}

int lire_bal(int id_bal, void *message_lu, size_t message_size, long type)
{
    if (id_bal == -1)
    {
        printf("Erreur : Impossible d'écrire dans une bal ayant comme id -1\n");
        return -1;
    }

    int res = msgrcv(id_bal, message_lu, message_size, type, 0);
    if (res == -1)
    {
        perror("Erreur perror");
        return -1;
    }
    return 0;
}