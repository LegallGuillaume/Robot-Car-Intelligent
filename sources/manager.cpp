#include <iostream>
#include "../headers/manager.h"

Manager * Manager::instance = nullptr;

Manager * Manager::getInstance(){
    if(instance == nullptr){
        instance = new Manager();
    }
    return instance;
}

