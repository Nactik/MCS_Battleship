#include "../include/header.h"
/**
 * \brief Nombre max de colonnes du plateau de jeu
 */
#define MAX_COLUMN  5
/**
 * \brief Nombre max de lignes du plateau de jeu
 */
#define MAX_LINE    5
/**
 * \brief Nombre de max de bateau que le joueur peut placer
 */
#define MAX_SHIP    5

#define WATER       0
#define SHIP        1
#define MISSED      2

/**
 * \brief Plateau de jeu du joueur (tableau 2d)
 */
int myBoard[MAX_LINE][MAX_COLUMN];
/**
 * \brief Plateau de jeu du joueur ennemi (tableau 2d)
 */
int oponentBoard[MAX_LINE][MAX_COLUMN];

/**
 * \brief Lance le jeu, initialise les plateau et permet aux joueur d'attaquer
 * 
 * \param socket Socket de discussion avec l'autre joueur
 * \param nbPlayer Numero du joueur (joueur 1 ou 2) pour definir qui attaquera en premier
 */
void startGame(int socket,int nbPlayer);

/**
 * \brief Initialise les plateaux de jeu avec de l'eau
 */
void initializeBoards();

/**
 * \brief Permet à un joueur de placer ses bateaux sur son plateau
 * 
 */
void placeShip();

/**
 * \brief Affiche le plateu de jeu (le sien ou celui de l'ennemi)
 * 
 * \param board Le plateau à afficher
 */
void showBoard(int board[MAX_LINE][MAX_COLUMN]);

/**
 * \brief Check si les coordonnées sont à l'interieur du plateau et non pas déjà été attaquées
 * 
 * \param line Ligne à checker
 * \param column Colonne à checker
 * \return int 0 si coordonnées non valides sinon 1
 */
int checkBound(int line, int column);

/**
 * \brief Verifie si on a touché un bateau adverse aux coordonnées passées en paramètre
 * 
 * \param line Ligne à checker
 * \param column Colonne à checker
 * \return int 1 si touché, 0 sinon
 */
int hitShip(int line, int column);

/**
 * \brief Attends que l'autre joueur effectue une requete d'attaque et envoie le résultat
 * 
 * \param socket Socket de discussion avec l'autre client
 * \param myShipTouched Nombre de bateau touché
 */
void waitAttack(int socket, int * myShipTouched);

/**
 * \brief Met à jour le plateau ennemi et le nombre de bateau touché selon le resultat de l'attaque (hitShip)
 * 
 * \param result    Resultat de la requete d'attaque - 'T' si touché - 'M' si manqué
 * \param line      Ligne à update    
 * \param column    Colonne à update
 * \param nbShipTouched Nombre de bateau touché par le client
 */
void updateBoard(char result, int line, int column,int * nbShipTouched);

/**
 * \brief Effectue une requete d'attaque à l'autre client et attent le resultat
 * 
 * \param socket Socket de discussion avec l'autre client
 * \param nbShipTouched Nombre de bateau touché
 */
void attack(int socket,int * nbShipTouched);

