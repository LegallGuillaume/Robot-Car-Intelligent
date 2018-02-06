#include <iostream>
#include "../headers/manager/end.h"

#include <string>
#include <sstream>

End * End::instance = nullptr;

End * End::getInstance(){
    if(instance == nullptr){
        instance = new End();
    }
    return instance;
}

std::string End::toString(){
    stringstream buf;
    buf<<"\"Arrive\": { \"X\": "<<(int)getPosition().first<<", \"Y\": "<<(int)getPosition().second<<" }";
    return buf.str();
}