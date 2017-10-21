#ifndef BLOC_H
#define BLOC_H

#include <utility> //pair<>
#include <vector>
#include <string>
#include "../abstract/multipleobject.h"
using namespace std;


class Bloc : public MultipleObject{

public:
    static Bloc * getInstance();
    vector<pair<int,int>> *list_bloc;
    bool add(int x, int y) override;
    bool remove(int x, int y) override;
    bool has(int x, int y) override;
    bool clear() override{
        list_bloc->clear();
        return list_bloc->size() == 0;
    }
    string toString() override;
    ~Bloc(){
        delete list_bloc;
        list_bloc = nullptr;
        delete instance;
        instance = nullptr;
    }

private:
    static Bloc * instance;
    Bloc(){
        list_bloc = new vector<pair<int, int>>();
    }
};

#endif