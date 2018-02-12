#ifndef CLIENT_H
#define CLIENT_H

#include <iostream>
#include <string>
#include <cstdio>
#include <cstdlib>
#include "../manager/serveur.h"

#define MAX_NAME_LENGHT 20

class Client {
  public:
    std::string m_id;
    uint16_t m_sock;
    bool init;
    pthread_t thread;
  public:
    Client();
    void stopThread(void);
};

#endif
