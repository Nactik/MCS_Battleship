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
        if(numReq == HAS_LOBBY){
            if(nbPlayer == 2){ //on ecrit en rouge si plein sinon en vert
                printf("\t|%6d|%20s|\033[22;31m%8d/2\x1b[0m|\n",numLobby,lobbyName,nbPlayer);
            } else {
                printf("\t|%6d|%20s|\033[22;32m%8d/2\x1b[0m|\n",numLobby,lobbyName,nbPlayer);
            }
        }
        sprintf(msgToSend, "%d", OK); 
        CHECK(write(sock,msgToSend,strlen(msgToSend)+1),"erreur write");
    } while ( numReq != END_LOBBY); 

    puts("\t+------+--------------------+----------+");
}

void createLobby(int sock_server, int * sock_lobby){
    
    int sockLen; 
    char buffer[MAX_BUFF], msgToSend[MAX_BUFF];
    struct sockaddr_in svc_lobby;

    printf("Veuillez indiquer un nom de salle: "); 
    scanf("%s", buffer); 
    CHECK(*sock_lobby=socket(AF_INET, SOCK_STREAM, 0), "Can't create");
    
    //Préparation de l’adressage du service à contacte
    svc_lobby.sin_family = AF_INET;
    svc_lobby.sin_port = htons(0); //Donne port aléa  
    svc_lobby.sin_addr.s_addr = INADDR_ANY; 
    memset(&svc_lobby.sin_zero, 0, 8);

    //Mise en place du bind
    CHECK(bind(*sock_lobby,(struct sockaddr *) &svc_lobby, sizeof svc_lobby), "Can't bind");

    //Récupération des data socket côté client
    sockLen = sizeof(svc_lobby); 
    CHECK(getsockname(*sock_lobby, (struct sockaddr *) &svc_lobby, &sockLen), "Can't get sockname"); 
    sprintf(msgToSend, "%d:%s:%d", CREATE_LOB, buffer,  ntohs(svc_lobby.sin_port)); 
    CHECK(write(sock_server, msgToSend, strlen(msgToSend)+1), "Can't write"); //On envoie la req
}