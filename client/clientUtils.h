#include "../game/gameUtils.h"

int displayMenu(); 
void printLobby(int sock);
void createLobby(int sock_server, int * sock_lobby); 
void * waitPlayer(void *);
void connectToLobby(int sock);