#include "include/header.h"
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

Joueur * tabJoueur; //tableau de joueur
Lobby * tabLobby; // tableau de Lobby

int addPlayer(char * pseudo);
int connectToServer(char * buffer);
void createLobby(char * buffer);
void printLobby(int sd);
int addLobby(char * lobbyName,char * ip, int port)
