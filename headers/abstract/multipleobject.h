#ifndef MULTI_OBJECT_H
#define MULTI_OBJECT_H

#include <string>

class MultipleObject{

public:
    virtual bool add(int x, int y) =0;
    virtual bool remove(int x, int y) =0; 
    virtual bool has(int x, int y) =0;
    virtual bool clear() =0;
    virtual std::string toString() =0;
};

#endif