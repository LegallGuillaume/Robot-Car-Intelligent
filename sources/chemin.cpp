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

bool Chemin::add(int x, int y){
    list_chemin->push_back(std::make_pair(x,y));
    return std::find(list_chemin->begin(), list_chemin->end(), std::make_pair(x,y)) != list_chemin->end();
}

bool Chemin::remove(int x, int y){
    if(has(x,y)){
        list_chemin->erase(std::remove(list_chemin->begin(), list_chemin->end(), std::make_pair(x,y)), list_chemin->end());
        return !has(x,y);
    }
        return true; //default true if isn't in table.
}

bool Chemin::has(int x, int y){
    return std::find(list_chemin->begin(), list_chemin->end(), std::make_pair(x,y)) != list_chemin->end();
}

string Chemin::toString(){
    int size = list_chemin->size();
    stringstream value;
    value << "";
    std::for_each(list_bloc->begin(), list_bloc->end(), [&](std::pair<int, int> _b){
        value << "{\""<<i<<"\": \""<<_b.first<< "," <<_b.second<<"\"}, ";
        ++i;
    });
    stringstream buf;
    buf <<"{ \"Number\": "<<size<<", \"Position\": "<<value.str().substr(0, value.str().size()-2)<<" }";
    return buf.str();
}