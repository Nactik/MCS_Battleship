#define CHECK(sts,msg) if ((sts) == -1) {perror(msg);exit(-1);}
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include <sys/select.h>
#define NB_PLAYER 20
#define MAX_BUFF 1024
char buffer[MAX_BUFF];
#define CONNECT_SRV     "100:Demande de connection au serveur"
#define CREATE_LOB      "200:Crée un salon de jeu"
#define CONNECT_LOB     "300:Demande de connexion à un salon de jeu"    
#define ATTACK          "400:Attaque le plateau ennemie à une position donnée"
#define CONNECT_SRV_OK  "102:Connexion au serveur acceptée"
#define CONNECT_LOB_OK  "302:Connexion à une salle acceptée"
#define RESULT_ATK      "500:Résultat d'une attaque d'un joueur"
#define ERREUR          "KO:Erreur survenue"
