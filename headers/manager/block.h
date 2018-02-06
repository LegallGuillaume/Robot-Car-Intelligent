#ifndef BLOCK_H
#define BLOCK_H

#include <utility> //pair<>
#include <vector>
#include <string>
#include "../abstract/multipleobject.h"
#include "../definition.h"
using namespace std;

/**
 * @file                block.h
 * @brief               Abstract of MultipleObject
 * @function            Virtual function
 * @version             1.0
 */
class Block : public MultipleObject{

public:
    /**
     * @file                block.h
     * @brief               Design patern singleton
     * @function            Singleton function
     * @version             1.0
     * 
     * @return Block*       have Block
     */
    static Block * getInstance();
    vector<pair<int8_t,int8_t>> *list_block;
    bool add(int8_t x, int8_t y) override;
    bool remove(int8_t x, int8_t y) override;
    bool set(std::vector<std::pair<int8_t, int8_t>> list){
        bool ret = clear();
        for(int8_t i=0; i<list.size(); ++i)
            list_block->push_back(list[i]);
        if(list_block->size() > 0)
            ret = false;
        return ret;
    }    
    bool has(int8_t x, int8_t y) override;
    bool clear() override{
        list_block->clear();
        return list_block->size() == 0;
    }
    string toString() override;
    ~Block(){
        delete []list_block;
        list_block = nullptr;
        delete instance;
        instance = nullptr;
    }

private:
    static Block * instance;
    Block(){
        list_block = new vector<pair<int8_t, int8_t>>();
        list_block->reserve(MAX_BLOC_SCENE);
    }
};

#endif