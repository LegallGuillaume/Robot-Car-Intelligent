#include <iostream>
#include "headers/manager.h"
#include "headers/traitement/pathfinding.h"
#include "headers/definition.h"
#include <unistd.h>
#include <utility>
#include <vector>


/*
        ---------UNITEST USAGE---------
        Usage:
        
        ./unitest images                >     Display Unitest from Images processing

        ./unitest pathfinding <X> <Y>   >     Display Unitest from Pathfinding processing
            <X> <Y> coordonate of a block (x2 y2 to have not possibility, 0 >= x and y < 4)
        ---------UNITEST USAGE---------
*/

void unitest_pathfinding(int8_t x, int8_t y){
    Manager *test1 = Manager::getInstance();
    Manager *test2 = Manager::getInstance();
    test1->initScene(4);
    printf("No Duplicate Manager\t\t | \t\t%s\n", (
                                    test1->getInstance() == test2->getInstance() ? "OK" : "KO <-~-~-~"));
    printf("No Duplicate Car\t\t | \t\t%s\n", (
                                    test1->car->getInstance() == test2->car->getInstance() ? "OK" : "KO <-~-~-~"));
    printf("No Duplicate Bloc\t\t | \t\t%s\n", (
                                    test1->bloc->getInstance() == test2->bloc->getInstance() ? "OK" : "KO <-~-~-~"));
    Bloc *b = test1->bloc;
    printf("Add Bloc \tx%d y%d\t\t | \t\t%s\n",x,y, (b->add(x, y) ? "OK" : "KO <-~-~-~"));
    printf("Exist Bloc \tx%d y%d\t\t | \t\t%s\n",x,y, (b->has(x, y) ? "OK" : "KO <-~-~-~"));
    b->add(0, 2);
    b->add(1, 2);
    b->add(3, 2);
    std::string comp = "{ \"Number\": 4, \"Position\": {\"0\": \"" +
                       to_string(x) + "," + to_string(y) + "\"}, {\"1\": \"0,2\"}, {\"2\": \"1,2\"}, {\"3\": \"3,2\"} }";
    printf("toString function\t\t | \t\t%s\n", (comp == b->toString() ? "OK" : "KO <-~-~-~"));
    printf("Remove Bloc \tx%d y%d\t\t | \t\t%s\n",x,y, (b->remove(x, y) ? "OK" : "KO <-~-~-~"));
    printf("Dont Exist Bloc \tx%d y%d\t | \t\t%s\n",x,y, (!b->has(x, y) ? "OK" : "KO <-~-~-~"));
    b->add(x, y);
    test1->update();
    Car *car = test1->car;
    printf("Default Position Car x-1 y-1\t | \t\t%s\n", (car->getPosition().first == -1 ? "OK" : "KO <-~-~-~"));
    car->setPosition(3, 0);
    printf("Position Car \tx%d y%d\t\t | \t\t%s\n",3,0, (
                        car->getPosition().first == 3 && car->getPosition().second == 0  ? "OK" : "KO <-~-~-~"));
    Arrive *arrive = test1->arrive;
    printf("Default Position Arrive x-1 y-1\t | \t\t%s\n", (arrive->getPosition().first == -1 ? "OK" : "KO <-~-~-~"));
    arrive->setPosition(0, 3);
    printf("Position Arrive \tx%d y%d\t | \t\t%s\n",0,3, (
                        arrive->getPosition().first == 0 && arrive->getPosition().second == 3  ? "OK" : "KO <-~-~-~"));
    test1->update();
    PathFinding *path = new PathFinding(
        test1->getGeneralTable(), test1->getSceneCarrer(), test1->getSceneCarrer(), 
        car->getPosition(), arrive->getPosition()
    );
    if(path->hasPossibility()){
        printf("Is Possible\t\t\t | \t\tOK\n");
        std::vector<std::pair<int8_t, int8_t>> *cheminTerminate = path->getChemin();
        for(int8_t index=0; index < (int8_t)cheminTerminate->size() - 1; ++index)
            test1->chemin->add(cheminTerminate->at(index).first, cheminTerminate->at(index).second);
        test1->update();
        printf("Chemin Correct\t\t\t | \t\t%s\n", ((int)cheminTerminate->size() == 6 ? "OK" : "KO <-~-~-~"));
    }else{
        printf("Not Possible\t\t\t | \t\tOK\n");
    }
    std::cout<<test1->forDevelopper()<<std::endl;
    delete test1;
    test1 = nullptr;
    printf("Delete Manager\t\t\t | \t\t%s\n", (test1 == nullptr ? "OK" : "KO <-~-~-~"));
}

int main(int argc, char *argv[]){
    if(argc > 1){
        if(argc == 2){ // ./unitest images
            printf("[WAIT] No unitest for images processing NOW\n");
            return 0;
        }
        else if(argc == 4){ // ./unitest pathfinding <X> <Y>
            int x = atoi(argv[2]);
            int y = atoi(argv[3]);
            if(x < 4 && x >= 0 && y >= 0 && y < 4){
                unitest_pathfinding((int8_t)x, (int8_t)y);
                return 0;
            }
        }

    }
    printf("\n\t\t---------UNITEST USAGE---------\n\tUsage:\n\n\t./unitest images\t\t>\tDisplay Unitest from Images processing\n\n\t./unitest pathfinding <X> <Y>\t>\tDisplay Unitest from Pathfinding processing\n\t\t<X> <Y> coordonate of a block (x2 y2 to have not possibility, 0 >= x and y < 4)\n\t\t---------UNITEST USAGE---------\n");
    return -1;
}