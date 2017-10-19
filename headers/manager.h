#ifndef MANAGER_H
#define MANAGER_H

#include "manager/car.h"
#include "manager/bloc.h"
#include "manager/chemin.h"
#include <string>

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
    int** getTable(){
        return generalTable;
    }
    void update();
    int getSceneCarrer(){
        return scenecarrer;
    }
    void initScene(int nb_carrer){
        scenecarrer = nb_carrer;
    }

    bool sceneIsDefine(){
        return scenecarrer > -1;
    }

    std::string toString();

  private:
    int **generalTable;
    static Manager * instance;
    int scenecarrer;
    void cleanMatrice();
    Manager()
    {
        car = Car::getInstance();
        bloc = Bloc::getInstance();
        chemin = Chemin::getInstance();
        generalTable = new int*();
        scenecarrer = -1;
    }
};

#endif