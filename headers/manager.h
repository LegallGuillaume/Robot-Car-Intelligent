#ifndef MANAGER_H
#define MANAGER_H

#include "manager/car.h"
#include "manager/bloc.h"
#include "manager/chemin.h"
#include "manager/arrive.h"
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

    int8_t getCoordonate(int8_t x, int8_t y){
        return generalTable[x][y];
    }
    void update();

    int8_t** getGeneralTable(){
        return generalTable;
    }

    int8_t getSceneCarrer(){
        return scenecarrer;
    }
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

    std::string toString();
    std::string forDevelopper();
    ~Manager(){
        for(int8_t i=0; i<scenecarrer; i++){
            delete [] generalTable[i];
            generalTable[i] = nullptr;
        }
        delete []generalTable;
        generalTable = nullptr;
    }
private:
    int8_t **generalTable;
    static Manager * instance;
    int8_t scenecarrer;
    void cleanMatrice();

    Manager()
    {
        car = Car::getInstance();
        bloc = Bloc::getInstance();
        chemin = Chemin::getInstance();
        arrive = Arrive::getInstance();
        scenecarrer = -1;
    }
};

#endif