#ifndef MANAGER_H
#define MANAGER_H

#include "manager/car.h"
#include "manager/bloc.h"
#include "manager/chemin.h"
#include "manager/arrive.h"
#include <string>
#include <utility>
#include <stdlib.h>


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

    int getCoordonate(int x, int y){
        return generalTable[x][y];
    }
    void update();

    int getSceneCarrer(){
        return scenecarrer;
    }
    void initScene(int nb_carrer){
        scenecarrer = nb_carrer;
        generalTable = (int**)malloc(nb_carrer * sizeof(*generalTable));
        if(generalTable == NULL){
            std::cout << "ERREUR ALLOC Manager TABLES" << std::endl;
            return;
        }
        for(int i = 0; i < nb_carrer; ++i){
            generalTable[i] = (int*)malloc(nb_carrer * sizeof(**generalTable) );       //On alloue des tableaux de 'taille2' variables.
            if(generalTable[i] == NULL){                              //En cas d'erreur d'allocation
                for(int a=0 ; a < nb_carrer ; a++){
                     free(generalTable[a]);
                }
            }
        }
    }

    bool sceneIsDefine(){
        return scenecarrer > -1;
    }

    std::string toString();
    std::string forDevelopper();

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
        arrive = Arrive::getInstance();
        scenecarrer = -1;
    }
};

#endif