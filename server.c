#include "include/header.h"

int checkeConnected(int * tabSocket) {
    for(int i=0;i<NB_PLAYER;i++){
        
    }
}

int main(int argc, char ** argv){
    int se, sd,svcLen,ret,status;
    struct sockaddr_in svc, clt;
    socklen_t cltLen;
    pid_t pid;
    fd_set rfsd; //set de canaux à multiplexer
    int socketPlayer[NB_PLAYER] = "";

    // Création de la socket de réception d’écoute des appels
    CHECK(se=socket(PF_INET, SOCK_STREAM, 0), "Can't create");
    puts("Création socket écoute");

    // Préparation de l’adressage du service (d’appel)
    svc.sin_family = PF_INET;
    svc.sin_port = htons(0);
    svc.sin_addr.s_addr = INADDR_ANY;
    memset(&svc.sin_zero, 0, 8);
    
    // Association de l’adressage préparé avec la socket d’écoute
    CHECK(bind(se,(struct sockaddr *) &svc, sizeof svc), "Can't bind");
    puts("Bind socket écoute");
    
    svcLen = sizeof(svc);
    CHECK(getsockname(se,(struct sockaddr *) &svc,&svcLen),"test");
    printf("port : %d\n",ntohs(svc.sin_port));
    printf("address : %s\n",inet_ntoa(svc.sin_addr));
    
    // Mise en écoute de la socket
    CHECK(listen(se, NB_PLAYER) , "Can't calibrate");
    puts("Mise en écoute socket écoute");

    FD_ZERO(&rfsd);
    FD_SET(se,&rfsd);

    while(1){
        if(FD_ISSET(se,&rfsd)){
            // Attente d’un appel
            cltLen = sizeof(clt);
            CHECK(sd=accept(se,(struct sockaddr *) &clt, &cltLen),"Can't connect");
            FD_SET(sd,&rfsd);
        } 
    }
}