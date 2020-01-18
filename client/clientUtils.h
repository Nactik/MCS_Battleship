#include "../include/header.h"

int displayMenu(); 
void printLobby(int sock);
void createLobby(int sock_server, int * sock_lobby); 
void waitPlayer(int *);
void connectToLobby(int sock);