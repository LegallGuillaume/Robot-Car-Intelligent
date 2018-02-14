#include "../headers/manager/serveur.h"
#include "../headers/manager.h"

Client::Client()
{
  m_id = "";
  init=true;
  }
void Client::stopThread(void)
{
pthread_cancel (thread);
}