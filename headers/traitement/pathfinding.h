#ifndef PATHFINDING_H
#define PATHFINDING_H


#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>
#include "cellule.h"
#include "../definition.h"
#include "../manager.h"

class PathFinding{

public:

    /**
     * @file                pathfinding.h
     * @brief               Search the best way to go to Arrival
     * @version             1.0
     * 
     * @param matrice       Matrix do not exceed 127x127 (contains blocks)
     * @param width_scene   Width of Matrix
     * @param height_scene  Height of Matrix
     * @param start         pair<int8_t,int8_t> position of start (x and y)
     * @param end           pair<int8_t,int8_t> position of Arrival (x and y)
     */
    PathFinding(int8_t **matrice, int8_t width_scene, /*matrice 0 to 127*/
        int8_t height_scene, std::pair<int8_t,int8_t> start, std::pair<int8_t,int8_t> end){
            start_coord = start;
            end_coord = end;
            width = width_scene;
            height = height_scene;
            _pathfinding = nullptr;
            if(surroundedBlocs(end.first, end.second, matrice) || surroundedBlocs(start.first, start.second, matrice)){
                return;
            }
            for(int8_t i_width = 0; i_width < width_scene; ++i_width){
                for(int8_t i_height= 0; i_height < height_scene; ++i_height){
                    bool isbloc = (matrice[i_width][i_height] == BLOC_DEF);
                    all_cellules.push_back(new Cellule(i_width, i_height, isbloc));
                }
            }
            Cellule *firstCell = getCellule(start.first, start.second);
            firstCell->m_P = 0;
            firstCell->everfind = true;
            current_list.push_back(firstCell);
            findPath();
            list_cheminOk = new std::vector<std::pair<int8_t, int8_t>>();
            list_cheminOk->reserve(width_scene*(width_scene/2));
        }
    
    /**
     * @file                pathfinding.h
     * @brief               Check if there are a solution
     * @version             1.0
     * 
     * @return bool         true if there are a Way to go to Arrival
     */
    bool hasPossibility(){
        return _pathfinding != nullptr;
    }

    /**
     * @file                pathfinding.h
     * @brief               return Way to go to Arrival
     * @version             1.0
     * 
     * @return std::vector<std::pair<int8_t, int8_t>>*  return vector to pair (position of way, first order Arrival)
     */
    std::vector<std::pair<int8_t, int8_t>>* getChemin(){
        list_cheminOk->clear();
        Cellule *current = _pathfinding;
        while(true){
            if(current == nullptr)
                break;
            if(current->getCoord() != start_coord)
                list_cheminOk->push_back(current->getCoord());
            if(current->m_parent != nullptr && !current->m_parent->everfind){
                current = current->m_parent;
                current->everfind = true;
            }else{
                break;
            }
        }
        //list_cheminOk->pop_back();
        return list_cheminOk;
    }

    /**
     * @file                pathfinding.h
     * @brief               Destructor to the class Pathfinding
     * @version             1.0 
     * 
     */
    ~PathFinding(){
        delete _pathfinding;
        _pathfinding = nullptr;
        delete []list_cheminOk;
        list_cheminOk = nullptr;
        for(uint8_t i=0; i<all_cellules.size(); ++i){
            delete all_cellules[i];
            all_cellules[i] = nullptr;
        }
        for(uint8_t i=0; i<current_list.size(); ++i){
            delete current_list[i];
            current_list[i] = nullptr;
        }
        
    }
private:
    Cellule *_pathfinding;
    std::vector<std::pair<int8_t,int8_t>> *list_cheminOk;

