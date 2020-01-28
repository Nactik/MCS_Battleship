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
#include <semaphore.h>
#include <pthread.h>
#include <sys/stat.h>
#include <fcntl.h>


#define CHECK(sts,msg) if ((sts) == -1) {perror(msg);exit(-1);}
#define NB_ELEM(tab) sizeof((tab))/sizeof((tab[0]))

#define MAX_BUFF 1024
#define MAX_LENGTH_IP 15
#define MAX_PLAYER_NAME 25
#define MAX_NAME_LOBBY 25

#define DISCONNECT      0
#define CONNECT_SRV     100
#define CONNECT_SRV_OK  102
#define CREATE_LOB      200
#define CREATE_LOB_OK   202
#define DELETE_LOB      203
#define DELETE_LOB_OK   204
#define CONNECT_LOB     300
#define CONNECT_LOB_OK  302
#define SPECT_LOB       303
#define PRINT_LOB       400
#define HAS_LOBBY       401
#define END_LOBBY       402
#define OK              151

#define ATTACK          500
#define RESULT_ATK      600
#define END_PLACING     700
#define ERREUR          -1
