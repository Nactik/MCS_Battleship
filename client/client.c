#include "clientUtils.h"

int dialogueSrv (int sock, struct sockaddr_in srv, int * sock_lobby) {
    
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
    CHECK(read(sock, msgToRead, sizeof(msgToRead)), "Can't read");
    printf("%s\n",msgToRead);

    do{
        choix = displayMenu();
        switch (choix)
        {
            case 1: //Afficher les lobbys dispo
                printLobby(sock);
            break; 
            case 2: //Créer un lobby
                createLobby(sock, sock_lobby); 
                break;
            case 3: //Jouer sur un lobby existant ou etre spectateur sur une partie en cours
                connectToLobby(sock);
            case 4: 
                break; 
            default:
                sprintf(msgToSend, "%d", DISCONNECT); 
                CHECK(write(sock, msgToSend, strlen(msgToSend)+1), "Can't write"); //On envoie la req
                CHECK(read(sock, msgToRead, sizeof(msgToRead)), "Can't read");
                printf("%s\n\n",msgToRead);
                break;
        }
    }while(choix!=5);
}  

int main(int argc, char ** argv){
    
    int sock, cltlen, sock_lobby;
    struct sockaddr_in svc;
    struct sockaddr_in clt;

    if(argc != 3){
        printf("Erreur, rentrez les bons arguments\n"); 
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
    
    //Dialogue avec le serveur
    dialogueSrv(sock, svc, &sock_lobby);
    shutdown(sock_lobby, 2);
    shutdown(sock,2);
    
    return 0;
}