    /**
     * @file                pathfinding.h
     * @brief               Find Path
     * @version             1.0
     * 
     * recusive function
     * 
     * +---------------+---------------+---------------+
     * |   0   0   0   |   0   0   0   |   0   0   0   |
     * |   0   0   0   |   0   0   0   |   0   0   0   |
     * |   0   0   0   |  A2   B1  C1  |   0   0   0   |
     * +---------------+---------------+---------------+
     * |   0   0   A1  |   A   B   C   |   C2   0   0  |
     * |   0   0   D1  |   D   E   F   |   F1   0   0  |
     * |   0   0   G2  |   G   H   I   |   I2   0   0  |
     * +---------------+---------------+---------------+
     * |   0   0   0   |  G1   H1  I1  |   0   0   0   |
     * |   0   0   0   |   0   0   0   |   0   0   0   |
     * |   0   0   0   |   0   0   0   |   0   0   0   |
     * +---------------+---------------+---------------+
    */
    void findPath(){
        _pathfinding = nullptr;
        if(current_list.size() == 0)
            return;
        std::vector<Cellule*> actuel_list = current_list;
        current_list.clear();
        for(uint8_t index=0; index<(uint8_t)actuel_list.size(); ++index){
            Cellule* _currentCell = actuel_list.at(index);
            uint8_t newP = _currentCell->m_P+10;
            Cellule* down = getCellule(_currentCell->getCoord().first + 1, _currentCell->getCoord().second);
            Cellule* up = getCellule(_currentCell->getCoord().first - 1, _currentCell->getCoord().second);
            Cellule* right = getCellule(_currentCell->getCoord().first, _currentCell->getCoord().second + 1);
            Cellule* left = getCellule(_currentCell->getCoord().first, _currentCell->getCoord().second - 1);
            //
            Cellule* a1 = getCellule(_currentCell->getCoord().first - 1, _currentCell->getCoord().second - 2);
            Cellule* d1 = getCellule(_currentCell->getCoord().first, _currentCell->getCoord().second - 2);
            Cellule* g2 = getCellule(_currentCell->getCoord().first + 1, _currentCell->getCoord().second - 2);
            Cellule* g1 = getCellule(_currentCell->getCoord().first + 2, _currentCell->getCoord().second -1);
            Cellule* h1 = getCellule(_currentCell->getCoord().first + 2, _currentCell->getCoord().second);
            Cellule* i1 = getCellule(_currentCell->getCoord().first + 2, _currentCell->getCoord().second + 1);
            Cellule* i2 = getCellule(_currentCell->getCoord().first + 1, _currentCell->getCoord().second + 2);
            Cellule* f1 = getCellule(_currentCell->getCoord().first, _currentCell->getCoord().second + 2);
            Cellule* c2 = getCellule(_currentCell->getCoord().first - 1, _currentCell->getCoord().second + 2);
            Cellule* c1 = getCellule(_currentCell->getCoord().first - 2, _currentCell->getCoord().second +1);
            Cellule* b1 = getCellule(_currentCell->getCoord().first - 2, _currentCell->getCoord().second);
            Cellule* a2 = getCellule(_currentCell->getCoord().first - 2, _currentCell->getCoord().second -1);


            if(g1 != nullptr && h1 != nullptr && i1 != nullptr && down->m_parent != _currentCell){
                if(!g1->isBloc() && !h1->isBloc() && !i1->isBloc()){
                    if(newP < down->m_P && _currentCell->m_parent != down){
                        down->m_parent = _currentCell;
                        down->m_P = newP;
                        current_list.push_back(down);
                    }
                    if(down->getCoord().first == end_coord.first && down->getCoord().second == end_coord.second){
                        _pathfinding = _currentCell;
                        break;
                    }
                }
            }
            if(a2 != nullptr && b1 != nullptr && c1 != nullptr && up->m_parent != _currentCell){
                if(!a2->isBloc() && !b1->isBloc() && !c1->isBloc()){
                    if(newP < up->m_P && _currentCell->m_parent != up){
                        up->m_parent = _currentCell;
                        up->m_P = newP;
                        current_list.push_back(up);
                    }
                }
                if(up->getCoord().first == end_coord.first && up->getCoord().second == end_coord.second){
                    _pathfinding = _currentCell;
                    break;
                }
            }
            if(a1 != nullptr && d1 != nullptr && g2 != nullptr && left->m_parent != _currentCell){
                if(!a1->isBloc() && !d1->isBloc() && !g2->isBloc()){
                    if(newP < left->m_P && _currentCell->m_parent != left ){
                        left->m_parent = _currentCell;
                        left->m_P = newP;
                        current_list.push_back(left);
                    }
                }
                if(left->getCoord().first == end_coord.first && left->getCoord().second == end_coord.second){
                    _pathfinding = _currentCell;
                    break;
                }
            }
            if(c2 != nullptr && f1 !=nullptr && i2 != nullptr && right->m_parent != _currentCell){
                if(!c2->isBloc() && !f1->isBloc() && !i2->isBloc()){
                    if(newP < right->m_P && _currentCell->m_parent != right){
                        right->m_parent = _currentCell;
                        right->m_P = newP;
                        current_list.push_back(right);
                    }
                }
                if(right->getCoord().first == end_coord.first && right->getCoord().second == end_coord.second){
                    _pathfinding = _currentCell;
                    break;
                }
            }
        }
        if(_pathfinding == nullptr){
            findPath();
        } 
        return;
    }

    /**   
     * @file                pathfinding.h
     * @brief               Get Cellule from position X and position Y
     * @version             1.0
     * 
     * @param x             Postion X into Matrix
     * @param y             Position Y into Matrix
     * @return Cellule*     return the Cellule choosen
     */
    Cellule* getCellule(int8_t x, int8_t y){
        if(x < 0 || y < 0)
            return nullptr;
        if(x >= width || y >= height)
            return nullptr;
        uint16_t index = (height*x) + y;
        if(all_cellules.size() > index)
            return all_cellules.at(index);
        return nullptr;
    }

    /**
     * @file                pathfinding.h
     * @brief               Return true if the Arrival or Start surrounded Blocks
     * @version             1.0
     * 
     * @param x             Position X into Matrix
     * @param y             Position Y into Matrix
     * @param matrice       Matrix contains all blocks
     * @return true         if Arrival or Start is surrounded by block 
     * @return false        if not surrounded by block
     */
    bool surroundedBlocs(int8_t x, int8_t y, int8_t** matrice){
        if(x == 0){
            if(matrice[x+1][y] == 0)
                return false;
        }
        if(y == 0){
            if(matrice[x][y+1] == 0)
                return false;
        }
        if(0 < x && x<width-1){
            if(matrice[x+1][y] == 0 || matrice[x-1][y] == 0)
                return false;
        }
        if(0 < y && y<height-1){
            if(matrice[x][y+1] == 0 || matrice[x][y-1] == 0)
                return false;
        }
        if(x == width-1){
            if(matrice[x-1][y] == 0)
                return false;
        }
        if(y == height-1){
            if(matrice[x][y-1] == 0)
                return false;
        }
        return true;
    }

    std::vector<Cellule*> all_cellules;
    std::vector<Cellule*> current_list;
    int8_t width;
    int8_t height;
    std::pair<int8_t, int8_t> start_coord;
    std::pair<int8_t, int8_t> end_coord;

};

#endif