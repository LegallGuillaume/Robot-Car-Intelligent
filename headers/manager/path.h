#ifndef PATH_H
#define PATH_H


#include <utility> //pair<>
#include <vector>
#include <string>
#include "../abstract/multipleobject.h"
using namespace std;

/**
 * @file                path.h
 * @brief               Abstract of MultipleObject
 * @function            Virtual function
 * @version             1.0
 */
class Path : public MultipleObject{

public:
    /**
     * @file                path.h
     * @brief               Design patern singleton
     * @function            Singleton function
     * @version             1.0
     * 
     * @return Path*       have Path
     */
    static Path * getInstance();
    vector<pair<int8_t,int8_t>> *list_path;
    bool add(int8_t x, int8_t y) override;
    bool remove(int8_t x, int8_t y) override;
    bool has(int8_t x, int8_t y) override;
    bool clear() override{
        list_path->clear();
        return list_path->size() == 0;
    }
    string toString() override;
    ~Path(){
        delete []list_path;
        list_path = nullptr;
        delete instance;
        instance = nullptr;
    }

private:
    static Path * instance;
    Path(){
        list_path = new vector<pair<int8_t, int8_t>>();
    }
};

#endif