#include <iostream>
#include "../headers/manager/arrive.h"

#include <string>
#include <sstream>

Arrive * Arrive::instance = nullptr;

Arrive * Arrive::getInstance(){
    if(instance == nullptr){
        instance = new Arrive();
    }
    return instance;
}

std::string Arrive::toString(){
    stringstream buf;
    buf<<"{ \"X\": "<<(int)getPosition().first<<", \"Y\": "<<(int)getPosition().second<<" }";
    return buf.str();
}