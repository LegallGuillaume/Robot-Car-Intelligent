#ifndef MANAGER_H
#define MANAGER_H

#include "manager/car.h"
#include "manager/bloc.h"
#include "manager/chemin.h"
#include "manager/arrive.h"
//#include "serveur/share_server.h"
#include <string>
#include <utility>
#include <stdlib.h>
#include <iostream>


/**
 * Example of Scene Matrice
 *          Scene = 6x6
 *          (X,Y)
 * 
 *  (0,0) | (0,1) | (0,2) | (0,3) | (0,4) | (0,5)
 * -------|-------|-------|-------|-------|-------
 *  (1,0) | (1,1) | (1,2) | (1,3) | (1,4) | (1,5)
 * -------|-------|-------|-------|-------|-------
 *  (2,0) | (2,1) | (2,2) | (2,3) | (2,4) | (2,5)
 * -------|-------|-------|-------|-------|-------
 *  (3,0) | (3,1) | (3,2) | (3,3) | (3,4) | (3,5)
 * -------|-------|-------|-------|-------|-------
 *  (4,0) | (4,1) | (4,2) | (4,3) | (4,4) | (4,5)
 * -------|-------|-------|-------|-------|-------
 *  (5,0) | (5,1) | (5,2) | (5,3) | (5,4) | (5,5)
 * 
*/
class Manager{

public:
    static Manager * getInstance();
    Car *car;
    Bloc *bloc;
    Chemin *chemin;
    Arrive *arrive;
    //sServer *server;

    /**
     *
     * @file                manager.h
     * @brief               allow to know *_DEF into the matrix (include "definition.h")
     * @version             1.0
     *
     * 
     * @param x             position X 0 to 127
     * @param y             position Y 0 to 127
     * @return int8_t       return (BLOC_DEF, VIDE_DEF, CAR_DEF, ARRIVE_DEF, ...) 
     */
    int8_t getCoordonate(int8_t x, int8_t y){
        return generalTable[x][y];
    }

    /**
     * @file                manager.h
     * @brief               update add all block, car, arrive, path into matrix
     * @version             1.0 
     */
    void update();

    /**
     * @file                manager.h
     * @brief               have the Matrix
     * @version             1.0
     * 
     * @return int8_t**     return the matrix max size 127x127
     */
    int8_t** getGeneralTable(){
        return generalTable;
    }

    /**
     * @file                manager.h
     * @brief               have the size of scene
     * @version             1.0
     * 
     * @return int8_t       return size 0 to 127
     */
    int8_t getSceneCarrer(){
        return scenecarrer;
    }

    /**
     * @file                manager.h
     * @brief               create the Matrix
     * @version             1.0
     * 
     * @return true         matrix correctly created
     * @return false        matrix not correctly created
     */
    bool initScene(int8_t nb_carrer){
        scenecarrer = nb_carrer;
        bool boolean=true;
        uint8_t allocmem = 0;
        try{
            generalTable = new int8_t * [nb_carrer];
            std::fill_n( generalTable, nb_carrer, static_cast<int8_t*>(0));
            for(allocmem = 0; allocmem < nb_carrer; ++allocmem){
                generalTable[ allocmem ] = new int8_t[nb_carrer];
            }
        }catch( const std::bad_alloc &) {
            for( uint8_t i = 0; i < allocmem; ++i ){
                delete [] generalTable[ i ];
            }
            delete [] generalTable;
            throw;
        }
        return boolean;
    }


    bool sceneIsDefine(){
        return scenecarrer > -1;
    }

    /**
     * @file                manager.h
     * @brief               Display info send to remote control (APPLI)
     * @version             1.0
     * 
     * @return std::string  String to send in remote control
     */
    std::string toString();

    /**
     * @file                manager.h
     * @brief               Display the matrix into terminal
     * @version             1.0
     * 
     * @return std::string  String to send in remote control
     */
    std::string forDevelopper();

    /**
     * @file                manager.h
     * @brief               Destructor of the class
     * @version             1.0
     */
    ~Manager(){
        for(int8_t i=0; i<scenecarrer; i++){
            delete [] generalTable[i];
            generalTable[i] = nullptr;
        }
        delete []generalTable;
        generalTable = nullptr;
    }
    void reset();

private:
    int8_t **generalTable;
    static Manager * instance;
    int8_t scenecarrer;
    void cleanMatrice();

    /**
     * @file                manager.h
     * @brief               Singleton Manager()
     * @version             1.0
     */
    Manager()
    {
        car = Car::getInstance();
        bloc = Bloc::getInstance();
        chemin = Chemin::getInstance();
        arrive = Arrive::getInstance();
        //server = sServer::getInstance();
        scenecarrer = -1;
    }
};

#endif