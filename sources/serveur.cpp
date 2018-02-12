#include <iostream>
#include "../headers/manager/serveur.h"
#include "../headers/manager.h"

#include <pthread.h>
#include <algorithm>

Serveur *Serveur::instance = nullptr;

Serveur *Serveur::getInstance()
{
    //instancie le serveur , si il est dejas instancier on retourn un pointeur vers le serveur
    if (instance == nullptr)
    {
        instance = new Serveur();
    }
    return instance;
}

Serveur::Serveur()
{
    //start le serveur
    printf("STARTING SERVER ...\n");

    //init variable
    int yes = 1;
    ID_CAR_use = false;
    ID_PC_use = false;
    ID_REMOTE_use = false;

    PC_CONTROL = false;

    //create socket for serveur
    serverSock = socket(AF_INET, SOCK_STREAM, 0);

    //clear serveuraddr
    memset(&serverAddr, 0, sizeof(sockaddr_in));

    //config socket
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(PORT_SERVEUR);
    //config  socket option

    setsockopt(serverSock, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));

    if (bind(serverSock, (const struct sockaddr *)&serverAddr, sizeof(sockaddr_in)) < 0)
    {
        std::cerr << "Failed to bind" << std::endl; /*n'est pas exécuté avec sudo*/
    }

    printf("SERVER CONNECTED\n");
    //create  client list
    clients = new std::vector<Client>();
    //reserve  memory for client
    clients->reserve(MAX_CONNECTION_CLIENT);
    //set the server socket as passif socket ,that mean this socket is using to wait connection from client.
    listen(serverSock, 5);
}

void Serveur::AcceptAndDispatch()
{
    //create client
    Client *client;

    socklen_t cliSize = sizeof(sockaddr_in);

    std::string tostring = "";

    while (1)
    {

        //wait new client

        client = new Client();
        //wait new client

        client->m_sock = accept(serverSock, (struct sockaddr *)&clientAddr, &cliSize);

        //check if the server can accept a new client.
        if (client->m_sock < 0 || clients->size() >= MAX_CONNECTION_CLIENT)
        {
            std::cout << "Error on accept" << std::endl;

            if (client != nullptr)
            {
                delete client;
                client = nullptr;
            }
        }
        else
        {
            std::cout << "client accepted" << std::endl;
            //check client ID in order to see if the client is  a valid.
            if (!synchronizeClient(client))
            {
                std::cout << "Client not valid" << std::endl;
                sendToClient(client, std::string(INVAL_CODE));
                //delete client.
                if (client != nullptr)
                {
                    delete client;
                    client = nullptr;
                }
            }
            else
            {

                //add client list.
                clients->push_back(*client);
                std::cout << "Client connected " << client->m_id << std::endl;
                sendToClient(client, std::string(VAL_CODE));

                //check id to start a appropriate fonction in a thread
                if (client->m_id == ID_CAR)
                {
                    //we can only have 1 car, we use a bool to check if a car is already connected to the serveur
                    ID_CAR_use = true;

                    bool colision = false;
                    tostring = Manager::getInstance()->car->getPosition().first + "," + Manager::getInstance()->car->getPosition().second;
                    sendToClient(client, colision + "[" + tostring + "]" + "\n");

                    std::cout << "CAR CONNECTED" << std::endl;

                    pthread_create(&client->thread, NULL, ComCAR, client);
                }
                else if (client->m_id == ID_REMOTE)
                {
                    //we can only have 1 remote, we use a bool to check if a remote is already connected to the serveur
                    ID_REMOTE_use = true;
                    sendToClient(client, "remote without screen connected\n");

                    std::cout << "REMOTE CONNECTED" << std::endl;
                    pthread_create(&client->thread, NULL, ComRemote, client);
                }
                else if (client->m_id == ID_PC)
                {
                    //we can only have 1 pc, we use a bool to check if a pc is already connected to the serveur
                    ID_PC_use = true;
                    sendToClient(client, Manager::getInstance()->toString() + "\n");

                    std::cout << "PC CONNECTED" << std::endl;

                    pthread_create(&client->thread, NULL, ComPC, client);
                }
                else if (client->m_id == ID_SMARTPHONE)
                {
                    //we can only have 1 smartphone, we use a bool to check if a smarphone is already connected to the serveur
                    ID_SMARTPHONE_use = true;
                    std::cout << "SMARTPHONE CONNECTED" << std::endl;
                    pthread_create(&client->thread, NULL, ComAPPLI, client);
                }
                else if (client->m_id == ID_TABLETTE)
                {
                    //we can only have 1 tablette, we use a bool to check if a tablette is already connected to the serveur
                    ID_TABLETTE_use = true;
                    std::cout << "TABLETTE CONNECTED" << std::endl;
                    //lance la fonction specifique au pc
                    pthread_create(&client->thread, NULL, ComAPPLI, client);
                }
            }
        }
    }
}

