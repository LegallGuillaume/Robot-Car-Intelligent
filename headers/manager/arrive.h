#ifndef ARRIVE_H
#define ARRIVE_H

#include <utility> //pair<>
#include <string>
#include "../abstract/simpleobject.h"

using namespace std;

class Arrive : public SimpleObject{

public:
    static Arrive * getInstance();
    bool setPosition(int8_t x, int8_t y) override{
        position->first = x;
        position->second = y;
    }
    pair<int8_t, int8_t> getPosition() override{
        return *position;
    }
    string toString() override;
    ~Arrive(){
        delete position;
        position = nullptr;
        delete instance;
        instance=nullptr;
    }
    
private:
    static Arrive * instance;
    pair<int8_t, int8_t> *position;
    Arrive(){
        position = new pair<int8_t, int8_t>();
        position->first = -1;
        position->second = -1;
    }
};

#endif