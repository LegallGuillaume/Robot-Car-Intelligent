#include <iostream>
#include "headers/manager.h"
#include "headers/traitement/pathfinding.h"
#include "headers/definition.h"
#include <unistd.h>
#include <utility>
#include <vector>

void unitest(int8_t x, int8_t y){
    Manager *test1 = Manager::getInstance();
    Manager *test2 = Manager::getInstance();
    printf("No Duplicate Manager\t\t | \t\t%s\n", (
                                    test1->getInstance() == test2->getInstance() ? "OK" : "KO <-~-~-~"));
    printf("No Duplicate Car\t\t | \t\t%s\n", (
                                    test1->car->getInstance() == test2->car->getInstance() ? "OK" : "KO <-~-~-~"));
    printf("No Duplicate Bloc\t\t | \t\t%s\n", (
                                    test1->bloc->getInstance() == test2->bloc->getInstance() ? "OK" : "KO <-~-~-~"));
    Bloc *b = test1->bloc;
    printf("Add Bloc \tx%d y%d\t\t | \t\t%s\n",x,y, (b->add(x, y) ? "OK" : "KO <-~-~-~"));
    printf("Exist Bloc \tx%d y%d\t\t | \t\t%s\n",x,y, (b->has(x, y) ? "OK" : "KO <-~-~-~"));
    b->add(5, 3);
    b->add(1, 7);
    std::string comp = "{ \"Number\": 3, \"Position\": {\"0\": \"" +
                       to_string(x) + "," + to_string(y) + "\"}, {\"1\": \"5,3\"}, {\"2\": \"1,7\"} }";
    printf("toString function\t\t | \t\t%s\n", (comp == b->toString() ? "OK" : "KO <-~-~-~"));
    printf("Remove Bloc \tx%d y%d\t\t | \t\t%s\n",x,y, (b->remove(x, y) ? "OK" : "KO <-~-~-~"));
    printf("Exist Bloc \tx%d y%d\t\t | \t\t%s\n",x,y, (!b->has(x, y) ? "OK" : "KO <-~-~-~"));
    ++x;
    --y;
    Car *car = test1->car;
    printf("Default Position Car x-1 y-1\t | \t\t%s\n", (car->getPosition().first == -1 ? "OK" : "KO <-~-~-~"));
    car->setPosition(x, y);
    printf("Position Car \tx%d y%d\t\t | \t\t%s\n",x,y, (
                        car->getPosition().first == x && car->getPosition().second == y  ? "OK" : "KO <-~-~-~"));
    delete test1;
    test1 = nullptr;
    printf("Delete Manager\t\t\t | \t\t%s\n", (test1 == nullptr ? "OK" : "KO <-~-~-~"));
}



int main(){
    //unitest(3, 5);
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
    //manager->bloc->add(2, 7);
    //manager->bloc->add(1, 8);
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