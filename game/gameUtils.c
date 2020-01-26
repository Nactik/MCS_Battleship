#include "gameUtils.h"

void initializeBoards()
{
    int line, column;
        for(line=0 ; line < MAX_LINE ; line++ )
            for(column=0 ; column < MAX_COLUMN ; column++ )
                myBoard[line][column]=WATER;
                oponentBoard[line][column]=WATER;
}

void showBoard(int board[MAX_LINE][MAX_COLUMN])
{

    int line, column;

        printf("\t1 \t2 \t3 \t4 \t5");
        printf("\n");

        for(line=0 ; line < MAX_LINE ; line++ ){
            printf("%d",line+1);
            for(column=0 ; column < MAX_COLUMN ; column++ ){
                if(board[line][column]==WATER){
                    printf("\t\033[1;34m~\x1b[0m");
                }else if(board[line][column]==SHIP){
                    printf("\t\033[22;32m*\x1b[0m");
                }else if(board[line][column]==MISSED){
                    printf("\t\033[22;31mX\x1b[0m");
                }

            }
            printf("\n");
        }

    }

int checkBound(int line, int column){
    if(line<=0 || line>MAX_LINE)
        return 0;
    if(column<=0 || column>MAX_COLUMN)
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
}

int hitShip(int line, int column)
{
    if(myBoard[line-1][column-1] == SHIP){
        return 1;
    }
    return 0;
}

void waitAttack(int socket){
    char msgToSend[MAX_BUFF],buffer[MAX_BUFF],content[MAX_BUFF];
    int numReq,line,column;
    //Si joueur 2, on joue en 2é donc on read d'abord
    CHECK(read(socket,buffer,sizeof(buffer)),"Can't read");
    sscanf(buffer,"%d:%d%d",&numReq,&line,&column);
    if(hitShip(line,column) == 1){
        sprintf(msgToSend,"%d:%c",RESULT_ATK,'T');
    } else{
        sprintf(msgToSend,"%d:%c",RESULT_ATK,'M');
    }
    CHECK(write(socket,msgToSend,strlen(msgToSend)+1),"Can't write");
}

void updateBoard(char result, int line, int column,int * nbShipTouched){
    if(result == 'T'){
        puts("\t Bravo, vous avez touché un bateau !!");
        oponentBoard[line-1][column-1] = SHIP;
        *(nbShipTouched) += 1;
    } else if(result == 'M'){
        puts("\t Dommage, vous n'avez rien touché !!");
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
    sprintf(buffer,"%d:%d%d",ATTACK,line,column);
    CHECK(write(socket,buffer,strlen(buffer)+1),"Can't write");
    //On attend la réponse de l'autre client
    CHECK(read(socket,buffer,sizeof(buffer)),"Can't read");
    sscanf(buffer,"%d:%c",&numReq,&result);
    updateBoard(result,line,column,nbShipTouched);
}

void startGame(int socket,int player) {
    char msgToSend[MAX_BUFF],buffer[MAX_BUFF];
    int nbShipTouched,numReq;

    //on init les plateau
    initializeBoards();
    //On place le pion
    placeShip();

    puts("J'ai fini de placé mes bateaux");
    //Synchro, on prévient l'autre qu'on a fini et on l'attent
    sprintf(msgToSend,"%d",END_PLACING);
    CHECK(write(socket,msgToSend,strlen(msgToSend)+1),"Can't write");
    CHECK(read(socket,buffer,sizeof(buffer)),"Can't read");
    
    puts("");

    while(nbShipTouched != MAX_SHIP){
        if(player == 2){
            waitAttack(socket);
        }

        showBoard(oponentBoard);
        attack(socket,&nbShipTouched);

        if(player ==1){
            waitAttack(socket);
        }
    }

    printf("\n\n\nFinished game. You hit the three ships in %d attempts", 3);
    showBoard(oponentBoard);
}
