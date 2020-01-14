#include "clientUtils.h"

int displayMenu(){
    int choix=0; 
    do
    {
        printf("\t----- MENU JEU BATAILLE NAVALE-----\n"); 
        printf("\t\t 1) Lister les lobby \n"); 
        printf("\t\t 2) Créer un lobby \n"); 
        printf("\t\t 3) Jouer sur un lobby \n"); 
        printf("\t\t 4) Etre spectateur sur un lobby \n"); 
        printf("\t\t 5) Quitter \n"); 
        scanf("%d", &choix); 
    } while (choix>5||choix<1);
    
    return choix;
}

void printLobby(int sock){
    int numReq;
    char msgToSend[MAX_BUFF];
    char msgToRead[MAX_BUFF];
    int numLobby;
    char lobbyName[MAX_NAME_LOBBY];
    int nbPlayer;

    sprintf(msgToSend, "%d", PRINT_LOB); 
    CHECK(write(sock, msgToSend, strlen(msgToSend)+1), "Can't send");

    puts("\t+------+--------------------+----------+");
    printf("\t|%6s|%20s|%10s|\n","Numéro","Nom de la salle","Joueurs");
    puts("\t+------+--------------------+----------+");
    
    do {    
        CHECK(read(sock, msgToRead, sizeof(msgToRead)), "Can't read");
        sscanf (msgToRead, "%d:%d:%[^:]:%d", &numReq, &numLobby,lobbyName,&nbPlayer);
        if(numReq == 105){
            if(nbPlayer == 2){ //on ecrit en rouge si plein sinon en vert
                printf("\t|%6d|%20s|\033[22;31m%8d/2\x1b[0m|\n",numLobby,lobbyName,nbPlayer);
            } else {
                printf("\t|%6d|%20s|\033[22;32m%8d/2\x1b[0m|\n",numLobby,lobbyName,nbPlayer);
            }
        }
        sprintf(msgToSend, "%d", OK); 
        CHECK(write(sock,msgToSend,strlen(msgToSend)+1),"erreur write");
    } while ( numReq != 106); 

    puts("\t+------+--------------------+----------+");
}
