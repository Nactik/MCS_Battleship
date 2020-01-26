#include "../include/header.h"
#define MAX_COLUMN  5
#define MAX_LINE    5
#define MAX_SHIP    5
#define WATER       0
#define SHIP        1
#define MISSED      2


int myBoard[MAX_LINE][MAX_COLUMN];
int oponentBoard[MAX_LINE][MAX_COLUMN];

void startGame(int socket,int nbPlayer);
void initializeBoards();
void placeShip();
void showBoard(int board[MAX_LINE][MAX_COLUMN]);
int checkBound(int line, int column);
int hitShip(int line, int column);
void waitAttack(int socket);
void updateBoard(char result, int line, int column,int * nbShipTouched);
void attack(int socket,int * nbShipTouched);

