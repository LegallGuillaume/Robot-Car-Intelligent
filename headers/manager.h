#ifndef MANAGER_H
#define MANAGER_H

#include "manager/car.h"
#include "manager/bloc.h"

class Manager{

public:
    static Manager * getInstance();
    Car *car;
    Bloc *bloc;
    int** getTable(){
        return generalTable;
    }

private:
    int **generalTable;
    static Manager * instance;
    Manager(){
        car = Car::getInstance();
        bloc = Bloc::getInstance();
        generalTable = new int*();
    }
};

#endif