#include <iostream>
#include <sstream>
#include "../headers/manager.h"
#include "../headers/definition.h"

Manager * Manager::instance = nullptr;

Manager * Manager::getInstance(){
    if(instance == nullptr){
        instance = new Manager();
    }
    return instance;
}

void Manager::update(){
    if(!areaIsDefine())
        return;
    cleanMatrix();
    
    /*Block*/
    for (int16_t index = 0; index < block->list_block->size(); ++index){
        generalTable[block->list_block->at(index).first][block->list_block->at(index).second] = BLOC_DEF;
    }

    /*path*/
    for (int16_t index = 0; index < path->list_path->size(); ++index){
        generalTable[path->list_path->at(index).first][path->list_path->at(index).second] = CHEMIN_DEF;
    }

    /*Car*/
    if(car->getPosition().first >= 0)
        generalTable[car->getPosition().first][car->getPosition().second] = CAR_DEF;

    /*end*/
    if(end->getPosition().first >= 0)
        generalTable[end->getPosition().first][end->getPosition().second] = ARRIVE_DEF;
}

void Manager::cleanMatrix(){
    for (int8_t x = 0; x < getSceneSquare(); ++x){
        for (int8_t y = 0; y < getSceneSquare(); ++y){
            generalTable[x][y] = 0;
        }
    }
}

std::string Manager::forDevelopper(){
    stringstream buf;
    buf << "---|---|---|---|---|---|---|---\n";
    for (int8_t x = 0; x < getSceneSquare();++x){
        for (int8_t y = 0; y < getSceneSquare();++y){
            buf << "-" + to_string(generalTable[x][y]) + "-|";
        }
        buf << "\n";
    }
    buf << "---|---|---|---|---|---|---|---\n";
    return buf.str();
}

std::string Manager::toString(){
    stringstream buf;
    buf << "{";
      buf <<"\"Scene\": "<<(int)scenesquare<<",";
      buf <<block->toString()<<",";
      buf <<end->toString()<<",";
      buf <<car->toString()<<",";
      buf <<path->toString()<<"";
      buf << "}";
    return buf.str();
}


void Manager::reset(){
    cleanMatrix();
    car->setPosition(-1,-1);
    end->setPosition(-1,-1);
    block->clear();
    path->clear();
}