uint16_t Serveur::sendToAll(std::string message)
{
    uint16_t nb_byte = 0;
    for (uint8_t i = 0; i < clients->size(); i++)
    {
        nb_byte += send(clients->at(i).m_sock, message.c_str(), strlen(message.c_str()), 0);
    }
    return nb_byte;
}

uint16_t Serveur::sendToClient(Client *client, std::string message)
{
    return send(client->m_sock, message.c_str(), strlen(message.c_str()), 0);
}

Client Serveur::getClient(std::string id)
{
    for (uint8_t index = 0; index < clients->size(); ++index)
    {
        if (clients->at(index).m_id == id)
            return clients->at(index);
    }
    return Client();
}

bool Serveur::synchronizeClient(Client *c)
{

    sendToClient(c, std::string(REQ_CODE));

    // config timeout
    struct timeval tv;
    tv.tv_sec = 1; // Secs Timeout
    tv.tv_usec = 0;

    //set socket option
    setsockopt(c->m_sock, SOL_SOCKET, SO_RCVTIMEO, (const char *)&tv, sizeof(struct timeval));

    //wait to receive ID
    std::string id_code = receive(c);

    c->m_id = id_code;
    //si le code reçus correspond a un code ID valid et que l'ID n'est pas utiliser retourne vrai sinon retour faux
    return (id_code == ID_PC && ID_PC_use == false || id_code == ID_REMOTE && ID_REMOTE_use == false || id_code == ID_CAR && ID_CAR_use == false || id_code == ID_SMARTPHONE && ID_SMARTPHONE_use == false || id_code == ID_TABLETTE && ID_TABLETTE_use == false);
}

std::string Serveur::receive(Client *c)
{
    //receive fonction

    //clear buffer
    memset(buffer, 0, sizeof buffer);
    //read incomming message
    if (read(c->m_sock, buffer, 255) <= 0)
    {
        return std::string(STOP_CONN);
    }

    return std::string(buffer);
}

bool Serveur::stop()
{
    //delete client et thread
    for (uint8_t index = 0; index < clients->size(); ++index)
    {
        //for each client
        if (clients->at(index).init)
        {

            //stop client's thread
            clients->at(index).stopThread();
            //delete client from list
            clients->erase(clients->begin() + index);
        }
    }

    return true;
}
void PC(Client *client);

static void *ComPC(void *c)
{
    //get server
    Serveur *serv = Serveur::getInstance();

    Client *client = (Client *)c;
    //start PC fonction
    serv->PC(client);

    return NULL;
}

