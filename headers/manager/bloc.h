/*Nombre d'otect en Allocation Max de la class:
 *
 *    - int8_t (1octet)                       : 013 octets
 *    - string (1octet/lettre) (1*6)octet     : 006 octets
 *    - bool (1octet)                         : 006 octets
 *    - vector (2otet)                        : 060 octets
 *
 * = 085 octets
*/

#ifndef BLOC_H
#define BLOC_H

#include <utility> //pair<>
#include <vector>
#include <string>
#include "../abstract/multipleobject.h"
#include "../definition.h"
using namespace std;


class Bloc : public MultipleObject{

public:
    static Bloc * getInstance();
    vector<pair<int8_t,int8_t>> *list_bloc;
    bool add(int8_t x, int8_t y) override;
    bool remove(int8_t x, int8_t y) override;
    bool set(std::vector<std::pair<int8_t, int8_t>> list){
        bool ret = clear();
        for(int8_t i=0; i<list.size(); ++i)
            list_bloc->push_back(list[i]);
        if(list_bloc->size() > 0)
            ret = false;
        return ret;
    }    
    bool has(int8_t x, int8_t y) override;
    bool clear() override{
        list_bloc->clear();
        return list_bloc->size() == 0;
    }
    string toString() override;
    ~Bloc(){
        delete []list_bloc;
        list_bloc = nullptr;
        delete instance;
        instance = nullptr;
    }

private:
    static Bloc * instance;
    Bloc(){
        list_bloc = new vector<pair<int8_t, int8_t>>();
        list_bloc->reserve(MAX_BLOC_SCENE);
    }
};

#endif