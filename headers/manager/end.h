#ifndef END_H
#define END_H

#include <utility> //pair<>
#include <string>
#include "../abstract/simpleobject.h"

using namespace std;

/**
 * @file                end.h
 * @brief               Abstract of SimpleObject
 * @function            Virtual function
 * @version             1.0
 */
class End : public SimpleObject{

public:
    /**
     * @file                end.h
     * @brief               Design patern singleton
     * @function            Singleton function
     * @version             1.0
     * 
     * @return End*       have Arrival
     */
    static End * getInstance();

    bool setPosition(int8_t x, int8_t y) override{
        position->first = x;
        position->second = y;
        return (position->first == x && position->second == y);
    }
    pair<int8_t, int8_t> getPosition() override{
        return *position;
    }
    string toString() override;
    ~End(){
        delete position;
        position = nullptr;
        delete instance;
        instance=nullptr;
    }
    
private:
    static End * instance;
    pair<int8_t, int8_t> *position;
    End(){
        position = new pair<int8_t, int8_t>();
        position->first = -1;
        position->second = -1;
    }
};

#endif