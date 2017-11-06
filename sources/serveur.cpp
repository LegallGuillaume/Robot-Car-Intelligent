#include <iostream>
#include "../headers/manager/serveur.h"
#include "../headers/manager.h"

#include <algorithm>
//#include <sstream>

Serveur * Serveur::instance = nullptr;

Serveur * Serveur::getInstance(){
    if(instance == nullptr){
        instance = new Serveur();
    }
    return instance;
}

Serveur::Serveur(){
    printf("STARTING SERVER ...\n");
    int yes = 1;
    serverSock = socket(AF_INET, SOCK_STREAM, 0);
    memset(&serverAddr, 0, sizeof(sockaddr_in));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(PORT_SERVEUR);

    setsockopt(serverSock,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof(int));
  
    if(bind(serverSock, (const struct sockaddr *) &serverAddr, sizeof(sockaddr_in)) < 0){
        std::cerr << "Failed to bind"<<std::endl; /*n'est pas exécuté avec sudo*/
    }
    printf("SERVER CONNECTED\n");
    clients = new std::vector<Client>();
    clients->reserve(MAX_CONNECTION_CLIENT);
    listen(serverSock, 5);
}

void Serveur::AcceptAndDispatch(){
    Client *client;
    socklen_t cliSize = sizeof(sockaddr_in);
    while(1) {
        client = new Client();
        client->m_sock = accept(serverSock, (struct sockaddr *) &clientAddr, &cliSize);
        if(client->m_sock < 0 || clients->size() >= MAX_CONNECTION_CLIENT) {
          std::cout << "Error on accept"<<std::endl;
          delete client;
        }else{
            if(!synchronizeClient(client)){
                std::cout << "Client not valid"<<std::endl;
                sendToClient(client, std::string(INVAL_CODE));
                delete client;
            }else{
                clients->push_back(*client);
                std::cout<<"Client connected "<<client->m_id<<std::endl;
                sendToClient(client, std::string(VAL_CODE));
                if(client->m_id == ID_CAR){
                    bool colision = false;
                    std::string tostring = Manager::getInstance()->car->getPosition().first + "," + 
                                                                    Manager::getInstance()->car->getPosition().second;
                    sendToClient(client, colision + "[" + tostring + "]" + "\n");
                }
                else{
                    sendToClient(client, Manager::getInstance()->toString() + "\n");
                }
                std::string recep = receive(client);
                if(recep == "" || recep == STOP_CONN){
                    std::cout<<"client disconnected !"<<std::endl;
                    delete client;
                }
                sendToClient(client, std::string(STOP_CONN));
            }
        }
    }
}

uint16_t Serveur::sendToAll(std::string message) {
    uint16_t nb_byte=0;
    for(uint8_t i=0; i<clients->size(); i++) {
        nb_byte += send(clients->at(i).m_sock, message.c_str(), strlen(message.c_str()), 0);
    }
    return nb_byte;
}

/**
*   Return nb of byte;
**/
uint16_t Serveur::sendToClient(Client *client, std::string message){
    return send(client->m_sock, message.c_str(), strlen(message.c_str()), 0);
}

Client Serveur::getClient(std::string id){
    for(uint8_t index=0; index<clients->size(); ++index){
        if(clients->at(index).m_id == id)
            return clients->at(index);
    }
    return Client();
}

bool Serveur::synchronizeClient(Client *c){
    sendToClient(c, std::string(REQ_CODE));
    std::string id_code = receive(c);
    return (id_code == ID_PC || id_code == ID_REMOTE || id_code == ID_CAR);
}

std::string Serveur::receive(Client *c){
    memset(buffer, 0, sizeof buffer);
    if(read(c->m_sock, buffer, 255) <= 0){
        return std::string("");
    }
    return std::string(buffer);
}