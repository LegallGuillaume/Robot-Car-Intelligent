#ifndef CAR_H
#define CAR_H

#include <utility> //pair<>
#include <string>
#include "../abstract/simpleobject.h"

using namespace std;

/**
 * @file                car.h
 * @brief               Abstract of SimpleObject
 * @function            Virtual function
 * @version             1.0
 */
class Car : public SimpleObject{

public:
    static Car * getInstance();
    void sendCommand(); //send command to car from position X & Y
    bool setPosition(int8_t x, int8_t y) override{
        position->first = x;
        position->second = y;
    }
    pair<int8_t, int8_t> getPosition() override{
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
    pair<int8_t, int8_t> *position;
    Car(){
        position = new pair<int8_t, int8_t>();
        position->first = -1;
        position->second = -1;
    }
};

#endif