#define MAX_NAME_LOBBY 25
#define MAX_NAME_PSEUDO 25


typedef struct Lobby {
    int numero;
    char nom[MAX_NAME_LOBBY];
    int nb_joueur;
    char * ip;
    int port;
} Lobby;

typedef struct Joueur
{
    char pseudo[MAX_NAME_PSEUDO];
    char * ip;
    int port;
} Joueur;
