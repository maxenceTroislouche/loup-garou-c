/*Le client doit : 
-Appeler la fonction TCHAT
-Un thread boucle et permet la saisie lorsque la période TCHAT_village est TRUE
-La saisie est envoyé par le thread dans la boite aux lettres
-un thread s'occupe de la boucle de lecture des messages de la boite aux lettres
-Les boucles se terminent quand TCHAT_village passe FALSE
-La nuit commence au bout de 60s de jour
-Lors de la nuit les loups garous peuvent discuter via le TCHAT_nuit
-La nuit les loups garous votent en envoyant dans la boite aux lettres leur PID et Nom du joueur pour lequel ils votent
-Le client attend un retour du serveur dans la boite aux lettres
-Le jour se lève au bout de 30s de nuit
-Appeler la fonction TCHAT et recommencer jusqu'à ce que le serveur signale une fin de partie */

#ifndef
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <pthread.h>
#include <stdbool.h>
#define type_loup_garou
#define type_villageois
#define type_all_joueurs


struct msgbuf {
    long mtype;
    char mtext[100];
};

int msgctl(int msqid, int cmd, struct msqid_ds *buf);
int msgget(key_t key, int msgflg);
ssize_t msgrcv(int msqid, void *msgp, size_t msgsz, long msgtyp, int msgflg);
int msgsnd(int msqid, const void *msgp, size_t msgsz, int msgflg);
void *TCHAT_village();
void *TCHAT_nuit();
void thread_temps();
bool tchat_village = false;
bool tchat_nuit = false;

void main ()
{
    int msgid;
    struct msgbuf buf;
    struct msqid_ds *buf2;
    msgid = msgget(0x1234, 0666); //Ouverture de la boîte aux lettres
    pthread_t thread_temps;
    pthread_create(&thread_temps, NULL, thread_temps, NULL);
    pthread_join(thread_temps, NULL);
    //Partie Connexion à l'hôte
    bool connexion_OK = false;
        do
        {
            //Saisie de l'ID de la boite aux lettres pour synchro avec hôte
            printf("saisir l'ID de la boite aux lettres de l'hôte (PID) : ");
            scanf("%d", &msgid);
            //Envoi de l'ID de la boite aux lettres au serveur
            buf.mtype = 2;
            //Envoi Message demande connexion au serveur
            printf("Envoi Message demande connexion au serveur");
            msgsnd(msgid, &buf, sizeof(buf), 0);
            //Attente de la réponse du serveur
            msgrcv(msgid, &buf, sizeof(buf), 2, 0);
            //Analyse de la réponse du serveur
            if (buf.mtext == "Connexion_OK")
            {
                connexion_OK = true;
            }
            else
            {
                printf("Connexion refusée");
            }   
                if(connexion_OK)
                {
                int PID_client=getpid();
                char Nom_client[100];
                printf("Entrez votre nom de joueur : ");
                scanf("%s", Nom_client);
                sprintf(buf.mtext, "%d %s", PID_client, Nom_client);
                buf.mtype = type_all_joueurs;
                msgsnd(msgid, &buf, sizeof(buf.mtext), 0);
                }
                else printf("Erreur connexion à l'hôte");
                }
            while(!connexion_OK);
           // On attend le début de la partie
           char reponse[100];
           do
           { 
           reponse = msgrcv(msgid, &buf, sizeof(buf), 2, 0);
            if (buf.mtext == "Debut_partie")
            {
                tchat_village = true;
            }
            else
            {
                printf("Partie en Attente");
            }
           }while(reponse != "START");
              //On lance le TCHAT
            // On commence la partie
        while (signal_fin_partie !=0){ //Tant que la partie n'est pas finie)
            if (tchat_village == true) {
                pthread_t thread1;
                pthread_create(&thread1, NULL, TCHAT_village, NULL); //Création du thread de saisie Tchat village car tchat_village = true
                pthread_join(thread1, NULL);
            }
            if (tchat_nuit == true) {
                pthread_t thread2;
                pthread_create(&thread2, NULL, TCHAT_nuit, NULL); //Création du thread de saisie Tchat nuit car tchat_nuit = true
                pthread_join(thread2, NULL);
            }
        }
}

void *TCHAT_village()
{
    int msgid;
    struct msgbuf buf;
    struct msqid_ds *buf2;
    msgid = msgget(0x1234, 0666); //Ouverture de la boîte aux lettres
    if (msgid == -1) {
        perror("msgget");
        exit(1);
    }
    while (tchat_village == true) {
        printf("Saisir un message : ");
        scanf("%s", buf.mtext);
        buf.mtype = type_all_joueurs; //Type de message tous les joueurs car c'est le TCHAT village
        if (msgsnd(msgid, &buf, sizeof(buf.mtext), 0) == -1) {
            perror("msgsnd");
            exit(1);
        }

    }
    pthread_exit(NULL); //Fin du thread + Passage à la nuit
}

void *TCHAT_nuit()
{
    int msgid;
    struct msgbuf buf;
    struct msqid_ds *buf2;
    msgid = msgget(0x1234, 0666); //Ouverture de la boîte aux lettres
    if (msgid == -1) {
        perror("msgget");
        exit(1);
    }
    while (tchat_nuit == true) {
        printf("Saisir un message : ");
        scanf("%s", buf.mtext);
        buf.mtype = type_loup_garou ; //type de message loup garou car c'est le TCHAT nuit
        if (msgsnd(msgid, &buf, sizeof(buf.mtext), 0) == -1) {
            perror("msgsnd");
            exit(1);
        }
    }
    pthread_exit(NULL); // Fin du thread + Passage au jour
}

void *thread_temps() // Jour/nuit
{
    while (signal_fin_partie=!0)
    {
        tchat_village = true;
        sleep(60);
        tchat_village = false;
        tchat_nuit = true;
        sleep(30);
        tchat_nuit = false;
    }
    pthread_exit(NULL);
}

#endif