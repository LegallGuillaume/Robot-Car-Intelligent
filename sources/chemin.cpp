#include <iostream>
#include <algorithm>
#include <sstream>
#include "../headers/manager/chemin.h"
#include "../headers/definition.h"

Chemin * Chemin::instance = nullptr;

Chemin * Chemin::getInstance(){
    if(instance == nullptr){
        instance = new Chemin();
    }
    return instance;
}

bool Chemin::add(int8_t x, int8_t y){
    list_chemin->push_back(std::make_pair(x,y));
    return std::find(list_chemin->begin(), list_chemin->end(), std::make_pair(x,y)) != list_chemin->end();
}

bool Chemin::remove(int8_t x, int8_t y){
    if(has(x,y)){
        list_chemin->erase(std::remove(list_chemin->begin(), list_chemin->end(), std::make_pair(x,y)), list_chemin->end());
        return !has(x,y);
    }
        return true; //default true if isn't in table.
}

bool Chemin::has(int8_t x, int8_t y){
    return std::find(list_chemin->begin(), list_chemin->end(), std::make_pair(x,y)) != list_chemin->end();
}

string Chemin::toString(){
    int16_t size = list_chemin->size();
    stringstream value;
    value << "";
    int16_t i =0;
    std::for_each(list_chemin->begin(), list_chemin->end(), [&](std::pair<int8_t, int8_t> _b){
        value << "\""<<i<<"\": \""<<(int)_b.first<< "," <<(int)_b.second<<"\", ";
        ++i;
    });
    stringstream buf;
    buf <<" \"Chemin\": { \"Number\": "<<size<<", \"Position\": {"<<value.str().substr(0, value.str().size()-2)<<"} }";
    return buf.str();
}