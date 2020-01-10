#include "serverUtils.h"

void dispLobby(int sd){
    char * menu;
    write(sd,tabLobby);
}

int connectToServer(char * buffer){
    char pseudo[MAX_PLAYER_NAME];
    sscanf (buffer, "%s",pseudo);
    return addPlayer(pseudo);
}

void createLobby(char * buffer){
    char lobbyName[MAX_NAME_LOBBY];
    char ip[MAX_LENGTH_IP];
    int port;
    scanf(buffer,"%s:%s:%d",lobbyName,ip,port);
    return addLobby(lobbyName,ip,port);
}

int addPlayer(char * pseudo){
    int oldMemSize = sizeof(tabJoueur);

    Joueur newPlayer;
    strcpy(newPlayer.pseudo,pseudo);
    int length  = sizeof(tabJoueur) / sizeof(Joueur);
    tabJoueur = (Joueur *) realloc(tabJoueur, oldMemSize + sizeof(newPlayer));
    tabJoueur[length] = newPlayer;

    return 1;
}  

int addLobby(char * lobbyName,char * ip, int port){
    int oldMemSize = sizeof(tabLobby);

    Lobby newLobby;
    strcpy(newLobby.nom,lobbyName);
    strcpy(newLobby.ip,ip);
    newLobby.port = port;
    newLobby.nb_joueur = 1;

    int length  = sizeof(tabLobby) / sizeof(Lobby);
    tabLobby = (Lobby *) realloc(tabLobby, oldMemSize + sizeof(newLobby));
    newLobby.numero = length;
    tabLobby[length] = newLobby;

    return 1;
}  
