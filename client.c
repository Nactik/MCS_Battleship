#include "include/header.h"

void dialogueClt (int sd, struct sockaddr_in clt) {
    char requete[MAX_BUFF];
    do {
        read(sd, buffer, sizeof(buffer));
        sscanf (buffer, "%s:%s",requete, buffer);
        switch (atoi(requete)) {
            case 0 : 
                printf("Au revoir\n"); 
                break;
            case 100 : 
                write(sd, OK, strlen(OK)+1);
                printf("OK : message recu %s\n", buffer);
                break;
            default : 
                write(sd, NOK, strlen(NOK)+1);
                printf("NOK : message recu %s\n", buffer); 
                break;
        }
    } while(atoi(requete) != 0);
} 

void deroute(int signal){
    int ret;
    switch(signal){
        case SIGCHLD :
            wait(&ret);
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
    
    //Dialogue avec le serveur
    dialogueSrv(sock, svc);

    shutdown(sock,SHUT_RDWR); 
    
    return 0;
}