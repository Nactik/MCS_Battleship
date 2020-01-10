#include "server/serverUtils.h"

void dialogueClt (int sd, struct sockaddr_in clt) {
    char requete[MAX_BUFF];
    char toSend[MAX_BUFF];
    write(sd,WELCOME,strlen(WELCOME)+1);

    do {
        read(sd, buffer, sizeof(buffer));
        sscanf (buffer, "%s:%s",requete, buffer);
        switch (atoi(requete)) {
            case CONNECT_SRV : 
                if(connectToServer(buffer) == 1){
                    sprintf(toSend,"%d,%s",CONNECT_SRV_OK,"Salle crée !");
                    write(sd,toSend,toSend+1);
                } else {
                    write(sd,ERREUR,ERREUR+1);
                }
                break;
                
            case CREATE_LOB : 
                createLobby(buffer);
                break;
            case CONNECT_LOB: 

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
    int se, sd,svcLen,ret,status;
    struct sockaddr_in svc, clt;
    socklen_t cltLen;
    pid_t pid;
    Lobby * tabLobby;

    signal(SIGCHLD,deroute);
    
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

    // Boucle permanente de service
    while (1) {
        // Attente d’un appel
        cltLen = sizeof(clt);
        CHECK(sd=accept(se,(struct sockaddr *) &clt, &cltLen),"Can't connect");
        puts("Attente socket écoute et création socket discussion");
    
        switch(pid = fork()){
            case -1 : 
                perror("erreur fork");
                break;
            case 0 :
                // Dialogue avec le client
                dialogueClt(sd,clt);
                shutdown(sd,2);
                kill(SIGCHLD,getppid());
                exit(0);
                break;
        };
    }
}