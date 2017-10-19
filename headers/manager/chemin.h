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
    vector<pair<int,int>> *list_chemin;
    bool add(int x, int y) override;
    bool remove(int x, int y) override;
    bool has(int x, int y) override;
    bool clear() override{
        list_chemin->clear();
        return list_chemin->size() == 0;
    }
    string toString() override;

private:
    static Chemin * instance;
    Chemin(){
        list_chemin = new vector<pair<int, int>>();
    }
};

#endif