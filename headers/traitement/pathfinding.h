#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>
#include "cellpathfinding.h"
#include "../definition.h"

class PathFinding{

public:
    PathFinding(int8_t **matrice, int8_t width_scene, /*matrice 0 to 127*/
        int8_t height_scene, std::pair<int8_t,int8_t> start, std::pair<int8_t,int8_t> end){
            start_coord = start;
            end_coord = end;
            width = width_scene;
            height = height_scene;
            if(surroundedBlocs(end.first, end.second, matrice) || surroundedBlocs(start.first, start.second, matrice)){
                has_possibility = false;
                return;
            }
            for(int8_t i_width = 0; i_width < width_scene; ++i_width){
                for(int8_t i_height= 0; i_height < height_scene; ++i_height){
                    /*if(!surroundedBlocs(i_width, i_height, matrice)) {
                        //uint8_t distCell = distanceCellule(std::make_pair(i_width, i_height), end);
                        uint8_t distCell = 10;
                        all_cellules.push_back(new Cellule(i_width, i_height, distCell));
                    }*/
                    bool isbloc = (matrice[i_width][i_height] == BLOC_DEF);
                    all_cellules.push_back(new Cellule(i_width, i_height, isbloc));
                }
            }
            if(all_cellules.size() == 0){
                has_possibility = false;
                return;
            }
            Cellule *firstCell = getCellule(start.first, start.second);
            firstCell->m_P = 0;
            current_list.push_back(firstCell);
            findPath();
            list_cheminOk = new std::vector<std::pair<int8_t, int8_t>>();
        }
    
    bool hasPossibility(){
        return has_possibility;
    }

    std::vector<std::pair<int8_t, int8_t>>* getChemin(){
        list_cheminOk->clear();
        Cellule *current = _pathfinding;
        while(true){
            if(current == nullptr)
                break;
            list_cheminOk->push_back(current->getCoord());
            if(current->m_parent != nullptr){
                current = current->m_parent;
            }else{
                break;
            }
        }
        list_cheminOk->pop_back();
        return list_cheminOk;
    }

private:
    Cellule *_pathfinding;
    std::vector<std::pair<int8_t,int8_t>> *list_cheminOk;
    void findPath(){
        _pathfinding = nullptr;
        if(current_list.size() == 0)
            return;
        std::vector<Cellule*> actuel_list = current_list;
        current_list.clear();
        for(uint8_t index=0; index<(uint8_t)actuel_list.size(); ++index){
            Cellule* _currentCell = actuel_list.at(index);
            bool culsac = true;
            uint16_t newP = _currentCell->m_P+10;
            Cellule* down = getCellule(_currentCell->getCoord().first + 1, _currentCell->getCoord().second);
            Cellule* up = getCellule(_currentCell->getCoord().first - 1, _currentCell->getCoord().second);
            Cellule* right = getCellule(_currentCell->getCoord().first, _currentCell->getCoord().second + 1);
            Cellule* left = getCellule(_currentCell->getCoord().first, _currentCell->getCoord().second - 1);
            if(down != nullptr){
                if(!down->isBloc()){
                    if(newP < down->m_P){
                        down->m_parent = _currentCell;
                        down->m_P = newP;
                        current_list.push_back(down);
                        culsac = false;
                    }
                    if(down->getCoord().first == end_coord.first && down->getCoord().second == end_coord.second){
                        _pathfinding = _currentCell;
                        break;
                    }
                }
            }
            if(up != nullptr){
                if(!up->isBloc()){
                    if(newP < up->m_P){
                        up->m_parent = _currentCell;
                        up->m_P = newP;
                        current_list.push_back(up);
                        culsac = false;
                    }
                }
                if(up->getCoord().first == end_coord.first && up->getCoord().second == end_coord.second){
                    _pathfinding = _currentCell;
                    break;
                }
            }
            if(left != nullptr){
                if(!left->isBloc()){
                    if(newP < left->m_P){
                        left->m_parent = _currentCell;
                        left->m_P = newP;
                        current_list.push_back(left);
                        culsac = false;
                    }
                }
                if(left->getCoord().first == end_coord.first && left->getCoord().second == end_coord.second){
                    _pathfinding = _currentCell;
                    break;
                }
            }
            if(right != nullptr){
                if(!right->isBloc()){
                    if(newP < right->m_P){
                        right->m_parent = _currentCell;
                        right->m_P = newP;
                        current_list.push_back(right);
                        culsac = false;
                    }
                }
                if(right->getCoord().first == end_coord.first && right->getCoord().second == end_coord.second){
                    _pathfinding = _currentCell;
                    break;
                }
            }
            if(culsac){
                _currentCell->setBloc(true);
            }
        }

        if(_pathfinding == nullptr){
            findPath();
        }
        return;
    }

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

    bool surroundedBlocs(int8_t x, int8_t y, int8_t** matrice){ /*return true if Cellule is surrounded by block*/
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
    Cellule* getParent(Cellule *cellcul){ /*supprimer la branche qui a un cul de sac */
        if(cellcul->m_parent != nullptr){
            cellcul->setBloc(true);
        }
        return cellcul;
    }
    uint8_t distance(std::pair<int8_t,int8_t> target, std::pair<int8_t,int8_t> end);
    std::vector<Cellule*> all_cellules;
    std::vector<Cellule*> current_list;
    int8_t width;
    int8_t height;
    bool has_possibility = true;
    std::pair<uint8_t, uint8_t> start_coord;
    std::pair<uint8_t, uint8_t> end_coord;

};