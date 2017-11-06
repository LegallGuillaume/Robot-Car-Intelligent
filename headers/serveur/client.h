#ifndef CLIENT_H
#define CLIENT_H

#include <iostream>
#include <string>
#include <cstdio>
#include <cstdlib>

#define MAX_NAME_LENGHT 20

class Client {
  public:
    std::string m_id;
    uint16_t m_sock;

  public:
    Client(){
      m_id = "";
    }
};

#endif