#include "clientUtils.h"

int displayMenu(){
    int choix=0; 
    do
    {
        printf("\t----- MENU JEU BATAILLE NAVALE-----\n"); 
        printf("\t\t 1) Créer un lobby \n"); 
        printf("\t\t 2) Jouer sur un lobby \n"); 
        printf("\t\t 3) Etre spectateur sur un lobby \n"); 
        printf("\t\t 4) Quitter \n"); 
        scanf("%d", &choix); 
    } while (choix>4||choix<1);
    
    return choix;
}