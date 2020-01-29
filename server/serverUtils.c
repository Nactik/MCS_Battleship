#include "serverUtils.h"

void printLobby(Sock sd){
    int i,sock;
    char line[MAX_BUFF];
    char recv[MAX_BUFF];
    int reqCode;
    int code;
    sock = sd.socket;
    for(i=0;i<server.nb;i++){
        if(server.tabLobby[i].affected == 1){
            sprintf(line,"%d:%d:%s:%s:%d",HAS_LOBBY,server.tabLobby[i].numero,server.tabLobby[i].nom,
                    server.tabLobby[i].owner.pseudo,server.tabLobby[i].nb_joueur);
            // getchar();
            write(sock,line,strlen(line)+1);
            CHECK(read(sock,recv,sizeof(recv)),"erreur read");
        }
    }

    sprintf(line,"%d:::",END_LOBBY);
    write(sock,line,strlen(line)+1);
    CHECK(read(sock,recv,sizeof(recv)),"erreur read");
}

int connectToServer(Sock * sd,char * buffer){
    char pseudo[MAX_PLAYER_NAME];
    //On recupère le pseudo
    sscanf (buffer, "%s",pseudo);
    //On l'associe à la socket
    strcpy(sd->client.pseudo,pseudo);
    return 1;
}

void connectToLobby(Sock sd, char * buffer){
    int lobby;
    char line[MAX_BUFF];
    //On recupère le num du lobby
    sscanf(buffer,"%d",&lobby);

    //On check si le num de lobby est valide
    if(lobby < 0 || lobby >= server.nb){ 
        sprintf(line,"%d:Numéro de lobby invalide, veuillez réessayer",ERREUR);
        CHECK(write(sd.socket,line,strlen(line)+1),"Can't write");
    }else {
        char * ip = server.tabLobby[lobby].ip;
        int port = server.tabLobby[lobby].port;
        //On check si le lobby est plein ou non
        if(server.tabLobby[lobby].nb_joueur == 2){
            //On prepare la requete pour spectate
            sprintf(line,"%d:%s:%d",SPECT_LOB,ip,port);
        } else {
            //On prepare la requete pour se connecter
            sprintf(line,"%d:%s:%d",CONNECT_LOB_OK,ip,port);
            server.tabLobby[lobby].nb_joueur ++;
        }
        CHECK(write(sd.socket,line,strlen(line)+1),"Can't write");
    }
}

int createLobby(Sock sd, char * buffer){
    char lobbyName[MAX_NAME_LOBBY];
    char ip[MAX_LENGTH_IP];
    int port;
    struct sockaddr_in clt;

    int cltLen = sizeof(clt);

    //On recupère l'ip du client
    CHECK(getsockname(sd.socket,(struct sockaddr *) &clt,&cltLen),"test");
    strcpy(ip,inet_ntoa(clt.sin_addr));
    
    //printf("%s\n",buffer);
    //On parse la requete pour recupèrer le nom du lobby et le port
    sscanf (buffer, "%[^:]:%d",lobbyName,&port);
    //printf("Lobby à créer : %s, %s, %d\n",lobbyName,ip,port);
    //printf("Owner : %s\n",sd.client.pseudo);

    return addLobby(sd.client.pseudo,lobbyName,ip,port);
}

int deleteLobby(char * owner){
    for(int i=0;i<MAX_LOBBY;i++){
        //On cherche le lobby du joueur
        if(strcmp(owner,server.tabLobby[i].owner.pseudo) == 0){
            //On met le flag affected à 0 pour signaler qu'on peut reaffecter le lobby
            server.tabLobby[i].affected = 0;
            return 1;
        }
    }
    return 0;
}

int addLobby(char * owner,char * lobbyName,char * ip, int port){
    //puts("Ajout d'un lobby ...");
    for(int i=0;i<MAX_LOBBY;i++){
        //printf("%d\n",server.tabLobby[i].affected);
        //On cherche un lobby reaffectable
        if(server.tabLobby[i].affected == 0){
            server.tabLobby[i].numero=i;
            strcpy(server.tabLobby[i].nom,lobbyName);
            strcpy(server.tabLobby[i].ip,ip);   
            server.tabLobby[i].port = port;
            server.tabLobby[i].nb_joueur = 1;
            server.tabLobby[i].affected = 1;
            strcpy(server.tabLobby[i].owner.pseudo,owner);
            server.nb++;
            return 1;
        }
    }
    //puts("... lobby créé");
    return 0;
}  
