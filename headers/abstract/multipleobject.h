#ifndef MULTI_OBJECT_H
#define MULTI_OBJECT_H

#include <string>

class MultipleObject{

public:
    /**
     * @file                multipleobject.h
     * @brief               add Position with X and Y
     * @function            Virtual function
     * @version             1.0
     * 
     * @param x             position x 0 to 127
     * @param y             position y 0 to 127
     * @return true         true if succefully added
     * @return false        false if doesn't succefully added
     */
    virtual bool add(int8_t x, int8_t y) =0;

    /**
     * @file                multipleobject.h
     * @brief               remove Position with X and Y
     * @function            Virtual function
     * @version             1.0
     * 
     * @param x             position x 0 to 127
     * @param y             position y 0 to 127
     * @return true         true if succefully removed
     * @return false        false if doesn't succefully removed
     */
    virtual bool remove(int8_t x, int8_t y) =0; 

    /**
     * @file                multipleobject.h
     * @brief               has Position
     * @function            Virtual function
     * @version             1.0
     * 
     * @param x             position x 0 to 127
     * @param y             position y 0 to 127
     * @return true         has position set
     * @return false        has not position set
     */
    virtual bool has(int8_t x, int8_t y) =0;

    /**
     * @file                multipleobject.h
     * @brief               clear list of position
     * @function            Virtual function
     * @version             1.0
     * 
     * @return true         true if succefully clear
     * @return false        false if doesn't succefully clear
     */
    virtual bool clear() =0;
    
    /**
     * @file                multipleobject.h
     * @brief               return list of position
     * @function            Virtual function
     * @version             1.0
     * 
     * @return std::string  toString() function list to string
     */
    virtual std::string toString() =0;
};

#endif