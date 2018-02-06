#ifndef SIMPLE_OBJECT_H
#define SIMPLE_OBJECT_H

#include <sstream>
#include <string>
#include <utility>

class SimpleObject{

public:
    /**
     * @file                simpleobject.h
     * @brief               set the position of object (car, arrival, start)
     * @function            Virtual function
     * @version             1.0
     * 
     * 
     * @param x             position x 0 to 127
     * @param y             position y 0 to 127
     * @return true         true if position correctly change
     * @return false        false if position not correctly change
     */
    virtual bool setPosition(int8_t x, int8_t y) =0;

    /**
     * @file                simpleobject.h
     * @brief               get the position of object (car, arrival, start)
     * @function            Virtual function
     * @version             1.0
     * 
     * @return std::pair<int8_t, int8_t> return the position of object
     */
    virtual std::pair<int8_t, int8_t> getPosition() =0;

    /**
     * @file                simpleobject.h
     * @brief               return the position to String
     * @function            Virtual function
     * @version             1.0
     * 
     * @return std::string  toString() function position to string
     */
    virtual std::string toString() =0;
};


#endif