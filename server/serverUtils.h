#include "../include/header.h"
#define WELCOME "Bienvenue sur la bataille navale ! \n"
#define BYE "Au revoir et à bientot sur la bataille navale !"

typedef struct Joueur{
    char pseudo[MAX_PLAYER_NAME];
} Joueur;

typedef struct Lobby {
    int numero;
    char nom[MAX_NAME_LOBBY];
    int nb_joueur;
    char ip[MAX_LENGTH_IP];
    int port;
} Lobby;

typedef struct Server {
    Lobby * tabLobby;
    int nb;
} Server;

Server server;

int addPlayer(char * pseudo);
int connectToServer(char * buffer);
int createLobby(int sd,char * buffer);
int connectToLobby(int sd, char * buffer);
void printLobby(int sd);
int addLobby(char * lobbyName,char * ip, int port);
