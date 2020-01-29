#include "gameUtils.h"

void initializeBoards()
{
    int line, column;
    //On rempli les plateux de jeu avec de l'eau
    for(line=0 ; line < MAX_LINE ; line++ ){
        for(column=0 ; column < MAX_COLUMN ; column++ ){
            myBoard[line][column]=WATER;
            oponentBoard[line][column]=WATER;
        }
    }
}

void showBoard(int board[MAX_LINE][MAX_COLUMN])
{

    int line, column;
    
    printf("\n\n");
    printf("\t");
    for(int i=0;i<MAX_LINE;i++){
        printf("\t%d ",(i+1));
    }
    printf("\n");

    for(line=0 ; line < MAX_LINE ; line++ ){
        printf("\t%d",line+1);
        for(column=0 ; column < MAX_COLUMN ; column++ ){
            if(board[line][column]==WATER){
                //Vague bleue si c'est de l'eau
                printf("\t\033[1;34m~\x1b[0m");
            }else if(board[line][column]==SHIP){
                //Etoile verte si c'est un bateau
                printf("\t\033[22;32m*\x1b[0m");
            }else if(board[line][column]==MISSED){
                //Croix rouge si on a manqué
                printf("\t\033[22;31mX\x1b[0m");
            }
        }
        printf("\n");
    }
    printf("\n\n");
}

int checkBound(int line, int column){
    if(line<=0 || line>MAX_LINE)
        return 0;
    if(column<=0 || column>MAX_COLUMN)
        return 0;
    if(oponentBoard[line][column] == SHIP)
        return 0;
    return 1;
}

void placeShip(){
    int line,column;
    int nbShiftPlaced = 0;
    while(nbShiftPlaced != MAX_SHIP){
        showBoard(myBoard);
        printf("Ligne où placer le bateau: ");
        scanf("%d",&line);

        printf("Colonne où placer le bateau: ");
        scanf("%d",&column);
        
        if(checkBound(line,column) == 0){
            puts("Erreur de coordonnées, veuillez réessayer !");
        }else {
            myBoard[line-1][column-1] = SHIP;
            nbShiftPlaced++;
        }
    }
    showBoard(myBoard);
}

int hitShip(int line, int column)
{   
   // printf("Case : %d\n",myBoard[line-1][column-1]);
    if(myBoard[line-1][column-1] == SHIP){
        return 1;
    }
    return 0;
}

void waitAttack(int socket, int * myShipTouched){
    char msgToSend[MAX_BUFF],buffer[MAX_BUFF],content[MAX_BUFF];
    int numReq,line,column;
    CHECK(read(socket,buffer,sizeof(buffer)),"Can't read");
    //printf("Je reçois : %s\n",buffer);
    sscanf(buffer,"%d:%d:%d",&numReq,&line,&column);
    if(numReq == ATTACK){
        if(hitShip(line,column) == 1){
            sprintf(msgToSend,"%d:%c",RESULT_ATK,'T');
            *(myShipTouched) += 1; //Un de mes bateaux a été touché
        } else{
            sprintf(msgToSend,"%d:%c",RESULT_ATK,'M');
        }
        //printf("J'envoie : %s\n",msgToSend);
        CHECK(write(socket,msgToSend,strlen(msgToSend)+1),"Can't write");
    } 
}

void updateBoard(char result, int line, int column,int * nbShipTouched){
    if(result == 'T'){
        puts("\tBravo, vous avez touché un bateau !!");
        oponentBoard[line-1][column-1] = SHIP;
        *(nbShipTouched) += 1;
    } else if(result == 'M'){
        puts("\tDommage, vous n'avez rien touché !!");
        oponentBoard[line-1][column-1] = MISSED;
    }
    showBoard(oponentBoard);
}

void attack(int socket,int * nbShipTouched){
    char buffer[MAX_BUFF];
    int flag = 0;
    int line, column,numReq;
    char result;

    while(flag != 1){
        printf("Ligne où attaquer le bateau: ");
        scanf("%d",&line);

        printf("Colonne où attaquer le bateau: ");
        scanf("%d",&column);
        
        if(checkBound(line,column) == 0){
            puts("Erreur de coordonnées, veuillez réessayer !");
        }else {
            flag = 1;
        } 
    }
    sprintf(buffer,"%d:%d:%d",ATTACK,line,column);
    CHECK(write(socket,buffer,strlen(buffer)+1),"Can't write");
    //On attend la réponse de l'autre client
    CHECK(read(socket,buffer,sizeof(buffer)),"Can't read");
    sscanf(buffer,"%d:%c",&numReq,&result);
    updateBoard(result,line,column,nbShipTouched);
}

void startGame(int socket,int player) {
    char msgToSend[MAX_BUFF],buffer[MAX_BUFF];
    int nbShipTouched,myShipTouched,numReq;
    int lost = 0;

    //on init les plateau
    initializeBoards();
    //On place le pion
    placeShip();

    puts("Placement des bateaux terminés !");
    //Synchro, on prévient l'autre qu'on a fini et on l'attent
    sprintf(msgToSend,"%d",END_PLACING);
    CHECK(write(socket,msgToSend,strlen(msgToSend)+1),"Can't write");
    CHECK(read(socket,buffer,sizeof(buffer)),"Can't read");
    
    puts("");

    nbShipTouched = 0; //bateaux adverse
    myShipTouched = 0; //Mes bateaux

    while(1){
        if(player == 2){
            puts("Au tour de l'adversaire !"); 
            waitAttack(socket, &myShipTouched);
            if(myShipTouched == MAX_SHIP){ //On sort 
                puts("Perdu !!"); 
                break; 
            }
        }

        //showBoard(oponentBoard);
        attack(socket,&nbShipTouched);
        if(nbShipTouched == MAX_SHIP){ //On sort 
            puts("Gagné !!"); 
            break; 
        }

        if(player ==1){
            puts("Au tour de l'adversaire !");
            waitAttack(socket, &myShipTouched);
            if(myShipTouched == MAX_SHIP){ //On sort 
                puts("Perdu !!"); 
                break; 
            }
        }
    }

    puts("Retour au serveur de jeu");
}
