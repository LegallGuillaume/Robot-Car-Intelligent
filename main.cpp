#include <iostream>
#include "headers/manager.h"
#include "headers/traitement/pathfinding.h"
#include "headers/definition.h"
#include <unistd.h>
#include <utility>
#include <vector>

/*Unitest move to ---> unitest.cpp*/

int main(){
    Manager *manager = Manager::getInstance();
    manager->initScene(8);
    manager->bloc->add(1, 1);
    manager->bloc->add(3, 2);
    manager->bloc->add(4, 1);
    manager->bloc->add(5, 1);
    manager->bloc->add(7, 2);
    manager->bloc->add(1, 3);
    manager->bloc->add(3, 3);
    manager->bloc->add(5, 3);
    manager->bloc->add(1, 4);
    manager->bloc->add(3, 4);
    manager->bloc->add(6, 4);
    manager->bloc->add(7, 4);
    manager->bloc->add(1, 5);
    manager->bloc->add(4, 5);
    manager->bloc->add(1, 6);
    manager->bloc->add(2, 6);
    manager->bloc->add(5, 6);
    manager->bloc->add(6, 6);
    manager->bloc->add(6, 7);
    manager->bloc->add(0, 5);
    manager->arrive->setPosition(1, 7);
    //manager->bloc->add(0, 7);
    //manager->bloc->add(1, 6);
    //manager->bloc->add(2, 7);
    manager->car->setPosition(7, 7);
    manager->update();
    std::cout<<manager->forDevelopper()<<std::endl;
    PathFinding *path = new PathFinding(
        manager->getGeneralTable(), manager->getSceneCarrer(), manager->getSceneCarrer(), 
        manager->car->getPosition(), manager->arrive->getPosition()
    );
    if(!path->hasPossibility()){
        std::cout<<"NO POSSIBILITY"<<std::endl;
        return 1;
    }
    std::vector<std::pair<int8_t, int8_t>> *cheminTerminate = path->getChemin();
    for(int index=0; index < (int)cheminTerminate->size() - 1; ++index)
        manager->chemin->add(cheminTerminate->at(index).first, cheminTerminate->at(index).second);
//
    std::cout<<manager->toString()<<std::endl;
    manager->update();
    std::cout<<manager->forDevelopper()<<std::endl;
    return 0;
}