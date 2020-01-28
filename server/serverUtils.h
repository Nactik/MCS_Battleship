#include "../include/header.h"
/**
 * \brief Message de bienvenu
 */
#define WELCOME "Bienvenue sur la bataille navale ! \n"

/**
 * \brief Message de déconnexion
 */
#define BYE "Au revoir et à bientot sur la bataille navale !"

/**
 * \brief Nombre de lobby maximal
 */
#define MAX_LOBBY 20

/**
 * \brief Taille de joueur maximal
 */
#define MAX_PLAYER 50

/**
 * \struct Joueur
 * 
 * \brief Structure Joueur - possede un pseudo
 */
typedef struct Joueur{
    char pseudo[MAX_PLAYER_NAME];
} Joueur;

/**
 * \struct Lobby
 *  
 * \brief Structure décrivant un Lobby (salle de jeu).
 * 
 */
typedef struct Lobby {
    int numero; /*!< Index du lobby */
    char nom[MAX_NAME_LOBBY];
    int nb_joueur; /*!< Nombre de joueurs presents dans le lobby */
    char ip[MAX_LENGTH_IP];
    int port;
    int affected; /*!< 0 si elle peut etre reaffecté */
    Joueur owner; /*!< Hebergeur de la partie*/
} Lobby;

/**
 * \struct Server
 *  
 * \brief Structure décrivant un tableau de lobby
 * 
 */
typedef struct Server {
    Lobby tabLobby[MAX_LOBBY];
    int nb;
} Server;

/**
 * \struct Sock
 *  
 * \brief Structure associant une socket et son client.
 * 
 */
typedef struct Sock {
    int socket;
    Joueur client;
} Sock;

/**
 * \brief norte tableau de lobby
 */
Server server;

/**
 * \brief Tableau de socket
 */
Sock client_socket[MAX_PLAYER];

/**
 * \brief Ajoute un joueur et associe son pseudo à sa socket
 * 
 * \param sd Socket de discussion avec le client
 * \param buffer Contient le pseudo du joueur
 */
void connectToServer(Sock * sd,char * buffer);

/**
 * \brief Recupère les informations permettant de creer un lobby de la requete du client
 * 
 * \param sd Socket de discussion avec le client    
 * \param buffer Contient une partie de la requete du client "ip:port"
 * \return int 1
 */
int createLobby(Sock sd,char * buffer);

/**
 * \brief Supprime le lobby associé au pseudo 
 * 
 * \param owner Pseudo de l'hebergeur du lobby
 * \return int 1 si lobby supprimé, sinon 0
 */
int deleteLobby(char * owner);

/**
 * \brief Envoie les informations du lobby voulu (ip et port) au client afin qu'il s'y connecte
 * 
 * \param sd Socket de discussion avec le client
 * \param buffer Numero du lobby 
 */
void connectToLobby(Sock sd, char * buffer);

/**
 * \brief Prepare et envoie les requetes de listages des lobby au client
 * 
 * \param sd Socket de discussion avec le client
 */
void printLobby(Sock sd);

/**
 * \brief Crée un lobby avec les informations passés en params (Pseudo, nom de salle, ip, port)
 * 
 * \param owner Pseudo du créateur de la salle
 * \param lobbyName Nom de la salle
 * \param ip Addresse ip lié à la salle
 * \param port Le port de la salle 
 * \return int 1
 */
int addLobby(char * owner,char * lobbyName,char * ip, int port);
