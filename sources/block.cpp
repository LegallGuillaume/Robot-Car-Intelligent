#include <iostream>
#include <algorithm>
#include <sstream>
#include "../headers/manager/block.h"
#include "../headers/definition.h"

Block * Block::instance = nullptr;

Block * Block::getInstance(){
    if(instance == nullptr){
        instance = new Block();
    }
    return instance;
}

bool Block::add(int8_t x, int8_t y){
    list_block->push_back(std::make_pair(x,y));
    return std::find(list_block->begin(), list_block->end(), std::make_pair(x,y)) != list_block->end();
}

bool Block::remove(int8_t x, int8_t y){
    if(has(x,y)){
        list_block->erase(std::remove(list_block->begin(), list_block->end(), std::make_pair(x,y)), list_block->end());
        return !has(x,y);
    }
        return true; //default true if isn't in table.
}

bool Block::has(int8_t x, int8_t y){
    return std::find(list_block->begin(), list_block->end(), std::make_pair(x,y)) != list_block->end();
}

string Block::toString(){
    int16_t size = list_block->size();
    stringstream value;
    value << "";
    int16_t i =0;
    std::for_each(list_block->begin(), list_block->end(), [&](std::pair<int8_t, int8_t> _b){
        value << "\""<<i<<"\": \""<<(int)_b.first<< "," <<(int)_b.second<<"\", ";
        ++i;
    });
    stringstream buf;
    buf <<" \"Block\": { \"Number\": "<<size<<", \"Position\": {"<<value.str().substr(0, value.str().size()-2)<<"} }";
    return buf.str();
}