void Serveur::PC(Client *client)
{
    std::string tostring = "";
    Client tmp;
    while (client != nullptr)
    {

        //receive data from client
        std::string recep = receive(client);
        //show receive data
        std::cout << "receive data from client PC : " + recep << std::endl;

        if (recep == "<TAKE_CONTROL>\n")
        { //take control of the car
            PC_CONTROL = true;
        } //give back control
        else if (recep == "<GIVE_CONTROL>\n")
        {
            PC_CONTROL = false;
        } //show all connected client
        else if (recep == "<CONNECTED?>\n")
        {
            for (uint8_t index = 0; index < clients->size(); ++index)
            {
                std::cout << (clients->at(index).m_id) << std::endl;
            }
        }
        else if (recep == "<STOP_CLIENT_CAR>\n")
        {
            //get CAR client
            tmp = getClient(ID_CAR);
            //if there is a CAR client
            if (tmp.m_id == ID_CAR)
            {
                //send stop to client
                sendToClient(&tmp, STOP_CONN);
                //stopclient's thread
                tmp.stopThread();

                ID_CAR_use = false;
                tmp = getClient("gtvrv");
                std::cout << "CAR DISCONNECTED" << std::endl;
            }
            else
            {
                std::cout << "CAR NOT CONNECTED" << std::endl;
            }
        }
        else if (recep == "<STOP_CLIENT_REMOTE>\n")
        {
            //get REMOTE client
            tmp = getClient(ID_REMOTE);
            //if there is a REMOTE client
            if (tmp.m_id == ID_REMOTE)
            {
                //send stop to client
                sendToClient(&tmp, STOP_CONN);
                //stopclient's thread
                tmp.stopThread();
                ID_REMOTE_use = false;
                tmp = getClient("gtvrv");
                std::cout << "REMOTE DISCONNECTED" << std::endl;
            }
            else
            {
                std::cout << "REMOTE NOT CONNECTED" << std::endl;
            }
        }
        else if (recep == "<STOP_CLIENT_SMARTPHONE>\n")
        {
            //get SMARTPHONE client
            tmp = getClient(ID_SMARTPHONE);
            //if there is a SMARTPHONE client
            if (tmp.m_id == ID_SMARTPHONE)
            {
                //send stop to client
                sendToClient(&tmp, STOP_CONN);
                //stopclient's thread
                tmp.stopThread();
                ID_SMARTPHONE_use = false;
                tmp = getClient("gtvrv");
                std::cout << "SMARTPHONE DISCONNECTED" << std::endl;
            }
            else
            {
                std::cout << "SMARTPHONE NOT CONNECTED" << std::endl;
            }
        }
        else if (recep == "<STOP_CLIENT_TABLETTE>\n")
        {
            //get tablette client
            tmp = getClient(ID_TABLETTE);
            //if there is a tablette client
            if (tmp.m_id == ID_TABLETTE)
            {
                //send stop to client
                sendToClient(&tmp, STOP_CONN);
                //stopclient's thread
                tmp.stopThread();
                ID_TABLETTE_use = false;
                tmp = getClient("gtvrv");
                std::cout << "TABLETTE DISCONNECTED" << std::endl;
            }
            else
            {
                std::cout << "TABLETTE NOT CONNECTED" << std::endl;
            }
        }
        else if (recep == "<STOP_CLIENT_ALL>\n")
        { //stop all client without PC client
            for (uint8_t index = 0; index < clients->size(); ++index)
            {
                //check if the client isn't the PC client
                if (clients->at(index).init && (clients->at(index).m_id != ID_PC))
                {
                    //stop thread and delete client
                    clients->at(index).stopThread();
                    clients->erase(clients->begin() + index);
                }
            }
        }

        else if (recep == "<ALL>\n")
        {
            sendToClient(client, Manager::getInstance()->toString() + "\n");
        }
        else if (recep == "<CAR-POSITION>\n")
        {
            tostring = Manager::getInstance()->car->getPosition().first + "," + Manager::getInstance()->car->getPosition().second;
            sendToClient(client, "[" + tostring + "]" + "\n");
        }
        else if (recep == "" || recep == STOP_CONN)
        { //stop the client
            sendToClient(client, STOP_CONN);
            if (client != nullptr)
            {

                std::cout << "PC DISCONNECTED" << std::endl;

                ID_PC_use = false;

                delete client;

                client = nullptr;
            }
        }
        else
        {
            sendToClient(client, "unvalid commande");
        }

        //clear recep
        recep = "";
    }
    //stop thread
    pthread_exit(NULL);
}

