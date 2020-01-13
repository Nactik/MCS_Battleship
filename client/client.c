#include "clientUtils.h"
int port; 
char ipAddr[15]; 

void dialogueSrv (int sock, struct sockaddr_in srv) {
    
    int choix, numLobby,numReq; 
    char msgToSend[MAX_BUFF], msgToRead[MAX_BUFF]; 
    char pseudo[MAX_PLAYER_NAME], buffer[100]; 

    //Message de bienvenu
    CHECK(read(sock, msgToRead, sizeof(msgToRead)), "Can't read");
    printf("%s", msgToRead);  

    //Entrée des infos du joueur et envoi au serveur(pseudo, ...)
    printf("Veuillez entrer votre nom d'utilisateur (MAX: %d caractères):\t", MAX_PLAYER_NAME); 
    scanf("%s", pseudo); 
    sprintf(msgToSend, "%d:%s",CONNECT_SRV, pseudo); 
    CHECK(write(sock, msgToSend, strlen(msgToSend)+1), "Can't send");

    //Attente de la réponse serveur et  affichage des salons disponibles envoyés par le serveur
    /*CHECK(read(sock, msgToRead, sizeof(msgToRead)), "Can't read");
    if(strcmp(msgToRead, "KO")){ //Pb survenu, on quitte 
        printf("Problème survenu\n");
        return;  
    }*/
    
    do{
        choix = displayMenu();
        switch (choix)
        {
            case 1: //Afficher les lobbys dispo
                sprintf(msgToSend, "%d", PRINT_LOB); 
                CHECK(write(sock, msgToSend, strlen(msgToSend)+1), "Can't send");
                do {
                    CHECK(read(sock, msgToRead, sizeof(msgToRead)), "Can't read");
                    sscanf (msgToRead, "%d:%s", &numReq, msgToRead);
                    printf("%s\n", msgToRead); 
                    sprintf(msgToSend, "%d", OK); 
                    CHECK(write(sock,msgToSend,strlen(msgToSend)+1),"erreur write");
                } while ( numReq != 106); //num a changer par variable
                    CHECK(write(sock,msgToSend,strlen(msgToSend)+1),"erreur write");

            break; 
            case 2: //Créer un lobby
                printf("Veuillez indiquer un nom de salle:"); 
                scanf("%s", buffer); 
                sprintf(msgToSend, "%d:%s:%s:%d", 200, buffer, ipAddr, port); 
                CHECK(write(sock, msgToSend, strlen(msgToSend)+1), "Can't write"); //On envoie la req
                break;
            case 3: //Jouer sur un lobby existant ou etre spectateur sur une partie en cours
            case 4: 
                printf("Veuillez indiquer le numéro de sur lequel jouer:"); 
                scanf("%d", &numLobby);
                sprintf(msgToSend, "%d:%d", 300, numLobby); 
                CHECK(write(sock, msgToSend, strlen(msgToSend)+1), "Can't write"); //On envoie la req
                break; 
            case 5: //Quitter 
                return; 
                break;
        
            default:
                break;
        }
    }while(choix != 5); //Affichage du menu + choix 
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
    CHECK(sock=socket(AF_INET, SOCK_STREAM, 0), "Can't create");
    
    //Préparation de l’adressage du service à contacte
    svc.sin_family = AF_INET;
    svc.sin_port = htons(atoi(argv[2]));  
    svc.sin_addr.s_addr = inet_addr(argv[1]); 
    memset(&svc.sin_zero, 0, 8);
    
    //Demande d’une connexion au service
    printf("Demande de connexion au serveur ...\n"); 
    CHECK(connect(sock, (struct sockaddr *)&svc, sizeof svc) , "Can't connect");

    
    //Récupération des data socket côté client
    cltlen = sizeof(clt); 
    CHECK(getsockname(sock, (struct sockaddr *) &clt, &cltlen), "Can't get sockname"); 
    printf("Client -> [ %s ] utilise le port %d pour communiquer.\n\n", inet_ntoa(clt.sin_addr), ntohs(clt.sin_port)); 
    strncpy(ipAddr, inet_ntoa(clt.sin_addr), 15); 
    port= ntohs(clt.sin_port); 
    
    //Dialogue avec le serveur
    dialogueSrv(sock, svc);
    
  
    shutdown(sock,2);
    
    return 0;
}