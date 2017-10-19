#ifndef BLOC_H
#define BLOC_H


#include <utility> //pair<>
#include <vector>
#include <string>
using namespace std;


class Bloc{

public:
    static Bloc * getInstance();
    vector<pair<int,int>> *list_bloc;
    bool add(int x, int y);
    bool remove(int x, int y); 
    bool has(int x, int y);
    bool clear();
    string toString();

private:
    static Bloc * instance;
    Bloc(){
        list_bloc = new vector<pair<int, int>>();
    }
};

#endif