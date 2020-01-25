#include "../include/header.h"
#define WELCOME "Bienvenue sur la bataille navale ! \n"
#define BYE "Au revoir et à bientot sur la bataille navale !"
#define MAX_LOBBY 20
#define MAX_PLAYER 50

typedef struct Joueur{
    char pseudo[MAX_PLAYER_NAME];
} Joueur;

typedef struct Lobby {
    int numero;
    char nom[MAX_NAME_LOBBY];
    int nb_joueur;
    char ip[MAX_LENGTH_IP];
    int port;
    int affected;
    Joueur owner;
} Lobby;

typedef struct Server {
    Lobby tabLobby[MAX_LOBBY];
    int nb;
} Server;

typedef struct Sock {
    int socket;
    Joueur client;
} Sock;

Server server;
Sock client_socket[MAX_PLAYER];

int addPlayer(Sock * sd,char * pseudo);
int connectToServer(Sock * sd,char * buffer);
int createLobby(Sock sd,char * buffer);
int connectToLobby(Sock sd, char * buffer);
void printLobby(Sock sd);
int addLobby(char * owner,char * lobbyName,char * ip, int port);
