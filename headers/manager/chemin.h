#ifndef CHEMIN_H
#define CHEMIN_H


#include <utility> //pair<>
#include <vector>
#include <string>
#include "../abstract/multipleobject.h"
using namespace std;


class Chemin : public MultipleObject{

public:
    static Chemin * getInstance();
    vector<pair<int8_t,int8_t>> *list_chemin;
    bool add(int8_t x, int8_t y) override;
    bool remove(int8_t x, int8_t y) override;
    bool has(int8_t x, int8_t y) override;
    bool clear() override{
        list_chemin->clear();
        return list_chemin->size() == 0;
    }
    string toString() override;
    ~Chemin(){
        delete []list_chemin;
        list_chemin = nullptr;
        delete instance;
        instance = nullptr;
    }

private:
    static Chemin * instance;
    Chemin(){
        list_chemin = new vector<pair<int8_t, int8_t>>();
    }
};

#endif