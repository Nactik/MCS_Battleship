#include "serverUtils.h"

int dialogueClt (Server * server, int sd, struct sockaddr_in clt) {
    char receive[MAX_BUFF];
    char requete[MAX_BUFF];
    char content[MAX_BUFF];
    int req;
    char toSend[MAX_BUFF];
    //write(sd,WELCOME,strlen(WELCOME)+1);

    CHECK(read(sd, receive, sizeof(receive)),"erreur read");
    sscanf (receive, "%d:%s",&req, content);
    printf("Req : %d\n", req);
    printf("Buffer : %s \n",content);
    switch (req) {
        case CONNECT_SRV : 
            if(connectToServer(content) == 1){
                sprintf(toSend,"%d,%s",CONNECT_SRV_OK,"Joueur connecté !");
                write(sd,toSend,strlen(toSend)+1);
            } else {
                // write(sd,ERREUR,strlen(ERREUR)+1);
            }
            break;
            
        case CREATE_LOB : 
            puts("CREATION LOBBY");
            createLobby(server,content);
            break;
        case CONNECT_LOB: 
            connectToLobby(*server,sd,content);
            break;

        case PRINT_LOB:
            puts("Affichage lobby");
            printLobby(sd,*server);
            puts("J'ai fini l'affichage");

            break;
        case DISCONNECT:    
            write(sd, BYE, strlen(BYE)+1);
            break;
        default : 
            write(sd, ERREUR, strlen(ERREUR)+1);
            break;
    }
    return req;
} 

void deroute(int signal){
    int ret;
    switch(signal){
        case SIGINT :
            wait(&ret);
            break;
    }
}

int main(int argc, char ** argv){
    int se, sd,svcLen,ret,status,max_sd,activity,retDial;
    int client_socket[NB_PLAYER];
    struct sockaddr_in svc, clt;
    socklen_t cltLen;
    Server server;
    server.nb=0;
    fd_set rfds;   

    //initialise all client_socket[] to 0 so not checked  
    for (int i = 0; i < NB_PLAYER; i++)   
    {   
        client_socket[i] = 0;   
    }   
         
    //signal(SIGCHLD,deroute);
    signal(SIGINT,deroute);
    // Création de la socket de réception d’écoute des appels
    CHECK(se=socket(AF_INET, SOCK_STREAM, 0), "Can't create");
    puts("Création socket écoute");

    // Préparation de l’adressage du service (d’appel)
    svc.sin_family = AF_INET;
    svc.sin_port = htons(0);
    svc.sin_addr.s_addr = INADDR_ANY;
    memset(&svc.sin_zero, 0, 8);
    
    // Association de l’adressage préparé avec la socket d’écoute
    CHECK(bind(se,(struct sockaddr *) &svc, sizeof svc), "Can't bind");
    puts("Bind socket écoute");
    
    svcLen = sizeof(svc);
    CHECK(getsockname(se,(struct sockaddr *) &svc,&svcLen),"test");
    printf("port : %d\n",ntohs(svc.sin_port));
    printf("address : %s\n",inet_ntoa(svc.sin_addr));
    
    // Mise en écoute de la socket
    CHECK(listen(se, NB_PLAYER) , "Can't calibrate");
    puts("Mise en écoute socket écoute");

    // Boucle permanente de service
    while (1) {
        //On clear le set de socket
        FD_ZERO(&rfds);   
     
        //On ajoute la socket d'écoute set  
        FD_SET(se, &rfds);   
        max_sd = se;   
             
        //Pour chaque enfant, on l'ajoute au set
        for (int i = 0 ; i < NB_PLAYER ; i++)   {  
            sd = client_socket[i];   
            if(sd > 0)   
                FD_SET(sd,&rfds);   
            if(sd > max_sd)   
                max_sd = sd;   
        }   

        CHECK(activity = select( max_sd + 1 , &rfds , NULL , NULL , NULL),"Erreur");   

        //Si activité sur la master => c'est une connextion
        if (FD_ISSET(se, &rfds)) {   
            cltLen = sizeof(clt);
            CHECK(sd=accept(se,(struct sockaddr *) &clt, &cltLen),"Can't connect");
            //Information sur l'utilisateur entrant
            printf("New connection , socket fd is %d , ip is : %s , port : %d\n",
                    sd, inet_ntoa(clt.sin_addr) , ntohs(clt.sin_port));   
           
            //On envoie bienvenu
            write(sd,WELCOME,strlen(WELCOME)+1);
            puts("Bienvenue envoyé !");   
                 
            //On ajoute sa socket, à la liste de socket enfant
            for (int i = 0; i < NB_PLAYER; i++)   
            {   
                //On check si y'a une place 
                if(client_socket[i] == 0 )   
                {   
                    client_socket[i] = sd;   
                    break;   
                }   
            }   
        }

        //On check si y'a une activité sur un des clients
        for (int i = 0; i < NB_PLAYER; i++)   
        {   
            sd = client_socket[i];   
            if (FD_ISSET(sd,&rfds)){   
                retDial = dialogueClt(&server,sd,clt);
                //shutdown  
                if(retDial == DISCONNECT){
                    shutdown(sd,2);
                    client_socket[i] = 0; 
                }
            }   
        }
    }
}