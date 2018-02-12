#ifndef SERVEUR_H
#define SERVEUR_H

#include "../definition.h"
#include <iostream>
#include <vector>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "../serveur/client.h"

class Serveur{

public:

    static Serveur * getInstance();
    bool stop();
    void AcceptAndDispatch();
    uint16_t sendToAll(std::string message);
    uint16_t sendToClient(Client *c, std::string message);
    Client getClient(std::string id);
    
    void CAR(Client *client);
    void Remote(Client *client);
    void PC(Client *client);
    void APPLI(Client *client);


private:
    static Serveur * instance;
    Serveur();
    std::vector<Client> *clients;
    int serverSock, clientSock;
    struct sockaddr_in serverAddr, clientAddr;
    char buffer[256];
    bool synchronizeClient(Client *c);
    std::string receive(Client *c);

    bool ID_CAR_use;
    bool ID_PC_use;
    bool ID_REMOTE_use;
    bool ID_TABLETTE_use;
    bool ID_SMARTPHONE_use;
    bool PC_CONTROL;
    
    
    Client *APPLI_CONTROL;

    
};
//thread fonction
static void* ComPC(void *c);
static void* ComAPPLI(void *c);
static void* ComCAR(void *c);
static void* ComRemote(void *c);
#endif