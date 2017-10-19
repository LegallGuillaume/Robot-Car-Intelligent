#include <iostream>
#include "../headers/manager/car.h"

Car * Car::instance = nullptr;

Car * Car::getInstance(){
    if(instance == nullptr){
        instance = new Car();
    }
    return instance;
}

void Car::sendCommand(){
    //TODO send command from wifi to Car
}