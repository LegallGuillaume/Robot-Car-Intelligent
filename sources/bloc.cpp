#include <iostream>
#include <algorithm>
#include <sstream>
#include "../headers/manager/bloc.h"
#include "../headers/definition.h"

Bloc * Bloc::instance = nullptr;

Bloc * Bloc::getInstance(){
    if(instance == nullptr){
        instance = new Bloc();
    }
    return instance;
}

bool Bloc::add(int8_t x, int8_t y){
    list_bloc->push_back(std::make_pair(x,y));
    return std::find(list_bloc->begin(), list_bloc->end(), std::make_pair(x,y)) != list_bloc->end();
}

bool Bloc::remove(int8_t x, int8_t y){
    if(has(x,y)){
        list_bloc->erase(std::remove(list_bloc->begin(), list_bloc->end(), std::make_pair(x,y)), list_bloc->end());
        return !has(x,y);
    }
        return true; //default true if isn't in table.
}

bool Bloc::has(int8_t x, int8_t y){
    return std::find(list_bloc->begin(), list_bloc->end(), std::make_pair(x,y)) != list_bloc->end();
}

string Bloc::toString(){
    int16_t size = list_bloc->size();
    stringstream value;
    value << "";
    int16_t i =0;
    std::for_each(list_bloc->begin(), list_bloc->end(), [&](std::pair<int8_t, int8_t> _b){
        value << "{\""<<i<<"\": \""<<(int)_b.first<< "," <<(int)_b.second<<"\"}, ";
        ++i;
    });
    stringstream buf;
    buf <<"{ \"Number\": "<<size<<", \"Position\": "<<value.str().substr(0, value.str().size()-2)<<" }";
    return buf.str();
}
