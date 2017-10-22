#ifndef MULTI_OBJECT_H
#define MULTI_OBJECT_H

#include <string>

class MultipleObject{

public:
    virtual bool add(int8_t x, int8_t y) =0;
    virtual bool remove(int8_t x, int8_t y) =0; 
    virtual bool has(int8_t x, int8_t y) =0;
    virtual bool clear() =0;
    virtual std::string toString() =0;
};

#endif