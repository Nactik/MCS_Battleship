#include "../include/header.h"
#define MAX_NAME_LOBBY 25
#define WELCOME "Bienvenue sur la bataille navale ! "

typedef struct Joueur{
    char pseudo[MAX_PLAYER_NAME];
} Joueur;

typedef struct Lobby {
    int numero;
    char nom[MAX_NAME_LOBBY];
    int nb_joueur;
    char * ip;
    int port;
} Lobby;

typedef struct Server {
    Lobby * tabLobby;
    int nb;
} Server;

int addPlayer(char * pseudo);
int connectToServer(char * buffer);
int createLobby(Server * server,char * buffer);
int connectToLobby(Server server,int sd, char * buffer);
void printLobby(int sd, Server server);
int addLobby(Server * server, char * lobbyName,char * ip, int port);
