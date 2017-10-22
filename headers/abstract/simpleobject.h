#ifndef SIMPLE_OBJECT_H
#define SIMPLE_OBJECT_H

#include <sstream>
#include <string>
#include <utility>

class SimpleObject{

public:
    virtual bool setPosition(int8_t x, int8_t y) =0;
    virtual std::pair<int8_t, int8_t> getPosition() =0;
    virtual std::string toString() =0;
};


#endif