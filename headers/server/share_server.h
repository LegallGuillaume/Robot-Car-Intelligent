#ifndef SHARE_SERVER_H
#define SHARE_SERVER_H

#include <iostream>
#include <cstdlib>
#include <cstring>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include "../definition.h"

struct _shareM{
    uint8_t index; /*if index is the same so the value is ever use*/
    char id_code[16];
    char tostring[255];
};

typedef struct _shareM shareMemory_t;

class sServer{

    public:
        static sServer* getInstance();
        bool sendThingsTo(const char *, char *);
        bool isConnected(){
            return is_connected;
        }
    private:
        bool is_connected;
        key_t key_send;
        int shmid_send;
        shareMemory_t *data_send;
        static sServer* instance;
        sServer();
        ~sServer(){
            shmctl(shmid_send, IPC_RMID, NULL);
            delete data_send;
        }

};

#endif