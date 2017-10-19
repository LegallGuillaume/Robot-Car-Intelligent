#ifndef ARRIVE_H
#define ARRIVE_H

#include <utility> //pair<>
#include <string>
#include "../abstract/simpleobject.h"

using namespace std;

class Arrive : public SimpleObject{

public:
    static Arrive * getInstance();
    bool setPosition(int x, int y) override{
        position->first = x;
        position->second = y;
    }
    pair<int, int> getPosition() override{
        return *position;
    }
    string toString() override;
    
private:
    static Arrive * instance;
    pair<int, int> *position;
    Arrive(){
        position = new pair<int, int>();
        position->first = -1;
        position->second = -1;
    }
};

#endif