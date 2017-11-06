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
private:
    static Serveur * instance;
    Serveur();
    std::vector<Client> *clients;
    int serverSock, clientSock;
    struct sockaddr_in serverAddr, clientAddr;
    char buffer[256];
    bool synchronizeClient(Client *c);
    std::string receive(Client *c);
};

#endif