#ifndef CAR_H
#define CAR_H

#include <utility> //pair<>

using namespace std;

class Car{

public:
    static Car * getInstance();
    void sendCommand(); //send command to car from position X & Y
    bool setPosition(int x, int y){
        position->first = x;
        position->second = y;
    }
    pair<int, int> getPosition(){
        return *position;
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