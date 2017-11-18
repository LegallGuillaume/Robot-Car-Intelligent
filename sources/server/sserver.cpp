#include <iostream>
#include "../../headers/serveur/share_server.h"

#include <string>
#include <sstream>
#include <cstring>

sServer * sServer::instance = nullptr;

sServer* sServer::getInstance(){
    if(instance == nullptr){
        instance = new sServer();
    }
    return instance;
}

sServer::sServer(){
    data_send = (shareMemory_t*)std::malloc(sizeof(shareMemory_t));
    data_send->index = 0;
    index_receive = 0;

    key_send = ftok("server.cpp", 'R');
    shmid_send = shmget(key_send, sizeof(shareMemory_t), 0644 | IPC_CREAT);
    data_send = (shareMemory_t*)shmat(shmid_send, (void*)0, 0);

    key_receive = ftok("sserver.cpp", 'R');
    shmid_receive = shmget(key_receive, sizeof(shareMemory_t), 0444 | IPC_CREAT);
    data_receive = (shareMemory_t*)shmat(shmid_receive, (void*)0, 0);
    sendThingsTo(ID_CONNECTION, INTER_PROC);
}

bool sServer::sendThingsTo(const char *id_code, char *msg){
    uint8_t index = data_send->index;
    strcpy(data_send->id_code, id_code);
    strcpy(data_send->tostring, msg);
    data_send->index = index + 1;
}

shareMemory_t* sServer::receive(){
    if(index_receive == data_receive->index)
        return nullptr; /*if same index its ever reveice from server*/
    index_receive = data_receive->index;
    return data_receive;
}



/*
int main(){
    sServer *serv = sServer::getInstance();
    sleep(5);
    serv->sendThingsTo(ID_PC, "{ID: \"TEST\"}");
    sleep(5);
    serv->sendThingsTo(ID_CAR, "{SCENE: \"1BLOC\"}");
    sleep(5);
    serv->sendThingsTo(ID_REMOTE, "{POSITION: \"FOOBAR\"}");
}
*/