static void *ComCAR(void *c)
{
    //get server
    Serveur *serv = Serveur::getInstance();

    Client *client = (Client *)c;
    //start car fonction
    serv->CAR(client);

    return NULL;
}

void Serveur::CAR(Client *client)
{
    std::string tostring = "";

    while (client != nullptr)
    {
        //receive data from car client
        std::string recep = receive(client);
        //show data
        std::cout << "receive data from client CAR : " + recep << std::endl;

        if (recep == "" || recep == STOP_CONN)
        {
            //stop the car client
            sendToClient(client, STOP_CONN);
            if (client != nullptr)
            {

                std::cout << "CAR DISCONNECTED" << std::endl;

                ID_CAR_use = false;
                delete client;
                client = nullptr;
            }
        }
        //clear recep
        recep = "";
    }
    //stop thread
    pthread_exit(NULL);
}

static void *ComRemote(void *c)
{
    //get server
    Serveur *serv = Serveur::getInstance();

    Client *client = (Client *)c;
    //start remote fonction
    serv->Remote(client);

    return NULL;
}

void Serveur::Remote(Client *client)
{
    std::string tostring = "";
    //bool use to check id the manual mode is send to the car
    bool manual_send = false;

    while (client != nullptr)
    {
        //get car client
        Client CAR = getClient(ID_CAR);
        //if theren't car client or car client disconect
        if (CAR.m_id != ID_CAR)
        {
            manual_send = false;
        }

        if (!(manual_send))
        {
            if (sendToClient(&CAR, MANUAL_MODE) > 0)
            {
                std::cout << "manual mode activated" << std::endl;
                manual_send = true;
            }
        }
        //receive data from client
        std::string recep = receive(client);
        //show data
        std::cout << "receive data from client REMOTE : " + recep << std::endl;

        if (!(PC_CONTROL))
        {
            if (recep == "H\n")
            {
                sendToClient(client, "HAUT\n");
                sendToClient(&CAR, "HA\n");
            }
            else if (recep == "B\n")
            {
                sendToClient(client, "BAS\n");
                sendToClient(&CAR, "BA\n");
            }
            else if (recep == "D\n")
            {
                sendToClient(client, "DROITE\n");
                sendToClient(&CAR, "DR\n");
            }
            else if (recep == "G\n")
            {
                sendToClient(client, "GAUCHE\n");
                sendToClient(&CAR, "GA\n");
            }
            else if (recep == "HD\n")
            {
                sendToClient(client, "HAUT DROITE\n");
                sendToClient(&CAR, "HD\n");
            }
            else if (recep == "HG\n")
            {
                sendToClient(client, "HAUT GAUCHE\n");
                sendToClient(&CAR, "HG\n");
            }
            else if (recep == "BD\n")
            {
                sendToClient(client, "BAS DROITE\n");
                sendToClient(&CAR, "BD\n");
            }
            else if (recep == "BG\n")
            {
                sendToClient(client, "BAS GAUCHE\n");
                sendToClient(&CAR, "BG\n");
            }
            else if (recep == "RD\n")
            {
                sendToClient(client, "ROTATION DROITE\n");
                sendToClient(&CAR, "RD\n");
            }
            else if (recep == "RG\n")
            {
                sendToClient(client, "ROTATION GAUCHE\n");
                sendToClient(&CAR, "RG\n");
            }
            else if (recep == "ST\n")
            {
                sendToClient(client, "STOP\n");
                sendToClient(&CAR, "ST\n");
            }
            else
            {
                sendToClient(client, "error\n");
            }
        }
        else if (PC_CONTROL)
        {
            sendToClient(client, "command disable by PC\n");
        }
        if (recep == "" || recep == STOP_CONN)
        {
            //stop client
            sendToClient(client, STOP_CONN);
            if (client != nullptr)
            {
                std::cout << "REMOTE DISCONNECTED" << std::endl;

                ID_REMOTE_use = false;
                delete client;
                client = nullptr;
            }
        }
        //clear recep
        recep = "";
    }
    //stop thread
    pthread_exit(NULL);
}

