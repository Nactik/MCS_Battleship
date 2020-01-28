#include "../game/gameUtils.h"

/**
 * \brief Mutex permettant de mettre en suspend le processus principal le temps du jeu
 * 
 */
sem_t * mutex;

/**
 * \brief Affiche le menu de navigation du clieny
 * 
 * \return int Choix du client dans le menu
 */
int displayMenu(); 

/**
 * @brief Affiche la liste des lobbies disponibles
 * 
 * @param sock La socket de discussion avec le server
 */
void printLobby(int sock);

/**
 * \brief Effectue une requete de création de lobby au server et lance le jeu en cas de succès
 * 
 * \param sock_server La socket de discussion avec le server
 */
void createLobby(int sock_server); 

/**
 * \brief Met le client en attente d'un adversaire puis lance le jeu
 * 
 * \return void* 
 */
void * waitPlayer(void *);

/**
 * \brief Effectue une requete de connexion au lobby au server
 * 
 * \param sock La socket de discussion avec le server
 */
void connectToLobby(int sock);