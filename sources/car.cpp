#include <iostream>
#include "../headers/manager/car.h"

#include <string>
#include <sstream>

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

std::string Car::toString(){
    stringstream buf;
    buf<<"\"Voiture\": { \"X\": "<<(int)getPosition().first<<", \"Y\": "<<(int)getPosition().second<<" }";
    return buf.str();
}