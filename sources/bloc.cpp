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

bool Bloc::add(int x, int y){
    list_bloc->push_back(std::make_pair(x,y));
    return std::find(list_bloc->begin(), list_bloc->end(), std::make_pair(x,y)) != list_bloc->end();
}

bool Bloc::remove(int x, int y){
    if(has(x,y)){
        list_bloc->erase(std::remove(list_bloc->begin(), list_bloc->end(), std::make_pair(x,y)), list_bloc->end());
        return !has(x,y);
    }
        return true; //default true if isn't in table.
}

bool Bloc::has(int x, int y){
    return std::find(list_bloc->begin(), list_bloc->end(), std::make_pair(x,y)) != list_bloc->end();
}

string Bloc::toString(){
    int size = list_bloc->size();
    stringstream value;
    value << "";
    for(int i=0; i<size; ++i)
        value << "{\""<<i<<"\": \""<<list_bloc->at(i).first<< "," <<list_bloc->at(i).second<<"\"}, ";
    stringstream buf;
    buf <<"{ \"Number\": "<<size<<", \"Position\": "<<value.str().substr(0, value.str().size()-2)<<" }";
    return buf.str();
}

/*
void Bloc::update_tab(){
    int size = list_bloc->size();
    int y = 0;
    for(int x=0; x<size; ++x){
        tab_bloc[list_bloc->at(x).first][list_bloc->at(x).second] = BLOC_DEF;
    }
}
*/