static void *ComAPPLI(void *c)
{
    //get server
    Serveur *serv = Serveur::getInstance();
    Client *client = (Client *)c;
    //start app fonction
    serv->APPLI(client);

    return NULL;
}

void Serveur::APPLI(Client *client)
{
    std::string tostring = "";
    bool mode_manuel = false;
    bool mode_auto = false;
    bool mode_virtuel = false;
    bool msg_recus = false;
    bool auto_go = false;
    bool virtuel_go = false;
    bool virtuel_matrix = false;

    Client *tmpappli = new Client();
    APPLI_CONTROL = tmpappli;
    while (client != nullptr)
    {
        //get car client
        Client CAR = getClient(ID_CAR);
        //receive data from app client
        std::string recep = receive(client);
        //show received data
        std::cout << "receive data from client APPLICATION : " + recep << std::endl;

        if (PC_CONTROL)
        {
            sendToClient(client, "command disable by PC\n");
        }

        else if (recep == MANUAL_MODE)
        {
            mode_manuel = true;
            mode_auto = false;
            sendToClient(client, "<OK_MANU>\n");
        }
        else if (recep == AUTO_MODE)
        {
            mode_manuel = false;
            mode_auto = true;
            sendToClient(client, "<OK_AUTO>\n");
        } //take the control of the car
        else if (recep == "<TAKE_CONTROL>\n")
        { //if the remote isn't connected
            if (!(ID_REMOTE_use))
            { //auto mode
                if (mode_auto)
                {
                    if (APPLI_CONTROL == tmpappli)
                    {
                        if (sendToClient(&CAR, AUTO_MODE) > 0)
                        {
                            std::cout << "mode auto activated" << std::endl;

                            APPLI_CONTROL = client;
                            sendToClient(client, "<OK_TAKE>\n");
                        }
                        else
                        {
                            sendToClient(client, "<FAIL_TAKE>\n");
                        }
                    }
                } //manual mode
                if (mode_manuel)
                {
                    if (APPLI_CONTROL == tmpappli)
                    {
                        if (sendToClient(&CAR, MANUAL_MODE) > 0)
                        {
                            std::cout << "mode manuelle activated" << std::endl;
                            APPLI_CONTROL = client;
                            sendToClient(client, "<OK_TAKE>\n");
                        }
                        else
                        {
                            sendToClient(client, "<FAIL_TAKE>\n");
                        }
                    }
                }
            }
            else
            {
                sendToClient(client, "<FAIL_TAKE>\n");
            }
        } //give back the control of the car
        else if (recep == "<GIVE_CONTROL>\n")
        {
            if (APPLI_CONTROL == client)
            {

                APPLI_CONTROL = tmpappli;

                sendToClient(client, "<OK_GIVE>\n");
            }
            else
            {
                sendToClient(client, "<FAIL_GIVE>\n");
            }
        }
        else if (mode_manuel && !(PC_CONTROL))
        { //if this client take the control of the car and the PC don't take the control
            if (!(ID_REMOTE_use) && APPLI_CONTROL == client)
            {
                if (recep == "HA\n")
                {
                    sendToClient(client, "HAUT\n");
                    msg_recus = true;
                    sendToClient(&CAR, "HA\n");
                }
                else if (recep == "BA\n")
                {
                    sendToClient(client, "BAS\n");
                    msg_recus = true;
                    sendToClient(&CAR, "BA\n");
                }
                else if (recep == "DR\n")
                {
                    sendToClient(client, "DROITE\n");
                    msg_recus = true;
                    sendToClient(&CAR, "DR\n");
                }
                else if (recep == "GA\n")
                {
                    sendToClient(client, "GAUCHE\n");
                    msg_recus = true;
                    sendToClient(&CAR, "GA\n");
                }
                else if (recep == "HD\n")
                {
                    sendToClient(client, "HAUT DROITE\n");
                    msg_recus = true;
                    sendToClient(&CAR, "HD\n");
                }
                else if (recep == "HG\n")
                {
                    sendToClient(client, "HAUT GAUCHE\n");
                    msg_recus = true;
                    sendToClient(&CAR, "HG\n");
                }
                else if (recep == "BD\n")
                {
                    sendToClient(client, "BAS DROITE\n");
                    msg_recus = true;
                    sendToClient(&CAR, "BD\n");
                }
                else if (recep == "BG\n")
                {
                    sendToClient(client, "BAS GAUCHE\n");
                    msg_recus = true;
                    sendToClient(&CAR, "BG\n");
                }
                else if (recep == "RD\n")
                {
                    sendToClient(client, "ROTATION DROITE\n");
                    msg_recus = true;
                    sendToClient(&CAR, "RD\n");
                }
                else if (recep == "RG\n")
                {
                    sendToClient(client, "ROTATION GAUCHE\n");
                    msg_recus = true;
                    sendToClient(&CAR, "RG\n");
                }
                else if (recep == "ST\n")
                {
                    sendToClient(client, "STOP\n");
                    msg_recus = true;
                    sendToClient(&CAR, "ST\n");
                }
            }
            else if ((ID_REMOTE_use))
            { //if the remote is connected send a message to the app client
                sendToClient(client, REMOTE_CONTROL);
            }
        }

        else if (mode_auto && !(PC_CONTROL))
        {
            if (!(ID_REMOTE_use) && APPLI_CONTROL == client)
            {
                if (recep == "<ALL>\n")
                {

                    sendToClient(client, Manager::getInstance()->toString() + "\n");
                }
                else if (recep == MANUAL_MODE)
                {
                    mode_manuel = true;
                    mode_auto = false;
                    sendToClient(client, "<OK_MANU>\n");
                }
                else if (recep == "<CAR_POSITION>\n")
                {
                    tostring = Manager::getInstance()->car->getPosition().first + "," + Manager::getInstance()->car->getPosition().second;
                    sendToClient(client, "[" + tostring + "]" + "\n");
                }
                else if (recep == "<GO>\n")
                {
                    //start auto mode using a auto matrix ( the car go to the ending point)
                }
                else
                {
                    sendToClient(client, "commande non valide\n");
                }
            }
            else if ((ID_REMOTE_use))
            { //if the remote is connected send a message to the app client
                sendToClient(client, REMOTE_CONTROL);
            }
        }
        else if (mode_virtuel && !(PC_CONTROL))
        {

            if (recep == "<matrix>\n")
            {

                //send to client the matrix
                virtuel_matrix = true;
            }

            if (recep == "<GO>\n")
            {

                // start auto mode using the virtual matrix
            }
        }
        else if (!(msg_recus))
        {
            sendToClient(client, "commande inconnue\n");
        }
        if (recep == "" || recep == STOP_CONN)
        { //stop app client
            sendToClient(client, STOP_CONN);

            if (client != nullptr)
            {
                if (APPLI_CONTROL == client)
                {
                    APPLI_CONTROL = tmpappli;
                }
                if (client->m_id == ID_SMARTPHONE)
                {
                    std::cout << "SMARTPHONE DISCONNECTED" << std::endl;
                    ID_SMARTPHONE_use = false;
                }
                else if (client->m_id == ID_TABLETTE)
                {
                    std::cout << "TABLETTE DISCONNECTED" << std::endl;
                    ID_TABLETTE_use = false;
                }
                delete client;
                client = nullptr;
            }
        }

        //clear recep
        recep = "";
        msg_recus = false;
    }
    //stop thread
    pthread_exit(NULL);
}