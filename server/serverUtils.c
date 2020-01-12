#include "serverUtils.h"

void printLobby(int sd, Server server){
    char line[MAX_BUFF];
    puts("coucou");
    sprintf(line,"%d:+------+--------------------+----------+\n",105);
    printf("%s",line);
    getchar();
    write(sd,line,strlen(line)+1);
    sprintf(line,"%d:|%7s|%20s|%10s|\n",105,"Numéro","Nom de la salle","Joueurs");
    printf("%s",line);
    getchar();

    write(sd,line,strlen(line)+1);
    sprintf(line,"%d:+------+--------------------+----------+\n",105);
    printf("%s",line);
    getchar();

    write(sd,line,strlen(line)+1);
    getchar();

    for(int i=0;i<server.nb;i++){
        sprintf(line,"%d:|%7d|%20s|%8d/2|\n",105,server.tabLobby[i].numero,server.tabLobby[i].nom,server.tabLobby[i].nb_joueur);
        printf("%s",line);
        getchar();

        write(sd,line,strlen(line)+1);
    }

    sprintf(line,"%d:+------+--------------------+----------+\n",105);
    printf("%s",line);
    getchar();

    write(sd,line,strlen(line)+1);
    sprintf(line,"%d:\n",106);
    printf("%s",line);
    getchar();

    write(sd,line,strlen(line)+1);
}

int connectToServer(char * buffer){
    char pseudo[MAX_PLAYER_NAME];
    sscanf (buffer, "%s",pseudo);
    return addPlayer(pseudo);
}

int connectToLobby(Server server,int sd, char * buffer){
    int lobby;
    char line[MAX_BUFF];
    sscanf(buffer,"%d",&lobby);

    if(lobby < 0 || lobby >= server.nb){
        sprintf(line,"Numéro de lobby invalide, veuillez réessayer");
        write(sd,line,strlen(line)+1);
    } else {
        char * ip = server.tabLobby[lobby].ip;
        int port = server.tabLobby[lobby].port;
        sprintf(line,"%d:%s:%d",CONNECT_LOB_OK,ip,port);
        write(sd,line,strlen(line)+1);
    }

    return 1;
}


int createLobby(Server * server, char * buffer){
    char lobbyName[MAX_NAME_LOBBY];
    char ip[MAX_LENGTH_IP];
    int port;
    printf("%s\n",buffer);
    sscanf(buffer,"%s:%s:%d",lobbyName,ip,&port);
    printf("Lobby à créer : %s, %s, %d\n",lobbyName,ip,port);
    return addLobby(server,lobbyName,ip,port);
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

int addLobby(Server * server,char * lobbyName,char * ip, int port){
    puts("On veut ajouter un lobby");

    int oldMemSize = (server->tabLobby == NULL) ? 0 : sizeof(server->tabLobby);
    if(oldMemSize == 0)
        server->nb = 0;

    Lobby newLobby;
    strcpy(newLobby.nom,lobbyName);
    strcpy(newLobby.ip,ip);
    newLobby.port = port;
    newLobby.nb_joueur = 1;

    server->tabLobby = (Lobby *) realloc(server->tabLobby, oldMemSize + sizeof(newLobby));
    newLobby.numero = server->nb;
    server->tabLobby[server->nb++] = newLobby;
    puts("normalement c'est bon mdr");
    return 1;
}  
