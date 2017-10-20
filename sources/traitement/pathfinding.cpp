
#include "../../headers/traitement/pathfinding.h"
#include "../../headers/manager.h"
#include "../../headers/definition.h"
#include <utility>
#include <vector>

using namespace std;

/*
vector<pair<int, int>> PathFinding::find(){
    return NULL;
}*/

/*http://www.robot-maker.com/forum/tutorials/article/35-exploration-pathfinding-dans-un-labyrinthe/*/

int PathFinding::test_case(int width, int height, int **tab, int x, int y){
    if(x>=0 && y>=0 && x<width && y<height && tab[x][y] == 0)
        return 1;
    return 0;
}

bool PathFinding::arround(int value, int search, int x, int y, int width, int height){
    if(x >= 0 && y >= 0 && x < width && y < height){
        return value == search;
    }
    return false;
}

int PathFinding::remplissage(int width, int height, int **tab, int nb_points, int *listX, int *listY,int Bx, int By){
    int *new_listX = (int*)malloc(sizeof(int)*nb_points*4);
    int *new_listY = (int*)malloc(sizeof(int)*nb_points*4);
    int new_nb_points=0;
    int i;
    bool hasPosibility;
    for(i=0;i<nb_points;i++)
    {
        hasPosibility = false;
        if(test_case(width,height,tab,listX[i]+1,listY[i]))
        {
            new_listX[new_nb_points] = listX[i]+1;
            new_listY[new_nb_points] = listY[i];
            tab[new_listX[new_nb_points]][new_listY[new_nb_points]] = tab[listX[i]][listY[i]] + 1;
            new_nb_points++;
            hasPosibility = true;
        }
        if(test_case(width,height,tab,listX[i]-1,listY[i]))
        {
            new_listX[new_nb_points] = listX[i]-1;
            new_listY[new_nb_points] = listY[i];
            tab[new_listX[new_nb_points]][new_listY[new_nb_points]] = tab[listX[i]][listY[i]] + 1;
            new_nb_points++;
            hasPosibility = true;
        }
        if(test_case(width,height,tab,listX[i],listY[i]+1))
        {
            new_listX[new_nb_points] = listX[i];
            new_listY[new_nb_points] = listY[i]+1;
            tab[new_listX[new_nb_points]][new_listY[new_nb_points]] = tab[listX[i]][listY[i]] + 1;
            new_nb_points++;
            hasPosibility = true;
        }
        if(test_case(width,height,tab,listX[i],listY[i]-1))
        {
            new_listX[new_nb_points] = listX[i];
            new_listY[new_nb_points] = listY[i]-1;
            tab[new_listX[new_nb_points]][new_listY[new_nb_points]] = tab[listX[i]][listY[i]] + 1;
            new_nb_points++;
            hasPosibility = true;
        }
    }
    free(listX);
    free(listY);
    if(hasPosibility){
        if(tab[Bx][By]!=0)
            return 1;
        else return remplissage(width, height, tab, new_nb_points, new_listX, new_listY,Bx, By);
    }else{
        return -1;
    }
}

int PathFinding::test()
{
    if(Manager::getInstance()->getSceneCarrer() > 0){
        int **tab;
        int Ax = Manager::getInstance()->car->getPosition().first;
        int Ay = Manager::getInstance()->car->getPosition().second;
        int Bx = Manager::getInstance()->arrive->getPosition().first;
        int By = Manager::getInstance()->arrive->getPosition().second;
        int i,j;
        int width = Manager::getInstance()->getSceneCarrer();
        int height = width;
        tab = Manager::getInstance()->getGeneralTable();
        tab[Bx][By] = 0;
        int *listX = (int*)malloc(sizeof(int)*1);
        int *listY = (int*)malloc(sizeof(int)*1);
        listX[0] = Ax;
        listY[0] = Ay;
        tab[Ax][Ay] = 1;
        int check = remplissage(width, width, tab, 1, listX, listY,Bx,By);
        if(check == -1){
            tab[Bx][By] = ARRIVE_DEF;
            return -1;
        }

        std::vector<std::pair<int, int>> *coord;
        coord = new vector<pair<int, int>>();
        coord->push_back(make_pair(Bx, By));
        int index = tab[Bx][By];
        int indexX = Bx;
        int indexY = By;
        bool hasPosibility;
        while(true){
            hasPosibility = false;
            if(indexY-1 >=0){
                if(arround(tab[indexX][indexY-1], index - 1, indexX, indexY-1, width, height)){
                    coord->push_back(make_pair(indexX, indexY-1));
                    --indexY;
                    --index;
                    hasPosibility = true;
                }
            }
            if(indexX-1 >=0){
                if(arround(tab[indexX-1][indexY], index - 1, indexX-1, indexY, width, height)){
                    coord->push_back(make_pair(indexX-1, indexY));
                    --indexX;
                    --index;
                    hasPosibility = true;
                }
            }
            if(indexY+1 <height){
                if(arround(tab[indexX][indexY+1], index - 1, indexX, indexY+1, width, height)){
                    coord->push_back(make_pair(indexX, indexY+1));
                    ++indexY;
                    --index;
                    hasPosibility = true;
                }
            }
            if(indexX+1 < width){
                if(arround(tab[indexX+1][indexY], index - 1, indexX+1, indexY, width, height)){
                    coord->push_back(make_pair(indexX+1, indexY));
                    ++indexX;
                    --index;
                    hasPosibility = true;
                }
            }
            if(!hasPosibility){
                return -1;
            }
            if(index == 1){
                break;
            }
        }
        tab[Bx][By] = ARRIVE_DEF;
        if(hasPosibility){
            for(int ad = coord->size() -1; ad > -1; --ad){
                printf("X%d Y%d, ", coord->at(ad).first, coord->at(ad).second);
            }
            printf("\n");
            return 0;
        }else{
            return -1;
        }
    }else{
        return -1;
    }
}