#include <iostream>
#include <algorithm>
#include <sstream>
#include "../headers/manager/path.h"
#include "../headers/definition.h"

Path * Path::instance = nullptr;

Path * Path::getInstance(){
    if(instance == nullptr){
        instance = new Path();
    }
    return instance;
}

bool Path::add(int8_t x, int8_t y){
    list_path->push_back(std::make_pair(x,y));
    return std::find(list_path->begin(), list_path->end(), std::make_pair(x,y)) != list_path->end();
}

bool Path::remove(int8_t x, int8_t y){
    if(has(x,y)){
        list_path->erase(std::remove(list_path->begin(), list_path->end(), std::make_pair(x,y)), list_path->end());
        return !has(x,y);
    }
        return true; //default true if isn't in table.
}

bool Path::has(int8_t x, int8_t y){
    return std::find(list_path->begin(), list_path->end(), std::make_pair(x,y)) != list_path->end();
}

string Path::toString(){
    int16_t size = list_path->size();
    stringstream value;
    value << "";
    int16_t i =0;
    std::for_each(list_path->begin(), list_path->end(), [&](std::pair<int8_t, int8_t> _b){
        value << "\""<<i<<"\": \""<<(int)_b.first<< "," <<(int)_b.second<<"\", ";
        ++i;
    });
    stringstream buf;
    buf <<" \"Chemin\": { \"Number\": "<<size<<", \"Position\": {"<<value.str().substr(0, value.str().size()-2)<<"} }";
    return buf.str();
}