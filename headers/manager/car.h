#ifndef CAR_H
#define CAR_H

#include <utility> //pair<>
#include <string>
#include "../abstract/simpleobject.h"

using namespace std;

class Car : public SimpleObject{

public:
    static Car * getInstance();
    void sendCommand(); //send command to car from position X & Y
    bool setPosition(int x, int y) override{
        position->first = x;
        position->second = y;
    }
    pair<int, int> getPosition() override{
        return *position;
    }
    string toString() override;
    ~Car(){
        delete position;
        position = nullptr;
        delete instance;
        instance = nullptr;
    }
private:
    static Car * instance;
    pair<int, int> *position;
    Car(){
        position = new pair<int, int>();
        position->first = -1;
        position->second = -1;
    }
};

#endif