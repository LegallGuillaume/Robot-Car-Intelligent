#include <iostream>
#include "headers/manager.h"
#include "headers/definition.h"
#include <unistd.h>

void unitest(int x, int y){
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
    //unitest(5, 8);
    Manager *manager = Manager::getInstance();
    manager->initScene(8);
    manager->car->setPosition(0, 0);
    manager->arrive->setPosition(7, 5);
    manager->bloc->add(5, 2);
    manager->bloc->add(1, 7);
    manager->bloc->add(2, 1);
    manager->update();
    std::cout<<manager->forDevelopper()<<std::endl;
    int x=0;
    int y=0;
    int findit = 0;
    while(true){
        usleep(10000*60);
        ++findit;
        if(y >= manager->getSceneCarrer() - 1 || x >= manager->getSceneCarrer() - 1){
            x=0;
            y=2;
        }else{
            x = (rand()%manager->getSceneCarrer()); 
            y = (rand()%manager->getSceneCarrer()); 
        }
        manager->car->setPosition(x, y);
        manager->update();
        std::cout<<manager->forDevelopper()<<std::endl;
        if(manager->car->getPosition() == manager->arrive->getPosition()){
            std::cout<<"ARRIVER TROUVée !!! en "<< findit <<" coups"<<std::endl;
            break;
        }
    }

    return 0;
}