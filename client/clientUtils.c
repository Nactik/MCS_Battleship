#include "clientUtils.h"

int displayMenu(){
    int choix=0; 
    puts("");
    puts("");
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
    char owner[MAX_PLAYER_NAME];
    int nbPlayer;

    sprintf(msgToSend, "%d", PRINT_LOB); 
    CHECK(write(sock, msgToSend, strlen(msgToSend)+1), "Can't send");

    puts("\t+------+-------------------------+-------------------------+----------+");
    printf("\t|%6s|%25s|%26s|%10s|\n","Numéro","Nom de la salle","Hébergeur","Joueurs");
    puts("\t+------+-------------------------+-------------------------+----------+");
    
    do {    
        CHECK(read(sock, msgToRead, sizeof(msgToRead)), "Can't read");
        sscanf (msgToRead, "%d:%d:%[^:]:%[^:]:%d", &numReq, &numLobby,lobbyName,owner,&nbPlayer);
        if(numReq == HAS_LOBBY){
            if(nbPlayer == 2){ //on ecrit en rouge si plein sinon en vert
                printf("\t|%6d|%25s|%25s|\033[22;31m%8d/2\x1b[0m|\n",numLobby,lobbyName,owner,nbPlayer);
            } else {
                printf("\t|%6d|%25s|%25s|\033[22;32m%8d/2\x1b[0m|\n",numLobby,lobbyName,owner,nbPlayer);
            }
        }
        sprintf(msgToSend, "%d", OK); 
        CHECK(write(sock,msgToSend,strlen(msgToSend)+1),"erreur write");
    } while ( numReq != END_LOBBY); 

    puts("\t+------+-------------------------+-------------------------+----------+");
}

void createLobby(int sock_server, int * sock_lobby){
    int sockLen; 
    void * ret;
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

    // Mise en écoute de la socket
    CHECK(listen(*sock_lobby, 1) , "Can't calibrate");
    puts("Mise en écoute socket écoute"); 
    mutex = sem_open("/mutex",O_CREAT,666,1);
    sem_wait(mutex);
    pthread_t monThread = pthread_create(&monThread,NULL,waitPlayer,(void *) sock_lobby);
    pthread_join(monThread,&ret);
    sem_wait(mutex);
    CHECK(sem_close(mutex),"Erreur destruction mutex"); //On detruit la mutex
    CHECK(sem_unlink("/mutex"),"Erreur unlink mutex"); //On unlink la mutex
}

void * waitPlayer(void * arg){

    int sd;
    int * sock_lobby = (int *) arg;
    struct sockaddr_in clt;
    socklen_t cltLen;

    puts("\t\t En attente de joueurs....");
    CHECK(sd=accept(*sock_lobby,(struct sockaddr *) &clt, &cltLen),"Can't connect");
    printf("New connection , socket fd is %d , ip is : %s , port : %d\n",
                    sd, inet_ntoa(clt.sin_addr) , ntohs(clt.sin_port));   
    startGame(sd,1);
    shutdown(*sock_lobby,2);
    sem_post(mutex);
    pthread_exit(0);
}

void connectToLobby(int sock ){
    char msgToSend[MAX_BUFF],content[MAX_BUFF],msgToRead[MAX_BUFF], ip[MAX_LENGTH_IP];
    int port,req,numLobby,sockLobby;
    struct sockaddr_in svc;

    printf("Veuillez indiquer le numéro de sur lequel jouer:"); 
    scanf("%d", &numLobby);
    sprintf(msgToSend, "%d:%d", CONNECT_LOB, numLobby); 
    CHECK(write(sock, msgToSend, strlen(msgToSend)+1), "Can't write"); //On envoie la req
    CHECK(read(sock,msgToRead,sizeof(msgToRead)),"Can't Read");

    sscanf (msgToRead, "%d:%[^:]",&req,content);
    if(req ==CONNECT_LOB_OK){
        sscanf (content, "%[^:]:%d",ip,&port);
        printf("--> %s", content); 

        printf("Création de la socket ...\n"); 
        CHECK(sockLobby=socket(AF_INET, SOCK_STREAM, 0), "Can't create");
        
        //Préparation de l’adressage du service à contacte
        svc.sin_family = AF_INET;
        svc.sin_port = htons(port);  
        svc.sin_addr.s_addr = inet_addr(ip); 
        memset(&svc.sin_zero, 0, 8);
        
        //Demande d’une connexion au service
        printf("Demande de connexion au serveur [%s] - [%d]...\n", ip, port); 
        CHECK(connect(sockLobby, (struct sockaddr *)&svc, sizeof svc) , "Can't connect");

        startGame(sockLobby,2);
        shutdown(sockLobby,2);
    } else if(req == ERREUR){
        printf("\033[22;31m%s\x1b[0m \n\n",content);
    }
    

}