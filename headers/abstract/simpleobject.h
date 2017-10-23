/*Nombre d'otect en Allocation Max de la class:
 *
 *    - int8_t (1octet)                       : 004 octets
 *    - string (1octet/lettre) (127*6)octet   : 762 octets
 *    - bool (1octet)                         : 001 octets
 *
 * = 765 octets
*/
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