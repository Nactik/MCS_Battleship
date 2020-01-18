#include "serverUtils.h"

void printLobby(int sd){
    int i;
    char line[MAX_BUFF];
    char recv[MAX_BUFF];
    int reqCode;
    int code;
    for(i=0;i<server.nb;i++){
        sprintf(line,"%d:%d:%s:%d",HAS_LOBBY,server.tabLobby[i].numero,server.tabLobby[i].nom,server.tabLobby[i].nb_joueur);
        // getchar();
        write(sd,line,strlen(line)+1);
        CHECK(read(sd,recv,sizeof(recv)),"erreur read");
    }

    sprintf(line,"%d:::",END_LOBBY);
    write(sd,line,strlen(line)+1);
    CHECK(read(sd,recv,sizeof(recv)),"erreur read");
}

int connectToServer(char * buffer){
    char pseudo[MAX_PLAYER_NAME];
    sscanf (buffer, "%s",pseudo);
    return addPlayer(pseudo);
}

int connectToLobby(int sd, char * buffer){
    int lobby;
    char line[MAX_BUFF];
    sscanf(buffer,"%d",&lobby);

    if(lobby < 0 || lobby >= server.nb){
        sprintf(line,"%d:Numéro de lobby invalide, veuillez réessayer",ERREUR);
        write(sd,line,strlen(line)+1);
    } else {
        char * ip = server.tabLobby[lobby].ip;
        int port = server.tabLobby[lobby].port;
        sprintf(line,"%d:%s:%d",CONNECT_LOB_OK,ip,port);
        write(sd,line,strlen(line)+1);
    }

    return 1;
}


int createLobby(int sd, char * buffer){
    char lobbyName[MAX_NAME_LOBBY];
    char ip[MAX_LENGTH_IP];
    int port;
    struct sockaddr_in clt;

    int cltLen = sizeof(clt);
    CHECK(getsockname(sd,(struct sockaddr *) &clt,&cltLen),"test");
    strcpy(ip,inet_ntoa(clt.sin_addr));
    
    printf("%s\n",buffer);
    sscanf (buffer, "%[^:]:%d",lobbyName,&port);
    printf("Lobby à créer : %s, %s, %d\n",lobbyName,ip,port);
    return addLobby(lobbyName,ip,port);
}

int addPlayer(char * pseudo){
   /* int oldMemSize = sizeof(tabJoueur);

    Joueur newPlayer;
    strcpy(newPlayer.pseudo,pseudo);
    int length  = sizeof(tabJoueur) / sizeof(Joueur);
    tabJoueur = (Joueur *) realloc(tabJoueur, oldMemSize + sizeof(newPlayer));
    tabJoueur[length] = newPlayer;*/

    return 1;
}  

int addLobby(char * lobbyName,char * ip, int port){
    puts("Ajout d'un lobby ...");

    int oldMemSize = (server.tabLobby == NULL) ? 0 : sizeof(server.tabLobby); 
    if(oldMemSize == 0)
        server.nb = 0;

    Lobby newLobby;
    strcpy(newLobby.nom,lobbyName);
    strcpy(newLobby.ip,ip);   
    newLobby.port = port;
    newLobby.nb_joueur = 1;

    server.tabLobby = (Lobby *) realloc(server.tabLobby, oldMemSize + sizeof(newLobby));
    newLobby.numero = server.nb;
    server.tabLobby[server.nb++] = newLobby;
    puts("... lobby créé");
    return 1;
}  
