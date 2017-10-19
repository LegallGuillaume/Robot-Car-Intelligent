#ifndef SIMPLE_OBJECT_H
#define SIMPLE_OBJECT_H

#include <sstream>
#include <string>
#include <utility>

class SimpleObject{

public:
    virtual bool setPosition(int x, int y) =0;
    virtual std::pair<int, int> getPosition() =0;
    virtual std::string toString() =0;
};


#endif