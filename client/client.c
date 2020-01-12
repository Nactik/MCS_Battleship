#include "clientUtils.h"
int port; 
char ipAddr[15]; 

void dialogueSrv (int sock, struct sockaddr_in srv) {
    
    int choix, numLobby; 
    char msgToSend[MAX_BUFF], msgToRead[MAX_BUFF], numReq[MAX_BUFF]; 
    char pseudo[MAX_PLAYER_NAME], buffer[100]; 

    //Message de bienvenu
    CHECK(read(sock, msgToRead, sizeof(msgToRead)), "Can't read");
    printf("%s", msgToRead);  

    //Entrée des infos du joueur et envoi au serveur(pseudo, ...)
    printf("Veuillez entrer votre nom d'utilisateur (MAX: %d caractères):\t", NB_MAX_PLAYER); 
    scanf("%s", pseudo); 
    sprintf(msgToSend, "%d:%s",CONNECT_SRV, pseudo); 
    CHECK(write(sock, msgToSend, strlen(msgToSend)+1), "Can't send");

    //Attente de la réponse serveur et  affichage des salons disponibles envoyés par le serveur
    CHECK(read(sock, msgToRead, sizeof(msgToRead)), "Can't read");
    if(strcmp(msgToRead, "KO")){ //Pb survenu, on quitte 
        printf("Problème survenu\n");
        return;  
    }
    
    choix = displayMenu(); //Affichage du menu + choix 

    switch (choix)
    {
        case 1: //Afficher les lobbys dispo
            sprintf(msgToSend, "%d", LIST_LOB, pseudo); 
            CHECK(write(sock, msgToSend, strlen(msgToSend)+1), "Can't send");
            do {
                CHECK(read(sock, msgToRead, sizeof(msgToRead)), "Can't read");
                sscanf (msgToRead, "%s:%s", numReq, msgToRead);
                switch (atoi(numReq)) {
                    case 105 : //num a changer par variable 
                        printf("%s", msgToRead); 
                        break;
                }
            } while ( atoi(numReq) != 106); //num a changer par variable
        break; 
        case 2: //Créer un lobby
            printf("Veuillez indiquer un nom de salle:"); 
            scanf("%s", buffer); 
            sprintf(msgToSend, "%d:%d:%s:%d", 200, buffer, ipAddr, port); 
            CHECK(write(sock, msgToSend, strlen(msgToSend)+1), "Can't write"); //On envoie la req
            break;
        case 3: //Jouer sur un lobby existant ou etre spectateur sur une partie en cours
        case 4: 
            printf("Veuillez indiquer le numéro de sur lequel jouer:"); 
            scanf("%d", numLobby);
            sprintf(msgToSend, "%d:%d", 300, numLobby); 
            CHECK(write(sock, msgToSend, strlen(msgToSend)+1), "Can't write"); //On envoie la req
            break; 
        case 5: //Quitter 
            return; 
            break;
    
        default:
            break;
    }


}  

int main(int argc, char ** argv){
    
    int sock, cltlen;
    struct sockaddr_in svc;
    struct sockaddr_in clt;

    if(argc != 3){
        printf("Erreur, rentrez les bons arguments"); 
        return 0; 
    }
    
    //Création de la socket d’appel et de dialogue
    printf("Création de la socket ...\n"); 
    CHECK(sock=socket(PF_INET, SOCK_STREAM, 0), "Can't create");
    getchar(); 
    
    //Préparation de l’adressage du service à contacte
    svc.sin_family = PF_INET;
    svc.sin_port = htons(atoi(argv[2]));  
    svc.sin_addr.s_addr = inet_addr(argv[1]); 
    memset(&svc.sin_zero, 0, 8);
    
    //Demande d’une connexion au service
    printf("Demande de connexion au serveur ...\n"); 
    CHECK(connect(sock, (struct sockaddr *)&svc, sizeof svc) , "Can't connect");
    getchar(); 

    
    //Récupération des data socket côté client
    cltlen = sizeof(clt); 
    CHECK(getsockname(sock, (struct sockaddr *) &clt, &cltlen), "Can't get sockname"); 
    printf("Client -> [ %s ] utilise le port %d pour communiquer.\n\n", inet_ntoa(clt.sin_addr), ntohs(clt.sin_port)); 
    strncpy(ipAddr, inet_ntoa(clt.sin_addr), 15); 
    port= ntohs(clt.sin_port); 
    
    //Dialogue avec le serveur
    dialogueSrv(sock, svc);
    
    close(sock);
    
    return 0;
}