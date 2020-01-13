#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include <sys/select.h>

#define CHECK(sts,msg) if ((sts) == -1) {perror(msg);exit(-1);}
#define NB_ELEM(tab) sizeof((tab))/sizeof((tab[0]))

#define NB_PLAYER 20
#define MAX_BUFF 1024
#define MAX_LENGTH_IP 15
#define MAX_PLAYER_NAME 25

#define CONNECT_SRV     100
#define CREATE_LOB      200
#define CONNECT_LOB     300
#define PRINT_LOB       150
#define OK              151

#define ATTACK          400
#define CONNECT_SRV_OK  102
#define CONNECT_LOB_OK  302
#define RESULT_ATK      500
#define ERREUR          "KO:Une erreur est survenue ! Réesayer"
